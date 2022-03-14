// SPDX-License-Identifier: MIT

// SPDX-FileCopyrightText: 2004 Craig Edwards
// SPDX-FileCopyrightText: 2005 Robin Mulloy
// SPDX-FileCopyrightText: 2005 Tom Burns
// SPDX-FileCopyrightText: 2019-2021 Stefan Schmidt
// SPDX-FileCopyrightText: 2022 Erik Abair

#ifndef HAL_XBOX_H
#define HAL_XBOX_H


#if defined(__cplusplus)
extern "C"
{
#endif

void XReboot(void);

/**
 * Retrieves information persisted by the process that launched the current XBE.
 * 
 * launchDataType will (likely) be one of the LDT_* defines in xboxkrnl.h
 *
 * Returns non-zero in the case of failure.
 */
int XGetLaunchInfo(unsigned long *launchDataType, const unsigned char **launchData);

/**
 * Launches an XBE.  Examples of xbePath might be:
 *   c:\\blah.xbe
 *   c:/foo/bar.xbe
 * If the XBE is able to be launched, this method will
 * not return.  If there is a problem, then it return.
 */
void XLaunchXBE(const char *xbePath);

/**
 * Launches an XBE and sets the LAUNCH_DATA_PAGE's LaunchData, which is
 * retrievable by the newly launched process.
 *
 * Examples of xbePath might be:
 *   c:\\blah.xbe
 *   c:/foo/bar.xbe
 * If the XBE is able to be launched, this method will
 * not return.  If there is a problem, then it return.
 */
void XLaunchXBEEx(const char *xbePath, const void *launchData);

#ifdef __cplusplus
}
#endif

#endif
