#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <chrono>
#include <thread>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <cstring>

constexpr int CLIENT_DATA_BUFFER_SIZE = 300;
constexpr int PORT = 8888;

struct Packet
{
    uint8_t header[4];
    uint32_t counter;
    uint64_t timestamp;
    uint64_t uid[2];
    uint16_t payload[128];
    uint8_t reserved[8];
    uint8_t footer[4];
};

std::vector<Packet> loadPacketsFromFile(const std::string &filename)
{
    std::vector<Packet> packets;
    std::ifstream file(filename, std::ios::binary);

    if (!file)
    {
        std::cerr << "Error opening file " << filename << std::endl;
        return packets;
    }

    while (!file.eof())
    {
        Packet packet;
        file.read(reinterpret_cast<char *>(&packet.header), sizeof(packet.header));
        file.read(reinterpret_cast<char *>(&packet.counter), sizeof(packet.counter));
        packet.counter = ntohl(packet.counter);
        file.read(reinterpret_cast<char *>(&packet.timestamp), sizeof(packet.timestamp));
        file.read(reinterpret_cast<char *>(&packet.uid), sizeof(packet.uid));
        file.read(reinterpret_cast<char *>(&packet.payload), sizeof(packet.payload));
        file.read(reinterpret_cast<char *>(&packet.reserved), sizeof(packet.reserved));
        file.read(reinterpret_cast<char *>(&packet.footer), sizeof(packet.footer));

        if (file.gcount() == 0)
        {
            break;
        }

        if (file.gcount() < sizeof(Packet))
        {
            std::cerr << "Incomplete packet in file " << filename << std::endl;
            break;
        }

        packets.push_back(packet);
    }

    file.close();
    return packets;
}

void sendPackets(const std::vector<Packet> &packets, const std::string &ip)
{
    // Create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Configure server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = inet_addr(ip.c_str());

    // Send packets
    for (const auto &packet : packets)
    {
        sendto(sock, &packet, sizeof(Packet), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    // Close the socket
    close(sock);
}

void receivePackets()
{
    // Create a socket
    int sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock < 0)
    {
        std::cerr << "Error creating socket" << std::endl;
        return;
    }

    // Configure server address
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    // Bind the socket
    if (bind(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {
        std::cerr << "Error binding socket" << std::endl;
        return;
    }

    // Receive and process packets
    Packet packet;
    struct sockaddr_in client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    while (true)
    {
        recvfrom(sock, &packet, sizeof(Packet), 0, (struct sockaddr *)&client_addr, &client_addr_len);

        // Check header and footer, and print the counter if they are correct
        if (memcmp(packet.header, "\xAF\xDB\xAA\x00", 4) == 0 &&
            memcmp(packet.footer, "\xFF\xAA\xDF\x00", 4) == 0)
        {
            uint32_t counter = ntohl(packet.counter);
            std::cout << "Counter: " << counter << std::endl;
        }
        else
        {
            std::cout << "Invalid packet" << std::endl;
        }
    }

    // Close the socket
    close(sock);
}
int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        std::cerr << "Usage: " << argv[0] << " client|server [input_file]" << std::endl;
        return 1;
    }

    std::string mode = argv[1];
    if (mode == "client")
    {
        if (argc < 4)
        {
            std::cerr << "Usage: " << argv[0] << " client [input_file] [server_ip]" << std::endl;
            return 1;
        }
        std::string input_file = argv[2];
        std::string server_ip = argv[3];
        auto packets = loadPacketsFromFile(input_file);
        sendPackets(packets, server_ip);
    }
    else if (mode == "server")
    {
        receivePackets();
    }
    else
    {
        std::cerr << "Invalid mode: " << mode << std::endl;
        return 1;
    }

    return 0;
}