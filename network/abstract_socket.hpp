#include "includes.h"
#include <string>

namespace Network::Abstract
{   // Network::Abstract

struct Socket_common
{
    virtual ~Socket_common() { close(); }
    virtual SOCKET socket() const = 0;

    virtual bool is_valid() const { return ISVALIDSOCKET(socket()); }
    virtual bool is_invalid() const { return !is_valid(); }
    virtual void close() const { CLOSESOCKET(socket()); }

    virtual int send_n(const char* _msg, size_t _n, int _flags = 0) const { return ::send(socket(), _msg, _n, _flags); }
    virtual bool send(const char* _msg, size_t _n, int _flags = 0) const { return send_n(_msg, _n, _flags) > 0; }
    virtual bool send(const std::string& _msg, int _flags = 0) const { return send_n(_msg.c_str(), _msg.size(), _flags) > 0; }

    virtual int recv_n(char* _buf, size_t _n, int _flags = 0) const { return ::recv(socket(), _buf, _n, _flags); }
    virtual bool recv(char* _buf, size_t _n, int _flags = 0) const { return recv_n(_buf, _n, _flags) > 0; }
    virtual std::string recv_n(size_t _n, int _flags = 0) const
    {
        auto buf = new char[_n];
        if( !buf )
            throw std::bad_alloc();
        recv_n(buf, _n, _flags);
        std::string msg(buf);
        delete [] buf;
        return msg;
    }

};  // Network::Abstract::Socket

};  // Network::Abstract