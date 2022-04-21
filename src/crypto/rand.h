#ifndef CRYPTO_RAND_H
#define CRYPTO_RAND_H

#include <types.h>

void RandAddSeed(bool fPerfmon=false);
uint64 GetRand(uint64 nMax);

#endif