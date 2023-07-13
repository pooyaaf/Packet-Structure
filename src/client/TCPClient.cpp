#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h> // Include this header

using namespace std;
#define SERVER_IP "127.0.0.1"
#define PORT 12345
#define BUFFER_SIZE 1024

int main()
{
    int sockfd;
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return 1;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    if (connect(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error connecting to the server" << std::endl;
        close(sockfd);
        return 1;
    }

    char packet[] = "\xAF\xDB\xAA\x00\x00\x00\x00\x01\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x02\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x03\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x04\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x05\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00\xAF\xDB\xAA\x00\x00\x00\x00\x06\x00\x00\x00\x00\x00\x00\x00\x00\xFF\xAA\xDF\x00";

    ssize_t bytesSent = send(sockfd, packet, sizeof(packet) - 1, 0);
    if (bytesSent < 0)
    {
        std::cerr << "Error sending data" << std::endl;
        close(sockfd);
        return 1;
    }

    close(sockfd);

    return 0;
}
