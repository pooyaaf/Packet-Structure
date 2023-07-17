#include "../../include/TCP/TCPServer.h"
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

void TCPServer::run()
{
    // Create socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
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

    std::cout << "TCP Server is listening for incoming connections..." << std::endl;

    // Listen for incoming connections
    listen(sockfd, 1);
    socklen_t clientLen = sizeof(clientAddr);

    // Accept a client connection
    int clientSocket = accept(sockfd, (struct sockaddr *)&clientAddr, &clientLen);
    if (clientSocket < 0)
    {
        std::cerr << "Error accepting client connection." << std::endl;
        return;
    }

    std::cout << "TCP Client connected. Sending the file..." << std::endl;

    // Wait for 1 second before sending the file
    std::this_thread::sleep_for(std::chrono::seconds(1));

    // Send the connection confirmation to the client
    const char *connectionConfirmedMsg = "ConnectionConfirmed";
    send(clientSocket, connectionConfirmedMsg, strlen(connectionConfirmedMsg), 0);

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
        send(clientSocket, packetBuffer, bytesRead, 0);

        // Print the packet number and its size
        std::cout << "Packet #" << packetNumber << " (Size: " << bytesRead << " bytes)" << std::endl;

        // Print the received packet data (assuming it's a string)
        packetBuffer[bytesRead] = '\0'; // Null-terminate the received data to print as a string
        std::cout << "Received Packet Data: " << packetBuffer << std::endl;

        // Increment the packet number for the next packet
        packetNumber++;
    }

    // Close the file and sockets
    file.close();
    close(clientSocket);
    close(sockfd);

    std::cout << "File sent successfully." << std::endl;

    return;
}
