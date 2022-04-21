#ifndef NET_CADDRESS_H
#define NET_CADDRESS_H

#include <winsock.h>
#include <cstring>
#include <cstdio>
#include <vector>
#include <string>
#include <types.h>
#include <serialize.h>
#include <net/net.h>
#include <utils/macros.h>
#include <utils/time.h>
#include <utils/string.h>

using namespace std;

static const unsigned char pchIPv4[12] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0xff, 0xff };

class CAddress
{
public:
    uint64 nServices;
    unsigned char pchReserved[12];
    unsigned int ip;
    unsigned short port;

    // disk only
    unsigned int nTime;

    // memory only
    unsigned int nLastFailed;

    CAddress()
    {
        nServices = 0;
        memcpy(pchReserved, pchIPv4, sizeof(pchReserved));
        ip = 0;
        port = DEFAULT_PORT;
        nTime = GetAdjustedTime();
        nLastFailed = 0;
    }

    CAddress(unsigned int ipIn, unsigned short portIn=DEFAULT_PORT, uint64 nServicesIn=0)
    {
        nServices = nServicesIn;
        memcpy(pchReserved, pchIPv4, sizeof(pchReserved));
        ip = ipIn;
        port = portIn;
        nTime = GetAdjustedTime();
        nLastFailed = 0;
    }

    explicit CAddress(const struct sockaddr_in& sockaddr, uint64 nServicesIn=0)
    {
        nServices = nServicesIn;
        memcpy(pchReserved, pchIPv4, sizeof(pchReserved));
        ip = sockaddr.sin_addr.s_addr;
        port = sockaddr.sin_port;
        nTime = GetAdjustedTime();
        nLastFailed = 0;
    }

    explicit CAddress(const char* pszIn, uint64 nServicesIn=0)
    {
        nServices = nServicesIn;
        memcpy(pchReserved, pchIPv4, sizeof(pchReserved));
        ip = 0;
        port = DEFAULT_PORT;
        nTime = GetAdjustedTime();
        nLastFailed = 0;

        char psz[100];
        if (strlen(pszIn) > ARRAYLEN(psz)-1)
            return;
        strcpy(psz, pszIn);
        unsigned int a, b, c, d, e;
        if (sscanf(psz, "%u.%u.%u.%u:%u", &a, &b, &c, &d, &e) < 4)
            return;
        char* pszPort = strchr(psz, ':');
        if (pszPort)
        {
            *pszPort++ = '\0';
            port = htons(atoi(pszPort));
        }
        ip = inet_addr(psz);
    }

    IMPLEMENT_SERIALIZE
    (
        if (nType & SER_DISK)
        {
            READWRITE(nVersion);
            READWRITE(nTime);
        }
        READWRITE(nServices);
        READWRITE(FLATDATA(pchReserved));
        READWRITE(ip);
        READWRITE(port);
    )

    friend inline bool operator==(const CAddress& a, const CAddress& b)
    {
        return (memcmp(a.pchReserved, b.pchReserved, sizeof(a.pchReserved)) == 0 &&
                a.ip   == b.ip &&
                a.port == b.port);
    }

    friend inline bool operator<(const CAddress& a, const CAddress& b)
    {
        int ret = memcmp(a.pchReserved, b.pchReserved, sizeof(a.pchReserved));
        if (ret < 0)
            return true;
        else if (ret == 0)
        {
            if (ntohl(a.ip) < ntohl(b.ip))
                return true;
            else if (a.ip == b.ip)
                return ntohs(a.port) < ntohs(b.port);
        }
        return false;
    }

    vector<unsigned char> GetKey() const
    {
        CDataStream ss;
        ss.reserve(18);
        ss << FLATDATA(pchReserved) << ip << port;

        #if defined(_MSC_VER) && _MSC_VER < 1300
        return vector<unsigned char>((unsigned char*)&ss.begin()[0], (unsigned char*)&ss.end()[0]);
        #else
        return vector<unsigned char>(ss.begin(), ss.end());
        #endif
    }

    struct sockaddr_in GetSockAddr() const
    {
        struct sockaddr_in sockaddr;
        sockaddr.sin_family = AF_INET;
        sockaddr.sin_addr.s_addr = ip;
        sockaddr.sin_port = port;
        return sockaddr;
    }

    bool IsIPv4() const
    {
        return (memcmp(pchReserved, pchIPv4, sizeof(pchIPv4)) == 0);
    }

    bool IsRoutable() const
    {
        return !(GetByte(3) == 10 || (GetByte(3) == 192 && GetByte(2) == 168) || GetByte(3) == 127 || GetByte(3) == 0);
    }

    unsigned char GetByte(int n) const
    {
        return ((unsigned char*)&ip)[3-n];
    }

    string ToStringIPPort() const
    {
        return strprintf("%u.%u.%u.%u:%u", GetByte(3), GetByte(2), GetByte(1), GetByte(0), ntohs(port));
    }

    string ToStringIP() const
    {
        return strprintf("%u.%u.%u.%u", GetByte(3), GetByte(2), GetByte(1), GetByte(0));
    }

    string ToString() const
    {
        return strprintf("%u.%u.%u.%u:%u", GetByte(3), GetByte(2), GetByte(1), GetByte(0), ntohs(port));
        //return strprintf("%u.%u.%u.%u", GetByte(3), GetByte(2), GetByte(1), GetByte(0));
    }

    void print() const
    {
        printf("CAddress(%s)\n", ToString().c_str());
    }
    
};

static CAddress addrIncoming;

#endif