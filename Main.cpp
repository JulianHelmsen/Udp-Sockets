#include <stdio.h>
#include "net/net.h"
#include <cstring>

int main(const int argc, const char** argv) {
    net::Initialize();
    net::Address address = net::ResolveHostname(net::any_address, 80);
    net::Address destination = net::ResolveHostname(net::localhost, 8000);

    net::Socket socket = net::Socket::CreateSocket(address);

    const char* msg = "Hallo";
    uint32_t send = socket.Send((const void*) msg, strlen(msg), destination);
    printf("send %d bytes\n", send);

    char recBuffer[256] = { 0 };
    net::Address addr;
    socket.Receive(recBuffer, sizeof(recBuffer), &addr);

    printf("%s:%d\n", addr.ip, addr.port);


    socket.Close();
    net::FreeAddress(&address);
    net::Cleanup();
    return 0;
}