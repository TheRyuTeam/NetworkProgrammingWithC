#include "includes.h"
#include <string>

namespace Network::Abstract
{   // Network::Abstract

struct Socket
{

    virtual ~Socket() { }    // HERE is trouble with close ?????
    virtual SOCKET socket() const = 0;

    virtual bool is_valid() const { return ISVALIDSOCKET(socket()); }
    virtual bool is_invalid() const { return !is_valid(); }
    virtual void close() const
    {
        if( is_valid() )
            CLOSESOCKET(socket());
    }

};  // Network::Abstract::Socket

};  // Network::Abstract