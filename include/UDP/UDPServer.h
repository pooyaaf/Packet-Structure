#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <sys/types.h>
#include <cstdint>

// Add the structure definitions here
struct PacketHeader
{
    uint32_t size;
    char start[4];
};

struct Counter
{
    uint32_t size;
    char start[4];
};

struct Timestamp
{
    uint64_t size;
    char start[8];
};

struct UID
{
    uint64_t size;
    char start[16];
};

struct Payload
{
    uint16_t size;
    char data[256];
};

struct Reserved
{
    uint64_t size;
    char start[8];
};

struct PacketFooter
{
    uint32_t size;
    char end[4];
};

class UDPServer
{
public:
    void run();
};
#endif
