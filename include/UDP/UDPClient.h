#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H
#include <sys/types.h> // Add this line
#include <string>
class UDPClient
{
public:
    std::string convertHexToChar(const std::string &hexString);

    void run();
};

#endif // UDP_CLIENT_H
