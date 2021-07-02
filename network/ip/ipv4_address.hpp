#pragma once
#include "../abstract_address.hpp"
#include <string>
#include <stdexcept>

namespace Network::ipv4
{   // Network::ipv4

static constexpr family_t Family = AF_INET;

class Address : public Abstract::Address
{
protected:
    sockaddr_in addr_ { };
    socklen_t size_ { MAX_SIZE };
    static constexpr socklen_t MAX_SIZE { sizeof(sockaddr_in) };

public:
    Address() = default;
    Address(Address&& _addr) = default;
    Address(const Address& _addr) = default;
    Address(const sockaddr_in& _addr);
    Address(const std::string& _saddr, port_t _port)
    {
        addr_.sin_family = Family;
        if( inet_pton(Family, _saddr.c_str(), (void*)&addr_.sin_addr) != 1 )
            throw std::invalid_argument("Bad ipv4.");
        addr_.sin_port = htons(_port);
    }

    Address& operator=(Address&& _addr) = default;
    Address& operator=(const Address& _addr) = default;

    socklen_t& size() override { return size_; }
    socklen_t size() const override { return size_; }
    
    sockaddr* sockaddr_ptr() override { return (sockaddr*)&addr_; }
    const sockaddr* sockaddr_ptr() const { return (sockaddr*)&addr_; }

    sockaddr_in* sockaddr_in_ptr() { return &addr_; }
    const sockaddr_in* sockaddr_in_ptr() const { return &addr_; }

    in_addr sin_addr() const { return addr_.sin_addr; }
    port_t sin_port() const { return addr_.sin_port; }

    std::string to_string() const
    {
        return std::string(inet_ntoa(sin_addr())) + ":" + std::to_string(htons(sin_port()));
    }
    
};  // Network::ipv4::Address

};  // Network::ipv4