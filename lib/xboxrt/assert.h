#ifndef __ASSERT_H
#define __ASSERT_H

#ifndef NDEBUG
#include "debug.h"
#define assert(x) if (!(x)) { debugPrint("\nAssertion failed! '%s' in %s:%d\n\n", #x, __FILE__, __LINE__); }
#else
#define assert(x)
#endif

#endif
