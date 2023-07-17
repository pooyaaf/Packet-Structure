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
#include <chrono>
#include <thread>

#define SERVER_IP "127.0.0.1"
const int BUFFER_SIZE = 3000;
const int PORT = 12345;
int counter = 1; // Initialize the counter

void UDPClient::checkHeaderFooter(const char *buffer, int dataSize, int counter)
{
    std::string strHeader;
    for (int i = 1; i <= 19; ++i)
    {
        strHeader += buffer[i];
    }
    std::cout << "\n\nHeader: " << strHeader << std::endl;
    std::string strFooter;
    for (int i = 19; i >= 1; --i)
    {
        strFooter += buffer[dataSize - i];
    }

    std::cout << "Footer: " << strFooter << std::endl;

    std::string headerCheck = "0xAF 0xDB 0xAA 0x00";
    std::string footerCheck = "0xFF 0xAA 0xDF 0x00";

    if (strHeader == headerCheck)
    {
        if (strFooter == footerCheck)
        {
            std::cout << "Counter: " << counter << std::endl;
        }
        else
        {
            std::cout << "\n *** Error find in Footer of the packer! Not match *** \n";
        }
    }
    else
    {
        std::cout << "\n *** Error find in Header of the packer! Not match *** \n";
    }
}

void UDPClient::run()
{
    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error opening socket." << std::endl;
        return;
    }

    // Set up server address
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    inet_pton(AF_INET, SERVER_IP, &(serverAddr.sin_addr));

    // Send a connection request to the server
    const char *connectionRequest = "Connect";
    sendto(sockfd, connectionRequest, strlen(connectionRequest), 0, (struct sockaddr *)&serverAddr, sizeof(serverAddr));

    // Receive file data in chunks
    char buffer[BUFFER_SIZE];
    socklen_t serverLen = sizeof(serverAddr);
    int bytesReceived;
    int totalBytesReceived = 0;

    bool connectionConfirmed = false;

    do
    {
        bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&serverAddr, &serverLen);
        if (bytesReceived > 0)
        {
            // Check if the connection is confirmed
            if (!connectionConfirmed)
            {
                std::string receivedData(buffer, bytesReceived);
                if (receivedData == "ConnectionConfirmed")
                {
                    connectionConfirmed = true;
                    continue; // Skip printing this message
                }
            }

            // Print the received packet (assuming it's a string)
            buffer[bytesReceived] = '\0'; // Null-terminate the received data to print as a string
            std::cout << buffer;

            // Call the checkHeaderFooter function to check the header and footer
            checkHeaderFooter(buffer, bytesReceived, counter);
            counter = counter + 2;
            // Keep track of the total bytes received
            totalBytesReceived += bytesReceived;

            // Check if 3000 bytes have been received, then print the separator
            if (totalBytesReceived >= BUFFER_SIZE)
            {
                std::cout << "\n -------------------------------------------- \n";
                totalBytesReceived = 0; // Reset for the next packet
            }
        }
    } while (bytesReceived > 0);

    // Close the socket
    close(sockfd);

    return;
}