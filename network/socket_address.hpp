#pragma once
#include "abstract_address.hpp"
#include <stdexcept>
#include <type_traits>

namespace Network
{  // Network

class Address : public Abstract::Address
{
protected:
    sockaddr_storage addr_ { };
    socklen_t size_ { MAX_SIZE };
    static constexpr socklen_t MAX_SIZE { sizeof(sockaddr_storage) };

public:
    Address() = default;
    Address(Address&& _addr) = default;
    Address(const Address& _addr) = default;
    Address(const sockaddr* _addr, socklen_t _size)
    {
        if( !_addr ) 
            throw std::invalid_argument("Null ptr argument.");
        if( size_ < 0 || size_ > MAX_SIZE )
            throw std::invalid_argument("Socket size must be > 0 and <= sockaddr_storage size.");
        memcpy(&addr_, _addr, size_);
    }
    Address(const sockaddr_storage* _addr, socklen_t _size) : Address((sockaddr*)_addr, _size) {}

    Address& operator=(Address&& _addr) = default;
    Address& operator=(const Address& _addr) = default;

    virtual socklen_t& size() override { return size_; }
    virtual socklen_t size() const override { return size_; }
    
    virtual sockaddr* sockaddr_ptr() override { return (sockaddr*)&addr_; }
    virtual const sockaddr* sockaddr_ptr() const { return (sockaddr*)&addr_; }
    
}; // Network::Address

}; // Network