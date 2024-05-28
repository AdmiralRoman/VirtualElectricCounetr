#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <string.h>
#include <string>
#include <stdexcept>
#include "handler.h"

extern std::vector <uint8_t> sendbuf;
 
int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <number>" << std::endl;
        return -1;
    }

    int x = 0;
    try {
        x = std::stoi(argv[1]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Invalid number: " << argv[1] << std::endl;
        return -1;
    } catch (const std::out_of_range& e) {
        std::cerr << "Number out of range: " << argv[1] << std::endl;
        return -1;
    }
    // Create a socket
    int listening = socket(AF_INET, SOCK_STREAM, 0);
    if (listening == -1)
    {
        std::cerr << "Can't create a socket! Quitting" << std::endl;
        return -1;
    }
    
    // Bind the ip address and port to a socket
    sockaddr_in hint;
    hint.sin_family = AF_INET;
    hint.sin_port = htons(x);
    inet_pton(AF_INET, "0.0.0.0", &hint.sin_addr);

    bind(listening, (sockaddr*)&hint, sizeof(hint));
 
    // Tell Winsock the socket is for listening
    listen(listening, SOMAXCONN);

    std::cout << "Server is up!\n" ;
 
    // Wait for a connection
    sockaddr_in client;
    socklen_t clientSize = sizeof(client);
 
    int clientSocket = accept(listening, (sockaddr*)&client, &clientSize);

    char host[NI_MAXHOST];      // Client's remote name
    char service[NI_MAXSERV];   // Service (i.e. port) the client is connect on
 
    memset(host, 0, NI_MAXHOST); // same as memset(host, 0, NI_MAXHOST);
    memset(service, 0, NI_MAXSERV);
 
    if (getnameinfo((sockaddr*)&client, sizeof(client), host, NI_MAXHOST, service, NI_MAXSERV, 0) == 0)
    {
        std::cout << host << " connected on port " << service << std::endl;
    }
    else
    {
        inet_ntop(AF_INET, &client.sin_addr, host, NI_MAXHOST);
        std::cout << host << " connected on port " << ntohs(client.sin_port) << std::endl;
    }
 
    // Close listening socket
    close(listening);
    
    // While loop: accept and echo message back to client
    std::vector <uint8_t> buf(40);
    bool status = false;

    while (true)
    {
        //memset(&buf, 0, 40);

        // Wait for client to send data
        int bytesReceived = recv(clientSocket, &buf[0], buf.size(), 0);
        if (bytesReceived > 1)
        {
            Handling hand;
            status = hand.checkAdr(buf);
            if (!status) 
            {
                return -1;
                std::cerr << "Wrong address";
            }
            printf("Bytes received: %d\n :", bytesReceived);
            for (int i = 0;i<bytesReceived;i++) 
                std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int> (buf[i]) << " ";
            std::cout << std::endl;
            
            CRC crc;
            
            hand.switchRule(buf);
            crc.Crc16(sendbuf);
            crc.retCrc(sendbuf);
            
            
            printf("Bytes send: %d :\n", bytesReceived);
            for (auto i : sendbuf) std::cout << std::hex << std::setw(2) << std::setfill('0') << static_cast<int> (i) << " ";
            std::cout << std::endl;
            int iSendResult = send(clientSocket, &sendbuf[0], sendbuf.size(), 0);
            sendbuf.clear();
        }
        if (bytesReceived == -1)
        {
            std::cerr << "Error in recv(). Quitting" << std::endl;
            break;
        }
 
        if (bytesReceived == 0)
        {
            std::cout << "Client disconnected " << std::endl;
            break;
        }

    }
 
    // Close the socket
    close(clientSocket);
 
    return 0;
}
