#ifndef NET_NET_H
#define NET_NET_H

#include <winsock.h>

static const unsigned short DEFAULT_PORT = htons(8333);

static const unsigned int PUBLISH_HOPS = 5;

enum
{
    NODE_NETWORK = (1 << 0),
};

enum
{
    MSG_TX = 1,
    MSG_BLOCK,
    MSG_REVIEW,
    MSG_PRODUCT,
    MSG_TABLE,
};

static const char* ppszTypeName[] =
{
    "ERROR",
    "tx",
    "block",
    "review",
    "product",
    "table",
};

#endif