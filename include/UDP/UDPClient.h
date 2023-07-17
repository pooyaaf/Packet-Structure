#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <sys/types.h>
#include <string>
class UDPClient
{
public:
    void checkHeaderFooter(const char *buffer, int dataSize, int counter);
    void run();
};

#endif // UDP_CLIENT_H
