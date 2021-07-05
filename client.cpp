#include "network.hpp"
#include <iostream>

int main()
{
    using namespace Network::ipv4::tcp;
    Socket<> s( loopback(1234) );
    std::cout << s.recv_n(1024) << '\n';
    std::cout << s.send("Hello from client!") << '\n';
    return EXIT_SUCCESS;
}