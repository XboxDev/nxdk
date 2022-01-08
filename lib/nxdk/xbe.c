/*
 * Copyright (c) 2022 Stefan Schmidt
 *
 * Licensed under the MIT License
 */

#include "xbe.h"
#include <string.h>
#include <winnt.h>

PXBE_SECTION_HEADER nxXbeGetSectionByName (const char *name)
{
    for (DWORD i = 0; i < CURRENT_XBE_HEADER->NumberOfSections; i++) {
        XBE_SECTION_HEADER *const sectionHeader = &CURRENT_XBE_HEADER->PointerToSectionTable[i];

        if (strcmp(name, sectionHeader->SectionName) == 0) {
            return sectionHeader;
        }
    }

    return NULL;
}
