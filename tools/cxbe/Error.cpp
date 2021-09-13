// ******************************************************************
// *
// *  This file is part of Cxbe
// *
// *  This program is free software; you can redistribute it and/or
// *  modify it under the terms of the GNU General Public License
// *  as published by the Free Software Foundation; either version 2
// *  of the License, or (at your option) any later version.
// *
// *  This program is distributed in the hope that it will be useful,
// *  but WITHOUT ANY WARRANTY; without even the implied warranty of
// *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// *  GNU General Public License for more details.
// *
// *  You should have received a copy of the GNU General Public License
// *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
// *
// *  (c) 2002-2003 Aaron Robinson <caustik@caustik.com>
// *
// *  All rights reserved
// *
// ******************************************************************
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
