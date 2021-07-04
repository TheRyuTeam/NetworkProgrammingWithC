#pragma once
#include "includes.h"

namespace Network::Abstract
{  // Network::Abstract

/// Abstract Socket Address
/**
 */
struct Address
{
    virtual socklen_t& size() = 0;
    virtual socklen_t size() const = 0;
    
    virtual sockaddr* sockaddr_ptr() = 0;
    virtual const sockaddr* sockaddr_ptr() const = 0;

    virtual operator sockaddr*() { return sockaddr_ptr(); }
    virtual operator const sockaddr*() const { return sockaddr_ptr(); }

    virtual family_t family() const 
    {
        auto p = sockaddr_ptr();
        return p ? p->sa_family : AF_UNSPEC;
    }
}; // Network::Abstract::Address

}; // Network::Abstract