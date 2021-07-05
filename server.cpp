#include "network.hpp"
#include <iostream>

int main()
{
    using namespace Network::ipv4::tcp;
    Socket<> s( loopback(1234), 1 );
    endpoint caddr;
    auto c = s.accept(caddr);
    std::cout << caddr.to_string() << '\n';
    std::cout << c.send("Hello from server") << '\n';
    std::cout << c.recv_n(1024) << '\n';
    return EXIT_SUCCESS;
}