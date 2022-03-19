// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2019 Jannik Vogel
// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#include "Error.h"
#include "Common.h"

#include <string.h>

// clear the current error (returns false if error was fatal)
bool Error::ClearError()
{
    if(m_bFatal)
		return false;

    delete[] m_szError;

    m_szError = 0;

    m_bFatal  = false;

    return true;
}

// protected so only derived class may set an error
void Error::SetError(const char *x_szError, bool x_bFatal)
{
    if(m_szError == 0)
    {
        m_szError = new char[ERROR_LEN+1];
        m_szError[ERROR_LEN] = '\0';
    }

    strncpy(m_szError, x_szError, ERROR_LEN);

    m_bFatal = x_bFatal;

    return;
}
