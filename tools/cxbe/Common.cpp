// Licensed under GPLv2 or (at your option) any later version.
// Copyright (C) 2019 Jannik Vogel
// Copyright (C) 2002-2003 Aaron Robinson <caustik@caustik.com>

#include "Common.h"

#include <ctype.h>

// case-insensitive string compare
bool CompareString(const char *szA, const char *szB)
{
    while(*szA != '\0' && *szB != '\0')
        if(toupper(*szA++) != toupper(*szB++))
            return false;
    return *szA == *szB;
}
