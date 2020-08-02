#include <stdio.h>
#include "net/net.h"
#include <cstring>

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
    net::Initialize();
    
    net::Address destination = net::ResolveHostname("169.254.87.203", 28852);

    net::Socket socket = net::Socket::CreateSocket();

    const char* msg = "Hallo";
    uint32_t send = socket.Send((const void*) msg, strlen(msg), destination);
    printf("send %d bytes\n", send);

    SleepMs(100);

    char recBuffer[256] = { 0 };
    net::Address addr;
    socket.Receive(recBuffer, sizeof(recBuffer), &addr);

	printf("received: %s\n", recBuffer);
    printf("%s:%d\n", addr.ip, addr.port);




    socket.Close();
    net::Cleanup();
    return 0;
}
