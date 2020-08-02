#ifndef NET_H
#define NET_H

#include <stdint.h>
#include <cstddef>

namespace net {

    static const char* localhost = "127.0.0.1";
    static const char* any_address = NULL;
    
    struct Address {
        Address() : ip(NULL), port(0), nativeAddress(NULL), nativeAddressSize(0), ipLen(0) {}
        char* ip;
        uint8_t ipLen; // not including null byte
        uint16_t port;


        void* nativeAddress;
        uint32_t nativeAddressSize;
    };


    class Socket {
    public:
        static Socket CreateSocket() {return CreateSocket(NULL);}
        static Socket CreateSocket(const Address* address);

        template<typename T>
        inline uint32_t Send(const T* elements, uint32_t count, const Address& dest) {return Send((void*) elements, sizeof(T) * count, dest);}

        template<typename T>
        uint32_t Send(const T& element, const Address& dest) {return Send((void*) &element, sizeof(T), dest);}

        uint32_t Send(const void* buffer, const uint32_t bufferSize, const Address& dest);
        
        uint32_t Receive(void* buffer, const uint32_t bufferSize, Address* address);
        template<typename T>
        uint32_t Receive(T* data, Address* address) {return Receive((void*) data, sizeof(T), address);}

        void Close();

    private:
        void* m_nativeSocket;
    };

    void Initialize();
    void Cleanup();

    Address ResolveHostname(const char* hostname, const uint16_t port);
    void FreeAddress(const Address* address);

}


#endif //NET_H
