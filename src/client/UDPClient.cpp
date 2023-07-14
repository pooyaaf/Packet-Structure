#include "../../include/UDP/UDPClient.h"
#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

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

    char packet[] = "\xAF\xDB\xAA\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00";

    ssize_t bytesSent = sendto(sockfd, packet, sizeof(packet) - 1, 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));
    if (bytesSent < 0)
    {
        std::cerr << "Error sending data" << std::endl;
        close(sockfd);
        return;
    }

    close(sockfd);
}
