#pragma once
#include "includes.h"

namespace Network::Template
{

constexpr bool is_ipv4(family_t _family) { return _family == AF_INET; }
constexpr bool is_ipv6(family_t _family) { return _family == AF_INET6; }
constexpr bool is_ip(family_t _family) { return is_ipv4(_family) || is_ipv6(_family); }

template<family_t _Family> struct AddressTypeSelector { using type = sockaddr_storage; };
template<> struct AddressTypeSelector<AF_INET> { using type = sockaddr_in; };
template<> struct AddressTypeSelector<AF_INET6> { using type = sockaddr_in6; };

template<family_t _Family> 
class Address
{   
public:
    using addr_t = typename AddressTypeSelector<_Family>::type;
protected:
    addr_t addr_ { };
    socklen_t size_ { MAX_SIZE };
    static constexpr socklen_t MAX_SIZE { sizeof(addr_t) };
    
public:
    Address() = default;
    Address(Address&& _addr) = default;
    Address(const Address& _addr) = default;
    Address(const addr_t& _addr) : addr_ { _addr } {}
    Address(const sockaddr* _addr, size_t _size)
    {
        static_assert(!is_ip(_Family));
        if( !_addr ) 
            throw std::invalid_argument("Null ptr argument.");
        if( size_ > MAX_SIZE )
            throw std::invalid_argument("Socket size must be less or eq sockaddr_storage size.");
        memcpy(&addr_, _addr, size_);
    }
    Address(const std::string& _saddr, port_t _port)
    {
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has constructor from std::string and port.");
        if constexpr ( is_ipv4(_Family) ) {
            addr_.sin_family = _Family;
            addr_.sin_port = htons(_port);
        }
        else {
            addr_.sin6_family = _Family;
            addr_.sin6_port = htons(_port); 
        }
        if( inet_pton(_Family, _saddr.c_str(), (void*)&addr()) != 1 )
            throw std::invalid_argument("Bad ipv" + is_ipv4(_Family) ? "4." : "6.");
    }

    Address& operator=(Address&& _addr) = default;
    Address& operator=(const Address& _addr) = default;

    socklen_t& size() { return size_; }
    socklen_t size() const { return size_; }
    
    sockaddr* sockaddr_ptr() { return (sockaddr*)&addr_; }
    const sockaddr* sockaddr_ptr() const { return (sockaddr*)&addr_; }

    auto addr() const
    { 
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has addr().");
        if constexpr ( is_ipv4(_Family) )
            return addr_.sin_addr; 
        else
            return addr_.sin6_addr; 
    }

    operator sockaddr*() { return sockaddr_ptr(); }
    operator const sockaddr*() const { return sockaddr_ptr(); }

    family_t family() const 
    {
        auto p = sockaddr_ptr();
        return p ? p->sa_family : AF_UNSPEC;
    }
    auto& addr() 
    { 
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has addr().");
        if constexpr ( is_ipv4(_Family) )
            return addr_.sin_addr; 
        else
            return addr_.sin6_addr; 
    }
    auto port() const 
    { 
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has port().");
        if constexpr ( is_ipv4(_Family) )
            return addr_.sin_port; 
        else
            return addr_.sin6_port; 
    }

    std::string to_string() const 
    {
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has to_string().");
        static const int buf_size = is_ipv4(_Family) ? INET_ADDRSTRLEN : INET6_ADDRSTRLEN;
        static char buf[buf_size] { 0 };
        auto p = addr();
        auto str = inet_ntop(_Family, (void*)&p, buf, buf_size);
        return std::string(str ? buf : "<undefined>") + ":" + std::to_string(htons(port()));
    }
};


namespace ipv4
{
    using endpoint = Network::Template::Address<AF_INET>;
};

namespace ipv6
{
    using endpoint = Network::Template::Address<AF_INET6>;
};

};