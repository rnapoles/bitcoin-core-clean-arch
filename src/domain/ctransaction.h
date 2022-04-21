#ifndef DOMAIN_CTRANSACTION_H
#define DOMAIN_CTRANSACTION_H

#include <vector>
#include <globals.h>
#include <serialize.h>
#include <thread/sync.h>
#include <domain/ctxin.h>
#include <domain/ctxout.h>
#include <domain/coutpoint.h>
#include <domain/cinpoint.h>
#include <domain/cdisktxpos.h>

//
// The basic transaction that is broadcasted on the network and contained in
// blocks.  A transaction can contain multiple inputs and outputs.
//
class CTransaction
{
public:
    int nVersion;
    vector<CTxIn> vin;
    vector<CTxOut> vout;
    int nLockTime;

    CTransaction()
    {
        SetNull();
    }

    IMPLEMENT_SERIALIZE
    (
        READWRITE(this->nVersion);
        nVersion = this->nVersion;
        READWRITE(vin);
        READWRITE(vout);
        READWRITE(nLockTime);
    )

    void SetNull()
    {
        nVersion = 1;
        vin.clear();
        vout.clear();
        nLockTime = 0;
    }

    bool IsNull() const
    {
        return (vin.empty() && vout.empty());
    }

    uint256 GetHash() const
    {
        return SerializeHash(*this);
    }

    bool IsFinal() const
    {
        if (nLockTime == 0 || nLockTime < nBestHeight){
            return true;
        }

        for(const CTxIn& txin: vin)
        {
            if (!txin.IsFinal())
                return false;
        }

        return true;
    }

    bool IsNewerThan(const CTransaction& old) const
    {
        if (vin.size() != old.vin.size())
            return false;
        for (int i = 0; i < vin.size(); i++)
            if (vin[i].prevout != old.vin[i].prevout)
                return false;

        bool fNewer = false;
        unsigned int nLowest = UINT_MAX;
        for (int i = 0; i < vin.size(); i++)
        {
            if (vin[i].nSequence != old.vin[i].nSequence)
            {
                if (vin[i].nSequence <= nLowest)
                {
                    fNewer = false;
                    nLowest = vin[i].nSequence;
                }
                if (old.vin[i].nSequence < nLowest)
                {
                    fNewer = true;
                    nLowest = old.vin[i].nSequence;
                }
            }
        }
        return fNewer;
    }

    bool IsCoinBase() const
    {
        return (vin.size() == 1 && vin[0].prevout.IsNull());
    }

    bool CheckTransaction() const;

    bool IsMine() const
    {
        for(const CTxOut& txout: vout)
            if (txout.IsMine())
                return true;
        return false;
    }

    int64 GetDebit() const
    {
        int64 nDebit = 0;
        for(const CTxIn& txin: vin)
            nDebit += txin.GetDebit();
        return nDebit;
    }

    int64 GetCredit() const
    {
        int64 nCredit = 0;
        for(const CTxOut& txout: vout)
            nCredit += txout.GetCredit();
        return nCredit;
    }

    int64 GetValueOut() const
    {
        int64 nValueOut = 0;
        for(const CTxOut& txout: vout)
        {
            if (txout.nValue < 0)
                throw runtime_error("CTransaction::GetValueOut() : negative value");
            nValueOut += txout.nValue;
        }
        return nValueOut;
    }

    int64 GetMinFee(bool fDiscount=false) const
    {
        // Base fee is 1 cent per kilobyte
        unsigned int nBytes = ::GetSerializeSize(*this, SER_NETWORK);
        int64 nMinFee = (1 + (int64)nBytes / 1000) * CENT;

        // First 100 transactions in a block are free
        if (fDiscount && nBytes < 10000)
            nMinFee = 0;

        // To limit dust spam, require a 0.01 fee if any output is less than 0.01
        if (nMinFee < CENT)
            for(const CTxOut& txout: vout)
                if (txout.nValue < CENT)
                    nMinFee = CENT;

        return nMinFee;
    }

    bool ReadFromDisk(CDiskTxPos pos, FILE** pfileRet = nullptr);


    friend bool operator==(const CTransaction& a, const CTransaction& b)
    {
        return (a.nVersion  == b.nVersion &&
                a.vin       == b.vin &&
                a.vout      == b.vout &&
                a.nLockTime == b.nLockTime);
    }

    friend bool operator!=(const CTransaction& a, const CTransaction& b)
    {
        return !(a == b);
    }


    string ToString() const
    {
        string str;
        str += strprintf("CTransaction(hash=%s, ver=%d, vin.size=%d, vout.size=%d, nLockTime=%d)\n",
            GetHash().ToString().substr(0,6).c_str(),
            nVersion,
            vin.size(),
            vout.size(),
            nLockTime);
        for (int i = 0; i < vin.size(); i++)
            str += "    " + vin[i].ToString() + "\n";
        for (int i = 0; i < vout.size(); i++)
            str += "    " + vout[i].ToString() + "\n";
        return str;
    }

    void print() const
    {
        printf("%s", ToString().c_str());
    }


    /*
    bool DisconnectInputs(CTxDB& txdb);
    bool ConnectInputs(CTxDB& txdb, map<uint256, CTxIndex>& mapTestPool, CDiskTxPos posThisTx, int nHeight, int64& nFees, bool fBlock, bool fMiner, int64 nMinFee=0);

    bool AcceptTransaction(CTxDB& txdb, bool fCheckInputs=true, bool* pfMissingInputs=NULL);

    bool AcceptTransaction(bool fCheckInputs=true, bool* pfMissingInputs=NULL)
    {
        CTxDB txdb("r");
        return AcceptTransaction(txdb, fCheckInputs, pfMissingInputs);
    }
    */
    
    bool ClientConnectInputs();

protected:
    bool AddToMemoryPool();
public:
    bool RemoveFromMemoryPool();
};

static CCriticalSection cs_mapTransactions;
static map<uint256, CTransaction> mapTransactions;
static map<COutPoint, CInPoint> mapNextTx;

#endif