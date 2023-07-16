#include "../../include/UDP/UDPServer.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <iomanip>
#include <chrono>
#include <thread>
#include <cstdint>

const int BUFFER_SIZE = 3000;
const int PORT = 12345;

void UDPServer::run()
{
    // Create socket
    int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        std::cerr << "Error opening socket." << std::endl;
        return;
    }

    // Bind socket
    sockaddr_in serverAddr, clientAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) < 0)
    {
        std::cerr << "Error binding socket." << std::endl;
        return;
    }

    std::cout << "UDP Server is waiting for the client to connect..." << std::endl;

    // Wait for the client to connect (assuming it's a UDP client)
    socklen_t clientLen = sizeof(clientAddr);
    char buffer[BUFFER_SIZE];
    int bytesReceived;
    bool clientConnected = false;

    do
    {
        bytesReceived = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&clientAddr, &clientLen);
        if (bytesReceived > 0)
        {
            clientConnected = true;
        }
    } while (!clientConnected);

    std::cout << "Client connected. Waiting for 1 second before sending the file..." << std::endl;

    // Wait for 1 second
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Send the connection confirmation to the client
    const char *connectionConfirmedMsg = "ConnectionConfirmed";
    sendto(sockfd, connectionConfirmedMsg, strlen(connectionConfirmedMsg), 0, (struct sockaddr *)&clientAddr, clientLen);

    // Open the file to send
    std::ifstream file("../Packet.txt", std::ios::binary);
    if (!file)
    {
        std::cerr << "Error opening the file." << std::endl;
        return;
    }

    // Read and send the file data in chunks
    int bytesRead;
    char packetBuffer[BUFFER_SIZE];
    int packetNumber = 1;
    while (!file.eof())
    {
        // Read a chunk of data from the file
        file.read(packetBuffer, BUFFER_SIZE);
        bytesRead = file.gcount();

        // Send the packet data to the client
        sendto(sockfd, packetBuffer, bytesRead, 0, (struct sockaddr *)&clientAddr, clientLen);

        // Print the packet number and its size
        std::cout << "Packet #" << packetNumber << " (Size: " << bytesRead << " bytes)" << std::endl;

        // Print the received packet data (assuming it's a string)
        buffer[bytesReceived] = '\0'; // Null-terminate the received data to print as a string
        std::cout << "Received Packet Data: " << buffer << std::endl;

        // Increment the packet number for the next packet
        packetNumber++;
    }

    // Close the file and socket
    file.close();
    close(sockfd);

    std::cout << "File sent successfully." << std::endl;

    return;
}