#include "network.hpp"
#include <iostream>
#include <fstream>
#include <streambuf>

int main()
{
    std::string HTTP_HEADER = "HTTP/1.0 200 OK\r\n"
             "Content-length: %d\r\n"
             "Content-Type: text/html\r\n"
             "\r\n";
    std::ifstream input("../index.html");
    std::string body( (std::istreambuf_iterator<char>(input)), std::istreambuf_iterator<char>() );
    std::string html = std::move(HTTP_HEADER);
    html += body;
    using namespace Network::ipv4::tcp;
    Socket<> s(endpoint{ "127.0.0.1", 5695 }, 1);

    while( true ) {
        endpoint caddr;
        auto client = s.accept(caddr);
        if( client.last_error() != 0 ) {
            std::cout << client.last_error() << '\n';
        }
        client.recv_n(10240);
        client.send(html);
    }
}