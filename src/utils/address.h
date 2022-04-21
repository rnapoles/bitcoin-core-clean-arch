#ifndef UTILS_ADDRESS_H
#define UTILS_ADDRESS_H

#include <vector>
#include <string>
#include <utils/macros.h>
#include <encode/base58.h>

using namespace std;


static const unsigned char ADDRESSVERSION = 0;

inline string Hash160ToAddress(uint160 hash160)
{
    // add 1-byte version number to the front
    vector<unsigned char> vch(1, ADDRESSVERSION);
    vch.insert(vch.end(), UBEGIN(hash160), UEND(hash160));
    return EncodeBase58Check(vch);
}

inline bool AddressToHash160(const char* psz, uint160& hash160Ret)
{
    vector<unsigned char> vch;
    if (!DecodeBase58Check(psz, vch))
        return false;
    if (vch.empty())
        return false;
    unsigned char nVersion = vch[0];
    if (vch.size() != sizeof(hash160Ret) + 1)
        return false;
    memcpy(&hash160Ret, &vch[1], sizeof(hash160Ret));
    return (nVersion <= ADDRESSVERSION);
}

inline bool AddressToHash160(const string& str, uint160& hash160Ret)
{
    return AddressToHash160(str.c_str(), hash160Ret);
}

inline bool IsValidBitcoinAddress(const char* psz)
{
    uint160 hash160;
    return AddressToHash160(psz, hash160);
}

inline bool IsValidBitcoinAddress(const string& str)
{
    return IsValidBitcoinAddress(str.c_str());
}

inline string PubKeyToAddress(const vector<unsigned char>& vchPubKey)
{
    return Hash160ToAddress(Hash160(vchPubKey));
}

#endif