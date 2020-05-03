#ifndef XBOXRT_CRTDBG
#define XBOXRT_CRTDBG

#include <assert.h>

// Part of Microsoft CRT
#define _ASSERT_EXPR(booleanExpression, message) assert(booleanExpression)
#define _ASSERT(booleanExpression) assert(booleanExpression)
#define _ASSERTE(booleanExpression) assert(booleanExpression)

#endif
