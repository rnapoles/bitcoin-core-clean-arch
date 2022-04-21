#ifndef GLOBALS_H
#define GLOBALS_H

#include <types.h>

static const unsigned int MAX_SIZE = 0x02000000;
static const int64 COIN = 100000000;
static const int64 CENT = 1000000;
static const int COINBASE_MATURITY = 100;

static int nBestHeight = -1;
static unsigned int nTransactionsUpdated = 0;

#endif