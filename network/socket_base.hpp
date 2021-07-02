#pragma once
#include "abstract_socket.hpp"

namespace Network
{   // Network

class Socket : public Abstract::Socket_common
{
protected:
    SOCKET s_ { INVALID_SOCKET };

public:
    Socket() = default;
    Socket(Socket&& _socket) = default;
    Socket(const Socket& _socket) = delete;
    Socket(SOCKET _s) : s_ { _s } {}
    Socket(family_t _af, int _type, int _protocol = 0)  { create(_af, _type, _protocol); }
    
    Socket& operator=(Socket&& _socket) = default;
    Socket& operator=(const Socket& _socket) = delete;
    
    SOCKET socket() const override { return s_; }

    template <typename _Address_type>
    bool bind(const _Address_type& _addr) const { return ::bind(socket(), _addr, _addr.size()) == 0; }

    bool listen(int _n = 1) const { return ::listen(socket(), _n) == 0; }
    
    Socket accept() const { return { ::accept(socket(), nullptr, nullptr)}; }
    template <typename _Address_type>
    Socket accept(_Address_type& _addr) const { return { ::accept(socket(), _addr, &_addr.size()) }; }

    template <typename _Address_type>
    bool connect(const _Address_type& _addr) const { return ::connect(socket(), _addr, _addr.size()) == 0; }

    virtual void create(family_t _af, int _type, int _protocol = 0)
    {
        if( is_valid() )
            close();
        s_ = ::socket(_af, _type, _protocol);    
    }
};  //Network::Socket



};  //Network