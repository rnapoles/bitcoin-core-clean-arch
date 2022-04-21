#include <cstdio>
#include <domain/cdisktxpos.h>
#include <utils/string.h>

CDiskTxPos::CDiskTxPos()
{
    SetNull();
}

CDiskTxPos::CDiskTxPos(unsigned int nFileIn, unsigned int nBlockPosIn, unsigned int nTxPosIn)
{
    nFile = nFileIn;
    nBlockPos = nBlockPosIn;
    nTxPos = nTxPosIn;
}

void CDiskTxPos::SetNull() { 
  nFile = -1; 
  nBlockPos = 0; 
  nTxPos = 0; 
}

bool CDiskTxPos::IsNull() const { 
  return (nFile == -1); 
}

string CDiskTxPos::ToString() const
{
    if (IsNull())
        return strprintf("null");
    else
        return strprintf("(nFile=%d, nBlockPos=%d, nTxPos=%d)", nFile, nBlockPos, nTxPos);
}

void CDiskTxPos::print() const
{
    printf("%s", this->ToString().c_str());
}
