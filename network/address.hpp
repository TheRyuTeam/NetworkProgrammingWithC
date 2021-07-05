#pragma once
#include "abstract_address.hpp"
#include <stdexcept>
#ifndef DEFAULT_PORT
#define DEFAULT_PORT 9595
#endif

namespace Network
{


constexpr bool is_ipv4(family_t _af) { return _af == AF_INET; }
constexpr bool is_ipv6(family_t _af) { return _af == AF_INET6; }
constexpr bool is_ip(family_t _af)   { return is_ipv4(_af) || is_ipv6(_af); }

/// AddressTypeSelector
/**
 * Used for defining address type for specified address family
 * sockaddr_in for ipv4 addresses
 * sockaddr_in6 for ipv6 addresses
 * sockaddr_storage for other families
 * @tparam _AF address family
 * Using:
 * @code 
 * AddressTypeSelector< FAMILY >::type SOME_VARIABLE...;
 * or
 * using SOME_TYPE = typename AddressTypeSelector< FAMILY >::type;
 * @code
 */
template<family_t _AF> struct AddressTypeSelector { using type = sockaddr_storage; };
template<> struct AddressTypeSelector<AF_INET> { using type = sockaddr_in; };
template<> struct AddressTypeSelector<AF_INET6> { using type = sockaddr_in6; };

/// Address
/**
 * @tparam _Family inet family
 * Address store socket address in addr_t storage, defined in AddressTypeSelector,
 *  address length and maximal address length for selected addr_t
 * @see AddressTypeSelector
 * Default constructable, copyable, movable
 */
template<family_t _Family> 
class Address : public Network::Abstract::Address
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
    /// Construct address from valid sockaddr* with specified size
    /**
     * @param _addr sockaddr pointer 
     * @param _size structure size that must be less then addr_t size
     * @throws std::invalid_argument if _addr is nullptr or _size > MAX_SIZE
     */
    Address(const sockaddr* _addr, size_t _size) : size_ { _size }
    {
        if( !_addr ) 
            throw std::invalid_argument("Null ptr argument.");
        if( size_ > MAX_SIZE )
            throw std::invalid_argument("Socket size must be less or eq sockaddr_storage size.");
        memcpy(&addr_, _addr, size_);
    }
    /// Address ip constructor
    /**
     * Defined only for ip addresses
     * Construct Address with string ip and port
     * @param _saddr string ip address
     * @param _port host port
     * @throws std::invalid_adrgument if _saddr isn't valid
    */
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

    operator sockaddr*() { return sockaddr_ptr(); }
    operator const sockaddr*() const { return sockaddr_ptr(); }

    /// addr()
    /**
     * Defined only for ip addresses
     * @returns in_addr if address family is ipv4
     * @returns in6_addr if address family is ipv6
    */
    auto addr() const
    { 
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has addr().");
        if constexpr ( is_ipv4(_Family) )
            return addr_.sin_addr; 
        else
            return addr_.sin6_addr; 
    }
    /// addr()
    /**
     * Defined only for ip addresses
     * @returns in_addr& if address family is ipv4
     * @returns in6_addr& if address family is ipv6
    */
    auto& addr() 
    { 
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has addr().");
        if constexpr ( is_ipv4(_Family) )
            return addr_.sin_addr; 
        else
            return addr_.sin6_addr; 
    }
    /// port()
    /**
     * Defined only for ip addresses
     * @returns address port
    */
    auto port() const 
    { 
        static_assert(is_ip(_Family), "Only ipv4 and ipv6 has port().");
        if constexpr ( is_ipv4(_Family) )
            return addr_.sin_port; 
        else
            return addr_.sin6_port; 
    }

    /// to_string()
    /**
     * Defined only for ip addresses
     * @returns address string representation ( format <address>:<port> )
    */
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

namespace tcp { template<family_t _AF> using endpoint = Network::Address<_AF>; }
namespace udp { template<family_t _AF> using endpoint = Network::Address<_AF>; }

namespace ipv4
{
    using endpoint = Network::Address<AF_INET>;
    endpoint loopback(port_t _port = DEFAULT_PORT) { return { "127.0.0.1", _port }; }
    namespace tcp { using endpoint = ipv4::endpoint; auto loopback = ipv4::loopback; }
    namespace udp { using endpoint = ipv4::endpoint; auto loopback = ipv4::loopback; }
}

namespace ipv6
{
    using endpoint = Network::Address<AF_INET6>;
    endpoint loopback(port_t _port = DEFAULT_PORT) { return { "::1", _port }; }
    namespace tcp { using endpoint = ipv6::endpoint; auto loopback = ipv6::loopback; }
    namespace udp { using endpoint = ipv6::endpoint; auto loopback = ipv6::loopback; }
}

};  //Network