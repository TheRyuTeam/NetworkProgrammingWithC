#include "network/ip/ipv4_socket.hpp"
#include "network/ip/ipv4_address.hpp"
#include "network/template_address.hpp"
#include "network/template_socket.hpp"
#include <iostream>

int main()
{
    using namespace Network::Template::ipv4;
    tcp::Socket<> s(endpoint{ "127.0.0.1", 1234 });
    std::cout << s.recv_n(1024) << '\n';
    std::cout << s.send("Hello from client!") << '\n';
    return EXIT_SUCCESS;
}