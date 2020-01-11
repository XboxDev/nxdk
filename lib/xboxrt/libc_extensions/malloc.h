#ifndef _XBOXRT_MALLOC_H_
#define _XBOXRT_MALLOC_H_

// Some code expects to find a declaration of malloc here, so we simply include the whole C header
#include <stdlib.h>

// Forward alloca to the builtin with the Windows-specific name
#define alloca(size) _alloca(size)

#endif
