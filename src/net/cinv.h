#ifndef NET_CINV_H
#define NET_CINV_H

#include <stdexcept>
#include <string>
#include <uint256.h>
#include <serialize.h>
#include <net/net.h>
#include <utils/macros.h>
#include <utils/string.h>

using namespace std;

class CInv
{
public:
    int type;
    uint256 hash;

    CInv()
    {
        type = 0;
        hash = 0;
    }

    CInv(int typeIn, const uint256& hashIn)
    {
        type = typeIn;
        hash = hashIn;
    }

    CInv(const string& strType, const uint256& hashIn)
    {
        int i;
        for (i = 1; i < ARRAYLEN(ppszTypeName); i++)
        {
            if (strType == ppszTypeName[i])
            {
                type = i;
                break;
            }
        }
        if (i == ARRAYLEN(ppszTypeName))
            throw std::out_of_range(strprintf("CInv::CInv(string, uint256) : unknown type '%s'", strType.c_str()));
        hash = hashIn;
    }

    IMPLEMENT_SERIALIZE
    (
        READWRITE(type);
        READWRITE(hash);
    )

    friend inline bool operator<(const CInv& a, const CInv& b)
    {
        return (a.type < b.type || (a.type == b.type && a.hash < b.hash));
    }

    bool IsKnownType() const
    {
        return (type >= 1 && type < ARRAYLEN(ppszTypeName));
    }

    const char* GetCommand() const
    {
        if (!this->IsKnownType())
            throw std::out_of_range(strprintf("CInv::GetCommand() : type=% unknown type", type));
        return ppszTypeName[type];
    }

    string ToString() const
    {
        return strprintf("%s %s", this->GetCommand(), hash.ToString().substr(0,14).c_str());
    }

    void print() const
    {
        printf("CInv(%s)\n", ToString().c_str());
    }
};

#endif