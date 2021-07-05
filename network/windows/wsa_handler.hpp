#pragma once
#ifdef _WIN32

#include "common.h"

namespace Network
{
/// Windows Socket Api Handler
/**
 * Singletone
 * Handle WSAData
 */
class WSA
{
private:
    static WSAData data_;
    static WSA* ptr_;
    WSA()
    {
        ptr_ = this;
        ::WSAStartup(WSA_DEFAULT_VER, &data_);
    }
public:
    ~WSA() { WSACleanup(); }
    /// init
    /**
     * @returns pointer to existing WSA if WSA init-ed
     * @returns pointer to new WSA if WSA didn't init-ed
     */
    static WSA* init() 
    {
        return ptr_ ? ptr_ : new WSA();
    };
    static WSAData data() { return data_; }
    
};  //Network::WSA

    WSAData WSA::data_ = { };
    WSA* WSA::ptr_ = nullptr;
};  //Network

#endif