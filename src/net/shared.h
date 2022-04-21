#ifndef NET_SHARED_H
#define NET_SHARED_H

#include <net/cinv.h>

static bool fClient = false;
static uint64 nLocalServices = (fClient ? 0 : NODE_NETWORK);
static map<CInv, int64> mapAlreadyAskedFor;
static int nDropMessagesTest = 0;

#endif