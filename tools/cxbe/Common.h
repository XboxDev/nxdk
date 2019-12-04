// Licensed under GPLv2 or (at your option) any later version.
// Copyright (C) 2019 Jannik Vogel
// Copyright (C) 2002-2003 Aaron Robinson <caustik@caustik.com>

#ifndef COMMON_H
#define COMMON_H

#define OPTION_LEN 266
#define ERROR_LEN 256

struct Option {
    char *value;
    const char *key;
};

int ParseOptions(char *argv[], int argc,
                 const Option *options, char *szErrorMessage);
bool CompareString(const char *szA, const char *szB);

#endif
