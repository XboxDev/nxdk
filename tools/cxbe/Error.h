// This file is part of Cxbe
// SPDX-License-Identifier: GPL-2.0-or-later

// SPDX-FileCopyrightText: 2002-2003 Aaron Robinson <caustik@caustik.com>
// SPDX-FileCopyrightText: 2021 Stefan Schmidt

#ifndef ERROR_H
#define ERROR_H

#include "Cxbx.h"

// inherit from this class for handy error reporting capability
class Error
{
  public:
    // return current error (zero if there is none)
    const char *GetError() const
    {
        return m_szError;
    }

    // is the current error fatal? (class is "dead" on fatal errors)
    bool IsFatal() const
    {
        return m_bFatal;
    }

    // clear the current error (returns false if error was fatal)
    bool ClearError();

  protected:
    // protected constructor so this class must be inherited from
    Error() : m_szError(0), m_bFatal(false)
    {
    }

    // protected deconstructor
    ~Error()
    {
        delete[] m_szError;
    }

    // protected so only derived class may set an error
    void SetError(const char *x_szError, bool x_bFatal);

  private:
    // current error information
    bool m_bFatal;
    char *m_szError;
};

#endif
