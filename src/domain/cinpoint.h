#ifndef DOMAIN_CINPOINT_H
#define DOMAIN_CINPOINT_H

class CTransaction;

class CInPoint
{
public:
    CTransaction* ptx;
    unsigned int n;

    CInPoint() { SetNull(); }
    CInPoint(CTransaction* ptxIn, unsigned int nIn) { ptx = ptxIn; n = nIn; }
    void SetNull() { ptx = nullptr; n = -1; }
    bool IsNull() const { return (ptx == nullptr && n == -1); }
};

#endif