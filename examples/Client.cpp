#include <stdio.h>
#include "net/net.h"
#include <cstring>
#include <string>

#ifdef WINDOWS
#include <windows.h>
void SleepMs(uint32_t ms) {
    Sleep(ms);
}
#elif defined(LINUX)
#include <unistd.h>
void SleepMs(uint32_t ms) {
    usleep(ms * 1000);
}
#endif 




int main(const int argc, const char** argv) {
    if(argc < 3) {
        printf("You need to provide an ip and port\n");
        return 0;
    }

    uint16_t port = (uint16_t) atoi(argv[2]);
    printf("connect to: %s:%d\n", argv[1], port);

    net::Initialize();
  
    net::Socket socket = net::Socket::CreateSocket(NULL);
    net::Address destination = net::ResolveHostname(argv[1], port);
    printf("Created socket\n");

    const char* data = "Hey server!\n";
    socket.Send(data, strlen(data), destination);
    printf("Send data!\n");

    SleepMs(200);

    char buffer[256] = { 0 };
    net::Address address;
    uint32_t received = socket.Receive(buffer, sizeof(buffer), &address);
    buffer[received] = 0; // null termination
    printf("received: \n%s\n", buffer);


    net::FreeAddress(&address);
    net::FreeAddress(&destination);
    socket.Close();
    net::Cleanup();
    return 0;
}