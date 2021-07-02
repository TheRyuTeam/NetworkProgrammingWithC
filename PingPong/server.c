#include "addr_init.h"

//gcc server.c -o server.exe -lws2_32
//gcc .\server.exe

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
    int r = 0;
    if( r = getAddrFromArgs(argc, argv, &addr) ) {
        printf("%d\n", r);
        return -1;
    }
    
    int ADDRLEN = addr.ss_family == AF_INET6 ? INET6_ADDRSTRLEN : INET_ADDRSTRLEN;
    printf("%s\n", inet_ntoa(((LPSOCKADDR_IN)&addr)->sin_addr));
    
    SOCKET s = createSocket(addr.ss_family, SOCK_STREAM, 0);
    if( bind(s, (LPSOCKADDR)&addr, sizeof(addr)) ) {
        fprintf(stderr, "Socket bind error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    if( listen(s, 1) ) {
        fprintf(stderr, "Socket listen error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    char hostname[ADDRLEN];
    inet_ntop(addr.ss_family, voidPtrToSelected(&addr), hostname, ADDRLEN);
    printf("%d\n", GETSOCKETERRNO());
    printf("%s server at %s:%d\n", 
           addr.ss_family == AF_INET6 ? "ipv6" : "ipv4", hostname, htons(((LPSOCKADDR_IN)&addr)->sin_port));
    printf("Waiting for the opponent...\n");
    SOCKADDR_STORAGE client_addr;
    socklen_t client_len = sizeof(client_addr);
    SOCKET client_socket = accept(s, (LPSOCKADDR)&client_addr, &client_len);
    if ( !ISVALIDSOCKET(client_socket) ) {
        fprintf(stderr, "Socket accept error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    
    inet_ntop(client_addr.ss_family, voidPtrToSelected(&client_addr), hostname, sizeof(hostname));
    printf("%s connected\n", hostname);
    char bytes[5] = "Ping\0";
    printf("Me:\t%s\n", bytes);
    if( send(client_socket, bytes, sizeof(bytes), 0) < 1 ) {
        fprintf(stderr, "Socket send error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    if( recv(client_socket, bytes, sizeof(bytes), 0) < 1 ) {
        fprintf(stderr, "Socket send error (%d).\n", GETSOCKETERRNO());
        return 1;
    }
    printf("%s:\t%s\n", hostname, bytes);
    CLOSESOCKET(client_socket);
    CLOSESOCKET(s);
#ifdef __WIN32
    WSACleanup();
#endif
    return EXIT_SUCCESS;
}