#include "network/ip/ipv4_socket.hpp"
#include "network/ip/ipv4_address.hpp"
#include "network/template_address.hpp"
#include "network/template_socket.hpp"
#include <iostream>

int main()
{
    using namespace Network::Template::ipv4;
    tcp::Socket<> s(endpoint{ "127.0.0.1", 1234 }, 1);
    endpoint caddr;
    auto c = s.accept(caddr);
    std::cout << caddr.to_string() << '\n'; 
    std::cout << c.send("Hello from server") << '\n';
    std::cout << c.recv_n(1024) << '\n';
    return EXIT_SUCCESS;
}