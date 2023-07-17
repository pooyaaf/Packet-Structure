#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <sys/types.h>
#include <string>
class TCPClient
{
public:
    void run();
    void checkHeaderFooter(const char *buffer, int dataSize, int counter);
};

#endif // TCP_CLIENT_H
