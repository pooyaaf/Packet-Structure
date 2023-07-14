#include "../../include/UDP/UDPServer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iomanip>

#define PORT 12345
#define BUFFER_SIZE 1024

void UDPServer::run()
{
    int sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        close(sockfd);
        return;
    }

    char buffer[BUFFER_SIZE];
    int counter = 0;

    while (true)
    {
        ssize_t bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (bytesReceived < 0)
        {
            std::cerr << "Error receiving data" << std::endl;
            close(sockfd);
            return;
        }

        // Copy a portion of buffer to buffer1
        int j = 0;
        while (j < bytesReceived / 20)
        {
            int startIndex = 0 + j * 20;
            int endIndex = 20 + j * 20;
            int buffer1Size = endIndex - startIndex;
            char buffer1[buffer1Size];
            memcpy(buffer1, buffer + startIndex, buffer1Size);
            if (checkHeaderFooter(buffer1, buffer1Size))
            {
                counter++;
                std::cout << "\nCounter value: " << counter << std::endl;
            }
            for (int i = 0; i < buffer1Size; ++i)
            {
                unsigned char byte = static_cast<unsigned char>(buffer1[i]);
                std::cout << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(byte) << " ";
            }
            std::cout << std::endl
                      << "---- ------- ----" << std::endl;
            j++;
        }

        std::cout << "Received packet:" << std::endl;
        for (int i = 0; i < bytesReceived; ++i)
        {
            unsigned char byte = static_cast<unsigned char>(buffer[i]);
            std::cout << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(byte) << " ";
        }
        std::cout << std::endl;

        if (bytesReceived < BUFFER_SIZE)
        {
            std::cout << "Counter: " << counter << std::endl
                      << "Exiting the Program ...\n";
            break;
        }
    }

    close(sockfd);
}

bool UDPServer::checkHeaderFooter(const char *data, ssize_t bytesReceived)
{
    auto formatByteString = [](const char *byte)
    {
        std::ostringstream oss;
        unsigned char byteVal = static_cast<unsigned char>(*byte);
        oss << "0x" << std::uppercase << std::hex << std::setw(2) << std::setfill('0') << static_cast<unsigned int>(byteVal) << " ";
        return oss.str();
    };

    std::cout << "Header Format is: "
              << "0xAF 0xDB 0xAA 0x00" << std::endl;
    std::cout << "Footer Format is: "
              << "0xFF 0xAA 0xDF 0x00" << std::endl;

    auto checkFormat = [](const std::string &byteString, const std::string &expected)
    {
        return byteString == expected;
    };

    std::string byteString0 = formatByteString(&data[0]);
    std::string byteString1 = formatByteString(&data[1]);
    std::string byteString2 = formatByteString(&data[2]);
    std::string byteString3 = formatByteString(&data[3]);

    std::string byteString4 = formatByteString(&data[16]);
    std::string byteString5 = formatByteString(&data[17]);
    std::string byteString6 = formatByteString(&data[18]);
    std::string byteString7 = formatByteString(&data[19]);

    if (checkFormat(byteString0, "0xAF ") && checkFormat(byteString1, "0xDB ") &&
        checkFormat(byteString2, "0xAA ") && checkFormat(byteString3, "0x00 "))
    {
        std::cout << "Header Check passed successfully!" << std::endl;
        if (checkFormat(byteString4, "0xFF ") && checkFormat(byteString5, "0xAA ") &&
            checkFormat(byteString6, "0xDF ") && checkFormat(byteString7, "0x00 "))
        {
            std::cout << "Footer Check passed successfully!";
            return true;
        }
    }

    return false;
}
