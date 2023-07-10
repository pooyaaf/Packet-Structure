#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <fstream>
#include <string>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <fstream>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345

#define PACKET_SIZE 300
#define HEADER_OFFSET 0
#define FOOTER_OFFSET 296
#define COUNTER_OFFSET 4

bool validatePacket(char *packet)
{
    return (packet[HEADER_OFFSET] == 0xAF && packet[HEADER_OFFSET + 1] == 0xDB && packet[HEADER_OFFSET + 2] == 0xAA &&
            packet[HEADER_OFFSET + 3] == 0x00 && packet[FOOTER_OFFSET] == 0xFF && packet[FOOTER_OFFSET + 1] == 0xAA &&
            packet[FOOTER_OFFSET + 2] == 0xDF && packet[FOOTER_OFFSET + 3] == 0x00);
}

int extractCounter(char *packet)
{
    int counter = 0;
    counter |= packet[COUNTER_OFFSET] << 24;
    counter |= packet[COUNTER_OFFSET + 1] << 16;
    counter |= packet[COUNTER_OFFSET + 2] << 8;
    counter |= packet[COUNTER_OFFSET + 3];
    return counter;
}

int main()
{
    int serverSocket = socket(AF_INET, SOCK_DGRAM, 0);

    struct sockaddr_in serverAddr;
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP);

    bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    char packet[PACKET_SIZE];

    while (true)
    {
        struct sockaddr_in clientAddr;
        socklen_t addrLen = sizeof(clientAddr);
        ssize_t recvSize = recvfrom(serverSocket, packet, PACKET_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);

        if (recvSize < 0)
        {
            std::cerr << "Failed to receive data." << std::endl;
            break;
        }

        if (validatePacket(packet))
        {
            int counter = extractCounter(packet);
            std::cout << "Counter: " << counter << std::endl;
        }
        else
        {
            std::cout << "false" << std::endl;
        }
    }

    close(serverSocket);

    return 0;
}
