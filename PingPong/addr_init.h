#include "../includes.h"
#define DEFAULT_PORT 1234

PVOID voidPtrToSelected(SOCKADDR_STORAGE* storage)
{
    if( !storage ) {
        return NULL;
    }
    if( storage->ss_family == AF_INET6 ) {
        return (PVOID)&((LPSOCKADDR_IN6)storage)->sin6_addr; 
    }
    return (PVOID)&((LPSOCKADDR_IN)storage)->sin_addr;
}
int getAddrFromArgs(int argc, char** argv, SOCKADDR_STORAGE* addr)
{
    if( !addr ) {
        return -1;
    }
    memset(addr, 0, sizeof(*addr));
    addr->ss_family = AF_INET;
    LPSOCKADDR_IN ptr = (LPSOCKADDR_IN)addr; 
    ptr->sin_port = htons(DEFAULT_PORT);
    ptr->sin_addr.s_addr = 16777343;    //  htonl(INADDR_LOOPBACK)
    if( argc == 4 ) {
        addr->ss_family = atoi(argv[1]);
        int res = inet_pton(addr->ss_family, argv[2], voidPtrToSelected(addr));
        if( res != 1) {
            return -3;
        }
        ptr->sin_port = htons(atoi(argv[3]));
        return 0;
    }
    return argc == 1 ? 0 : -2;
}

SOCKET createSocket(int af, int type, int protocol)
{
    SOCKET s = socket(af, type, protocol);
    if( !ISVALIDSOCKET(s) ) {
        fprintf(stderr, "Socket create error (%d).\n", GETSOCKETERRNO());
        exit(1);
    }
    return s;
}