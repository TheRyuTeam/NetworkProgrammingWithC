#pragma once
#include "../abstract_address.hpp"
#include <string>

namespace Network::ipv6
{   // Network::ipv6

class Address : public Abstract::Address
{
protected:
    sockaddr_in6 addr_ { };
    socklen_t size_ { MAX_SIZE };
    static constexpr socklen_t MAX_SIZE { sizeof(sockaddr_in6) };

public:
    Address() = default;
    Address(Address&& _addr) = default;
    Address(const Address& _addr) = default;
    Address(const sockaddr_in& _addr);
    Address(const std::string& _saddr, port_t _port);

    Address& operator=(Address&& _addr) = default;
    Address& operator=(const Address& _addr) = default;

    socklen_t& size() override { return size_; }
    socklen_t size() const override { return size_; }
    
    sockaddr* sockaddr_ptr() override { return (sockaddr*)&addr_; }
    const sockaddr* sockaddr_ptr() const { return (sockaddr*)&addr_; }

    sockaddr_in6* sockaddr_in6_ptr() { return &addr_; }
    const sockaddr_in6* sockaddr_in6_ptr() const { return &addr_; }

};  // Network::ipv6::Address

};  // Network::ipv6