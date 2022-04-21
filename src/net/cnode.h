#ifndef NET_CNODE_H
#define NET_CNODE_H

#include <openssl/rand.h>
#include <openssl/sha.h>
#include <windows.h>
#include <winsock.h>
#include <vector>
#include <set>
#include <globals.h>
#include <serialize.h>
#include <uint256.h>
#include <thread/sync.h>
#include <crypto/rand.h>
#include <net/caddress.h>
#include <net/crequesttracker.h>
#include <net/cinv.h>
#include <net/cmessageheader.h>
#include <net/shared.h>

using namespace std;

class CNode
{
public:
    // socket
    uint64 nServices;
    SOCKET hSocket;
    CDataStream vSend;
    CDataStream vRecv;
    CCriticalSection cs_vSend;
    CCriticalSection cs_vRecv;
    unsigned int nPushPos;
    CAddress addr;
    int nVersion;
    bool fClient;
    bool fInbound;
    bool fNetworkNode;
    bool fDisconnect;
protected:
    int nRefCount;
public:
    int64 nReleaseTime;
    map<uint256, CRequestTracker> mapRequests;
    CCriticalSection cs_mapRequests;

    // flood
    vector<CAddress> vAddrToSend;
    set<CAddress> setAddrKnown;

    // inventory based relay
    set<CInv> setInventoryKnown;
    set<CInv> setInventoryKnown2;
    vector<CInv> vInventoryToSend;
    CCriticalSection cs_inventory;
    multimap<int64, CInv> mapAskFor;

    // publish and subscription
    vector<char> vfSubscribe;


    CNode(SOCKET hSocketIn, CAddress addrIn, bool fInboundIn=false)
    {
        nServices = 0;
        hSocket = hSocketIn;
        vSend.SetType(SER_NETWORK);
        vRecv.SetType(SER_NETWORK);
        nPushPos = -1;
        addr = addrIn;
        nVersion = 0;
        fClient = false; // set by version message
        fInbound = fInboundIn;
        fNetworkNode = false;
        fDisconnect = false;
        nRefCount = 0;
        nReleaseTime = 0;
        vfSubscribe.assign(256, false);

        // Push a version message
        /// when NTP implemented, change to just nTime = GetAdjustedTime()
        int64 nTime = (fInbound ? GetAdjustedTime() : GetTime());
        PushMessage("version", VERSION, nLocalServices, nTime, addr);
    }

    ~CNode()
    {
        if (hSocket != INVALID_SOCKET)
            closesocket(hSocket);
    }

private:
    CNode(const CNode&);
    void operator=(const CNode&);
public:


    bool ReadyToDisconnect()
    {
        return fDisconnect || GetRefCount() <= 0;
    }

    int GetRefCount()
    {
        return max(nRefCount, 0) + (GetTime() < nReleaseTime ? 1 : 0);
    }

    void AddRef(int64 nTimeout=0)
    {
        if (nTimeout != 0)
            nReleaseTime = max(nReleaseTime, GetTime() + nTimeout);
        else
            nRefCount++;
    }

    void Release()
    {
        nRefCount--;
    }



    void AddInventoryKnown(const CInv& inv)
    {
        CRITICAL_BLOCK(cs_inventory)
            setInventoryKnown.insert(inv);
    }

    void PushInventory(const CInv& inv)
    {
        CRITICAL_BLOCK(cs_inventory)
            if (!setInventoryKnown.count(inv))
                vInventoryToSend.push_back(inv);
    }

    void AskFor(const CInv& inv)
    {
        // We're using mapAskFor as a priority queue,
        // the key is the earliest time the request can be sent
        int64& nRequestTime = mapAlreadyAskedFor[inv];
        printf("askfor %s  %I64d\n", inv.ToString().c_str(), nRequestTime);

        // Make sure not to reuse time indexes to keep things in the same order
        int64 nNow = (GetTime() - 1) * 1000000;
        static int64 nLastTime;
        nLastTime = nNow = max(nNow, ++nLastTime);

        // Each retry is 2 minutes after the last
        nRequestTime = max(nRequestTime + 2 * 60 * 1000000, nNow);
        mapAskFor.insert(make_pair(nRequestTime, inv));
    }



    void BeginMessage(const char* pszCommand)
    {
        EnterCriticalSection(&cs_vSend);
        if (nPushPos != -1)
            AbortMessage();
        nPushPos = vSend.size();
        vSend << CMessageHeader(pszCommand, 0);
        printf("sending: %-12s ", pszCommand);
    }

    void AbortMessage()
    {
        if (nPushPos == -1)
            return;
        vSend.resize(nPushPos);
        nPushPos = -1;
        LeaveCriticalSection(&cs_vSend);
        printf("(aborted)\n");
    }

    void EndMessage()
    {
        if (nDropMessagesTest > 0 && GetRand(nDropMessagesTest) == 0)
        {
            printf("dropmessages DROPPING SEND MESSAGE\n");
            AbortMessage();
            return;
        }

        if (nPushPos == -1)
            return;

        // Patch in the size
        unsigned int nSize = vSend.size() - nPushPos - sizeof(CMessageHeader);
        memcpy((char*)&vSend[nPushPos] + offsetof(CMessageHeader, nMessageSize), &nSize, sizeof(nSize));

        printf("(%d bytes)  ", nSize);
        //for (int i = nPushPos+sizeof(CMessageHeader); i < min(vSend.size(), nPushPos+sizeof(CMessageHeader)+20U); i++)
        //    printf("%02x ", vSend[i] & 0xff);
        printf("\n");

        nPushPos = -1;
        LeaveCriticalSection(&cs_vSend);
    }

    void EndMessageAbortIfEmpty()
    {
        if (nPushPos == -1)
            return;
        int nSize = vSend.size() - nPushPos - sizeof(CMessageHeader);
        if (nSize > 0)
            EndMessage();
        else
            AbortMessage();
    }

    const char* GetMessageCommand() const
    {
        if (nPushPos == -1)
            return "";
        return &vSend[nPushPos] + offsetof(CMessageHeader, pchCommand);
    }




    void PushMessage(const char* pszCommand)
    {
        try
        {
            BeginMessage(pszCommand);
            EndMessage();
        }
        catch (...)
        {
            AbortMessage();
            throw;
        }
    }

    template<typename T1>
    void PushMessage(const char* pszCommand, const T1& a1)
    {
        try
        {
            BeginMessage(pszCommand);
            vSend << a1;
            EndMessage();
        }
        catch (...)
        {
            AbortMessage();
            throw;
        }
    }

    template<typename T1, typename T2>
    void PushMessage(const char* pszCommand, const T1& a1, const T2& a2)
    {
        try
        {
            BeginMessage(pszCommand);
            vSend << a1 << a2;
            EndMessage();
        }
        catch (...)
        {
            AbortMessage();
            throw;
        }
    }

    template<typename T1, typename T2, typename T3>
    void PushMessage(const char* pszCommand, const T1& a1, const T2& a2, const T3& a3)
    {
        try
        {
            BeginMessage(pszCommand);
            vSend << a1 << a2 << a3;
            EndMessage();
        }
        catch (...)
        {
            AbortMessage();
            throw;
        }
    }

    template<typename T1, typename T2, typename T3, typename T4>
    void PushMessage(const char* pszCommand, const T1& a1, const T2& a2, const T3& a3, const T4& a4)
    {
        try
        {
            BeginMessage(pszCommand);
            vSend << a1 << a2 << a3 << a4;
            EndMessage();
        }
        catch (...)
        {
            AbortMessage();
            throw;
        }
    }


    void PushRequest(const char* pszCommand,
                     void (*fn)(void*, CDataStream&), void* param1)
    {
        uint256 hashReply;
        RAND_bytes((unsigned char*)&hashReply, sizeof(hashReply));

        CRITICAL_BLOCK(cs_mapRequests)
            mapRequests[hashReply] = CRequestTracker(fn, param1);

        PushMessage(pszCommand, hashReply);
    }

    template<typename T1>
    void PushRequest(const char* pszCommand, const T1& a1,
                     void (*fn)(void*, CDataStream&), void* param1)
    {
        uint256 hashReply;
        RAND_bytes((unsigned char*)&hashReply, sizeof(hashReply));

        CRITICAL_BLOCK(cs_mapRequests)
            mapRequests[hashReply] = CRequestTracker(fn, param1);

        PushMessage(pszCommand, hashReply, a1);
    }

    template<typename T1, typename T2>
    void PushRequest(const char* pszCommand, const T1& a1, const T2& a2,
                     void (*fn)(void*, CDataStream&), void* param1)
    {
        uint256 hashReply;
        RAND_bytes((unsigned char*)&hashReply, sizeof(hashReply));

        CRITICAL_BLOCK(cs_mapRequests)
            mapRequests[hashReply] = CRequestTracker(fn, param1);

        PushMessage(pszCommand, hashReply, a1, a2);
    }



    bool IsSubscribed(unsigned int nChannel);
    void Subscribe(unsigned int nChannel, unsigned int nHops=0);
    void CancelSubscribe(unsigned int nChannel);
    void Disconnect();
};

#endif