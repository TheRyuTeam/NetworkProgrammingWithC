#pragma once
#include "includes.h"
#include <memory>

namespace Network::Template
{

template<family_t _Family, int _Type, int _Protocol = 0>
class Socket
{
protected:
#ifdef _WIN32
    const std::shared_ptr<WSA> wsa_ { WSA::init() };
#endif
    SOCKET s_ { INVALID_SOCKET };
    int mutable error_ { };
    Socket(SOCKET _s) : s_ { _s } 
    {
        if( !is_valid() )
            set_error();
    }

    void set_error() const { error_ = last_error(); }

    bool errnz(int _v) const
    {
        if( _v != 0 ) {
            set_error();
            return false;
        }
        return true;
    }

public:

    Socket() 
    {
        s_ = ::socket(_Family, _Type, _Protocol);
        if( !is_valid() )
            set_error();
    }
    template<typename _Addr_t>
    Socket(const _Addr_t& _addr) : Socket()
    {
        if( !connect(_addr) )
            set_error();   
    }
    template<typename _Addr_t>
    Socket(const _Addr_t& _addr, int _backlog) : Socket()
    {
        if( !bind(_addr) ){
            set_error();
            return;
        }
        if( !listen(_backlog) ) {
            set_error();
            return;
        }
    }
    virtual ~Socket() { close(); }

    SOCKET socket() const { return s_; }

    template<typename _Addr_t>
    bool bind(const _Addr_t& _addr) const { return errnz(::bind(socket(), _addr, _addr.size())); }
    bool listen(int _backlog = 1) const { return errnz(::listen(socket(), _backlog)); }

    Socket accept() const { return ::accept(socket(), nullptr, nullptr); }
    template<typename _Addr_t>
    Socket accept(_Addr_t& _addr) const { return ::accept(socket(), _addr, &_addr.size()); }

    template<typename _Addr_t>
    bool connect(const _Addr_t& _addr) const { return errnz(::connect(socket(), _addr, _addr.size())); }

    bool is_valid() const { return ISVALIDSOCKET(socket()); }
    bool is_invalid() const { return !is_valid(); }
    void close() const { CLOSESOCKET(socket()); }
    int last_error() const { return GETSOCKETERRNO(); }

    bool set_options(int _level, int _optname, const std::string& _optval)
    {
        return errnz(::setsockopt(socket(), _level, _optname, _optval.c_str(), _optval.size()));
    }
    bool get_options(int _level, int _optname, char* _optval, int* _optlen)
    {
        return errnz(::getsockopt(socket(), _level, _optname, _optval, _optlen));
    }

    int send_n(const char* _msg, size_t _n, int _flags = 0) const 
    { 
        int ret = ::send(socket(), _msg, _n, _flags);
        if( ret <= 0 )
            set_error();
        return ret; 
    }
    bool send(const char* _msg, size_t _n, int _flags = 0) const { return send_n(_msg, _n, _flags) > 0; }
    bool send(const std::string& _msg, int _flags = 0) const { return send(_msg.c_str(), _msg.size(), _flags); }

    int recv_n(char* _buf, size_t _n, int _flags = 0) const 
    { 
        int ret = ::recv(socket(), _buf, _n, _flags);
        if( ret <= 0 )
            set_error();
        return ret;
    }
    std::string recv_n(size_t _n, int _flags = 0) const
    {
        auto buf = new char[_n];
        if( !buf )
            throw std::bad_alloc();
        if( recv_n(buf, _n, _flags) <= 0)
            return "";
        std::string msg(buf);
        delete [] buf;
        return msg;
    }

};  //Network::Template::Socket


namespace tcp
{
    template<family_t _Family, int _Protocol = 0>
    using Socket = Network::Template::Socket<_Family, SOCK_STREAM, _Protocol>;
};  //Network::Template::tcp

namespace udp
{
    template<family_t _Family, int _Protocol = 0>
    using Socket = Network::Template::Socket<_Family, SOCK_DGRAM, _Protocol>;
};  //Network::Template::udp

namespace ipv4
{
    template<int _Type, int _Protocol = 0>
    using Socket = Network::Template::Socket<AF_INET, _Type, _Protocol>;

    namespace tcp 
    {
        template<int _Protocol = 0>
        using Socket = Network::Template::tcp::Socket<AF_INET, _Protocol>;
    };  //Network::Template::ipv4::tcp
    namespace udp 
    {
        template<int _Protocol = 0>
        using Socket = Network::Template::udp::Socket<AF_INET, _Protocol>;
    };  //Network::Template::ipv4::udp
};  //Network::Template::ipv4

namespace ipv6
{
    template<int _Type, int _Protocol = 0>
    using Socket = Network::Template::Socket<AF_INET6, _Type, _Protocol>;

    namespace tcp 
    {
        template<int _Protocol = 0>
        using Socket = Network::Template::tcp::Socket<AF_INET6, _Protocol>;
    };  //Network::Template::ipv6::tcp
    namespace udp 
    {
        template<int _Protocol = 0>
        using Socket = Network::Template::udp::Socket<AF_INET6, _Protocol>;
    }; //Network::Template::ipv6::udp
};  //Network::Template::ipv6

};  //Netowrk::Template