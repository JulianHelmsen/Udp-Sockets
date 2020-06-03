#include "net/net.h"
#include <cstring>
#include <stdlib.h>
#include <stdexcept>

#include <stdio.h>

#include <WS2tcpip.h>
#include <WinSock2.h>

namespace net {



    Address ResolveHostname(const char* hostname, const uint16_t port) {
        Address address;
        
        address.port = port;
        
        if(hostname) {
            uint32_t hostnameLen = strlen(hostname);
            address.ip = (char*) malloc(hostnameLen + 1);
            memcpy(address.ip, hostname, hostnameLen);
            address.ip[hostnameLen] = 0;
            address.ipLen = hostnameLen;
        }
        
        sockaddr_in* nativeAddress = (sockaddr_in*) calloc(1, sizeof(sockaddr_in));
        address.nativeAddress = nativeAddress;
        address.nativeAddressSize = sizeof(sockaddr_in);

        nativeAddress->sin_addr.s_addr = hostname ? inet_addr(hostname) : INADDR_ANY;
        nativeAddress->sin_port = htons(port);
        nativeAddress->sin_family = AF_INET;

        return address;
    }

    
    Socket Socket::CreateSocket(const Address& address) {
        Socket result;
        SOCKET nativeSocket = socket(AF_INET, SOCK_DGRAM, 0);
        result.m_nativeSocket = (void*) nativeSocket;

        if(bind(nativeSocket, (SOCKADDR*) address.nativeAddress, address.nativeAddressSize)) {
            throw std::runtime_error("Could not bind socket");
        }
        return result;
    }

    uint32_t Socket::Send(const void* buffer, const uint32_t bufferSize, const Address& dest) {
        SOCKET nativeSocket = (SOCKET) m_nativeSocket;
        int result = sendto(nativeSocket, (char*) buffer, bufferSize, 0, (const sockaddr*) dest.nativeAddress, dest.nativeAddressSize);
        if(result == SOCKET_ERROR) {
            throw std::runtime_error("Could not send data");
        }
        
        return (uint32_t) result;
    }

    uint32_t Socket::Receive(void* buffer, const uint32_t bufferSize, Address* address) {
        SOCKET nativeSocket = (SOCKET) m_nativeSocket;
        FreeAddress(address);
        address->nativeAddressSize = sizeof(sockaddr_in);
        sockaddr_in* nativeAddress = (sockaddr_in*) malloc(address->nativeAddressSize);;
        address->nativeAddress = (void*) nativeAddress;
        int received = recvfrom(nativeSocket, (char*) buffer, bufferSize, 0, (sockaddr*) nativeAddress, (int*) &address->nativeAddressSize);
        
        if(received == SOCKET_ERROR) {
            throw std::runtime_error("Could not received from socket");
        }

        printf("Received: %d\n", received);

        char* ip = inet_ntoa(((sockaddr_in*)address->nativeAddress)->sin_addr);
        uint32_t len = strlen(ip);
        address->ip = (char*) malloc(len + 1);
        address->ipLen = len;
        memcpy(address->ip, ip, len);
        address->ip[address->ipLen] = 0;
        address->port = htons(nativeAddress->sin_port);


        return received;
    }

    void FreeAddress(const Address* address) {
        if(address->ip)
            free(address->ip);
        if(address->nativeAddress)
            free(address->nativeAddress);
    }

    void Initialize() {
        WSAData data;
        WSAStartup(MAKEWORD(2, 2), &data);
    }

    void Cleanup() {
        WSACleanup();
    }

    
    void Socket::Close() {
        if(closesocket((SOCKET) m_nativeSocket) == SOCKET_ERROR) {
            throw std::runtime_error("Could not close socket");
        }
    }
    
}