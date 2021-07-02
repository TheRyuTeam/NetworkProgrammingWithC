#pragma once

#ifdef _WIN32

#include "common.h"
#include <iostream>
namespace Network
{

class WSA
{
private:
    static WSAData data_;
    static int error_;
    static WSA* ptr_;
    WSA()
    {
        ptr_ = this;
        error_ = ::WSAStartup(WSA_DEFAULT_VER, &data_) ? GETSOCKETERRNO() : 0;
    }
public:
    ~WSA() { WSACleanup(); }
    static int last_error() { return error_; }
    static WSAData data() { return data_; }
    static WSA* init() 
    {
        return ptr_ ? ptr_ : new WSA();
    };
};  //Network::WSA

    WSAData WSA::data_ = { };
    int WSA::error_ = 0;
    WSA* WSA::ptr_ = nullptr;
};  //Network

#endif