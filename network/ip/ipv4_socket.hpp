#pragma once
#include "../socket_base.hpp"
#include <stdexcept>

namespace Network::ipv4
{   //Network::ipv4
    
class Socket : public Network::Socket
{
private:
    using base = Network::Socket;
    Socket(SOCKET _s) : base(_s) {}
public:
    static constexpr family_t Family = AF_INET;

    Socket(Socket&& _socket) = default;
    Socket(const Socket& _socket) = delete;
    Socket(int _type, int _protocol = 0)  { create(_type, _protocol); }
    
    Socket& operator=(Socket&& _socket) = default;
    Socket& operator=(const Socket& _socket) = delete;

    void create(family_t _af, int _type, int _protocol = 0) override
    {
        if( _af != Family)
            throw std::invalid_argument("Invalid Family : must be ipv4.");
        base::create(Family, _type, _protocol);
    }
    void create(int _type, int _protocol = 0) { base::create(Family, _type, _protocol); }

    Socket accept() const { return { ::accept(socket(), nullptr, nullptr)}; }
    template <typename _Address_type>
    Socket accept(_Address_type& _addr) const { return { ::accept(socket(), _addr, &_addr.size()) }; }
};  // Network::ipv4::Socket

};  // Network::ipv4