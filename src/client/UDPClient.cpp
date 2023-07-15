#include "../../include/UDP/UDPClient.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

std::string UDPClient::convertHexToChar(const std::string &hexString)
{
    std::string charString;
    std::string hexValue;
    std::istringstream iss(hexString);

    while (std::getline(iss, hexValue, '\\'))
    {
        if (hexValue.empty())
            continue;

        unsigned int value;
        std::stringstream ss;
        ss << std::hex << hexValue;
        ss >> value;

        charString += static_cast<char>(value);
    }

    return charString;
}

void UDPClient::run()
{
    int sockfd;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    // char packet[] = "\xAF\xDB\xAA\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00";

    std::ifstream myfile("../test.txt");
    std::string mystring;
    std::string mystring2;
    if (myfile.is_open())
    {                          // always check whether the file is open
        myfile >> mystring;    // pipe file's content into stream
        std::cout << mystring; // pipe stream's content to standard output
    }
    mystring2 = convertHexToChar(mystring);

    // std::cout << sizeof(packet);
    std::cout << mystring2.length();
    std::cout << mystring.c_str() << "\n";
    ssize_t bytesSent = sendto(sockfd, mystring.c_str(), mystring.length(), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (bytesSent < 0)
    {
        std::cerr << "Error sending data" << std::endl;
        close(sockfd);
        return;
    }

    close(sockfd);
}
