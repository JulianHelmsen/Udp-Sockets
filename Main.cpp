#include <stdio.h>
#include "net/net.h"
#include <cstring>



int main(const int argc, const char** argv) {
    net::Initialize();
    
    net::Address destination = net::ResolveHostname("169.254.87.203", 28852);

    net::Socket socket = net::Socket::CreateSocket();

    const char* msg = "Hallo";
    uint32_t send = socket.Send((const void*) msg, strlen(msg), destination);
    printf("send %d bytes\n", send);

    char recBuffer[256] = { 0 };
    net::Address addr;
    socket.Receive(recBuffer, sizeof(recBuffer), &addr);

	printf("received: %s\n", recBuffer);
    printf("%s:%d\n", addr.ip, addr.port);




    socket.Close();
    net::Cleanup();
    return 0;
}
