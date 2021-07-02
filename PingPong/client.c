#include "addr_init.h"

//gcc client.c -o client.exe -lws2_32
//gcc .\client.exe

int main(int argc, char** argv)
{
#ifdef __WIN32
    WSADATA WSA_DATA;
    if( WSAStartup(WSA_VERSION, &WSA_DATA) ) {
        fprintf(stderr, "WTF where is my WSA DATA MTHFCKR!\n");
        return 1;
    }
#endif
    SOCKADDR_STORAGE addr;
    if( getAddrFromArgs(argc, argv, &addr) ) {
        return -1;
    }
    SOCKET s = createSocket(addr.ss_family, SOCK_STREAM, 0);
    if ( connect(s, (LPSOCKADDR)&addr, sizeof(addr)) ) {
        fprintf(stderr, "Socket connect error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    char hostname[47];
    inet_ntop(addr.ss_family, voidPtrToSelected(&addr), hostname, sizeof(hostname));
    printf("Connected to %s\n", hostname);
    char bytes[5];
    memset(bytes, 0, sizeof(bytes));
    if( recv(s, bytes, sizeof(bytes), 0) < 1 ) {
        fprintf(stderr, "Socket send error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    printf("%s:\t%s\n", hostname, bytes);
    memcpy(bytes, "Pong\0", 5);
    printf("Me:\t%s\n", bytes);
    if( send(s, bytes, sizeof(bytes), 0) < 1 ) {
        fprintf(stderr, "Socket send error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    CLOSESOCKET(s);
#ifdef __WIN32
    WSACleanup();
#endif
    return EXIT_SUCCESS;
}