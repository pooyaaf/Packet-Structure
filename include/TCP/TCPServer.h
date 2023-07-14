#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <sys/types.h> // Add this line

class TCPServer
{
public:
    void run();
    bool checkHeaderFooter(const char *data, ssize_t bytesReceived);
};

#endif // TCP_SERVER_H
