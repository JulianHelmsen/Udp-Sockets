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
    net::Address address = net::ResolveHostname(NULL, 28852);

    net::Socket socket = net::Socket::CreateSocket(&address);

    
	char recBuffer[256] = { 0 };
    net::Address addr;
    socket.Receive(recBuffer, sizeof(recBuffer), &addr);

	printf("received: %s\n", recBuffer);
    printf("%s:%d\n", addr.ip, addr.port);


    SleepMs(100);

    const char* msg = "Hallo";
    uint32_t send = socket.Send((const void*) msg, strlen(msg), addr);
    printf("send %d bytes\n", send);





    socket.Close();
    net::FreeAddress(&address);
    net::Cleanup();
    return 0;
}
