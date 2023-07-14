#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <sys/types.h> // Add this line

class UDPServer
{
public:
    void run();
    bool checkHeaderFooter(const char *data, ssize_t bytesReceived);
};

#endif // UDP_SERVER_H
