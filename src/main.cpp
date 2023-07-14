#include "../include/UDP/UDPServer.h"
#include "../include/UDP/UDPClient.h"
#include <iostream>

using namespace std;

int main()
{
    std::cout << "Choose an option:" << std::endl;
    std::cout << "1. TCP" << std::endl;
    std::cout << "2. UDP" << std::endl;

    int option;
    std::cin >> option;
    int option2;

    if (option == 1)
    {
        cout << "not yet";
    }
    else if (option == 2)
    {
        std::cout << "Choose an option:" << std::endl;
        std::cout << "1. UDP Server" << std::endl;
        std::cout << "2. UDP Client" << std::endl;
        std::cin >> option2;
        if (option2 == 1)
        {
            UDPServer udpServer;
            udpServer.run();
        }
        else if (option2 == 2)
        {
            UDPClient udpClient;
            udpClient.run();
        }
    }
    else
    {
        std::cout << "Invalid option" << std::endl;
    }

    return 0;
}
