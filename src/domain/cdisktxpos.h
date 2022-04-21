#ifndef CDISKTXPOS_H
#define CDISKTXPOS_H

#include <string>
#include <serialize.h>

using namespace std;

class CDiskTxPos
{
public:
    unsigned int nFile;
    unsigned int nBlockPos;
    unsigned int nTxPos;

    CDiskTxPos();
    CDiskTxPos(unsigned int nFileIn, unsigned int nBlockPosIn, unsigned int nTxPosIn);

    IMPLEMENT_SERIALIZE( READWRITE(FLATDATA(*this)); )

    void SetNull();
    bool IsNull() const;
    
    friend bool operator==(const CDiskTxPos& a, const CDiskTxPos& b)
    {
        return (a.nFile     == b.nFile &&
                a.nBlockPos == b.nBlockPos &&
                a.nTxPos    == b.nTxPos);
    }
    friend bool operator!=(const CDiskTxPos& a, const CDiskTxPos& b)
    {
        return !(a == b);
    }

    string ToString() const;

    void print() const;
};

#endif