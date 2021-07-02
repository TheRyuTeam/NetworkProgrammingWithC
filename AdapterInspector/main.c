#include "../includes.h"

//gcc main.c -o AdaptersInspector.exe -lws2_32 -liphlpapi
//.\AdaptersInspector.exe
#ifdef __WIN32
int main(void)
{
    WSADATA WSA_DATA;
    if( WSAStartup(WSA_VERSION, &WSA_DATA) ) {
        printf("WTF where is my WSA DATA MTHFCKR!\n");
        return -1;
    }
    DWORD asize = 20000;
    PIP_ADAPTER_ADDRESSES adapters;
    do {
        adapters = (PIP_ADAPTER_ADDRESSES)malloc(asize);
        if( !adapters ) {
            printf("Allocation error\n");
            WSACleanup();
            return -2;
        }
        int r = GetAdaptersAddresses(AF_UNSPEC/*AF_INET, AF_INET6*/, GAA_FLAG_INCLUDE_PREFIX, 0, adapters, &asize);
        if( r == ERROR_BUFFER_OVERFLOW ) {
            printf("Overflow\n");
            free(adapters);
        }
        else if( r == ERROR_SUCCESS ) {
            break;
        }
        else {
            printf("Unspec error\n");
            WSACleanup();
            return -3;
        }
    } while( !adapters );
    PIP_ADAPTER_ADDRESSES adapter = adapters;
    while( adapter ) {
        printf("Adapter name %S\n", adapter->FriendlyName);
        PIP_ADAPTER_UNICAST_ADDRESS address = adapter->FirstUnicastAddress;
        while (address) {
            printf( "\t%s",
                    address->Address.lpSockaddr->sa_family == AF_INET ?
                    "IPv4" : "IPv6");
            char ap[100];
            getnameinfo(address->Address.lpSockaddr, address->Address.iSockaddrLength,
                        ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);
            address = address->Next;
        }
        adapter = adapter->Next;
    }
    free(adapters);
    WSACleanup();
    return EXIT_SUCCESS;
}

#else

//gcc main.c -o AdaptersInspector
//.\AdaptersInspector

int main() {
    struct ifaddrs *addresses;
    if (getifaddrs(&addresses) == -1) {
        printf("getifaddrs call failed\n");
        return -1;
    }
    struct ifaddrs *address = addresses;
    while(address) {
        int family = address->ifa_addr->sa_family;
        if (family == AF_INET || family == AF_INET6) {
            printf("%s\t", address->ifa_name);
            printf("%s\t", family == AF_INET ? "IPv4" : "IPv6");
            char ap[100];
            const int family_size = family == AF_INET ?
            sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6);
            getnameinfo(address->ifa_addr,
                        family_size, ap, sizeof(ap), 0, 0, NI_NUMERICHOST);
            printf("\t%s\n", ap);
        }
        address = address->ifa_next;
    }
    freeifaddrs(addresses);
    return 0;
}
#endif
/*
Adapter name Ethernet
        IPv6    fe80::4162:5768:8476:cb20%14
        IPv4    169.254.203.32
Adapter name VirtualBox Host-Only Network   
        IPv6    fe80::101:c44:72a2:dfef%15  
        IPv4    192.168.56.1
Adapter name 
        IPv6    fe80::3d94:65da:2c93:18db%7 
        IPv4    169.254.24.219
Adapter name 
        IPv6    fe80::84dd:9a:60ff:8d59%3   
        IPv4    169.254.141.89
Adapter name
        IPv6    fe80::48c8:628d:9430:4d62%4
        IPv4    192.168.0.101
Adapter name Loopback Pseudo-Interface 1
        IPv6    ::1
        IPv4    127.0.0.1
*/