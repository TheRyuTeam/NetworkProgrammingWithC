#pragma once

#ifdef _WIN32

#include "windows/wsa_handler.hpp"

#else

#include "unix/common.h"

#endif

#include <stdlib.h>                     //  I/O
#include <stdio.h>

typedef short family_t;
typedef u_short port_t;
