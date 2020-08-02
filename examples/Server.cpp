#include <stdio.h>
#include "net/net.h"
#include <cstring>
#include <string>



int main(const int argc, const char** argv) {

    if(argc < 2) {
        printf("You need to provide a port\n");
        return 0;
    }

    uint16_t port = atoi(argv[1]);

    net::Initialize();

    
    net::Address address = net::ResolveHostname(NULL, port);
    net::Socket socket = net::Socket::CreateSocket(&address);

    net::Address clientAddress;
    char data[256] = { 0 };
    socket.Receive(data, sizeof(data), &clientAddress);
    printf("Received Data from: %s:%d\n", clientAddress.ip, clientAddress.port);
    printf("%s\n", data);


    const char* response = "thanks for the request!\n";
    socket.Send(response, strlen(response), clientAddress);



    net::FreeAddress(&clientAddress);
    net::FreeAddress(&address);
    socket.Close();
    net::Cleanup();
    return 0;
}