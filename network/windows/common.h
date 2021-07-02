#pragma once

#ifdef _WIN32

#ifndef WINVER
#define WINVER 0x0A00                   //  for example inet_ntop and inet_pton requires windows version > 0x0600
#endif
#ifndef _WIN32_WINNT
#define _WIN32_WINNT 0x0A00
#endif

#include <winsock2.h>                   //  Base windows network lib
#include <ws2tcpip.h>                   //  tcp/ip lib
#include <iphlpapi.h>               

#pragma comment(lib, "ws2_32.lib")     // In MinGW use -lws2_32
#pragma comment(lib, "iphlpapi.lib")   // In MinGW use -lws2_32

#ifndef WSA_DEFAULT_VER
#define WSA_DEFAULT_VER MAKEWORD(2, 2)
#endif

#define ISVALIDSOCKET(s) ((s) != INVALID_SOCKET)
#define CLOSESOCKET(s) closesocket(s)
#define GETSOCKETERRNO() (WSAGetLastError())

#endif