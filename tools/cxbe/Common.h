// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2019 Jannik Vogel

#ifndef COMMON_H
#define COMMON_H

#define OPTION_LEN 266
#define ERROR_LEN 256

struct Option
{
    char *value;
    const char *key;
    const char *desc;
};

int ParseOptions(char *argv[], int argc, const Option *options, char *szErrorMessage);
void ShowUsage(const char *program, const char *desc, const Option *options);
int GenerateFilename(char *szNewPath, const char *szNewExtension, const char *szOldPath,
                     const char *szOldExtension);
bool CompareString(const char *szA, const char *szB);

#endif
