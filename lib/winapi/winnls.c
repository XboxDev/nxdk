// SPDX-License-Identifier: MIT
// UTF-16 <-> UTF-8 (CP_UTF8 65001).

#include <stdint.h>
#include <string.h>
#include <winbase.h>
#include <winnls.h>
#include <winerror.h>

static int IsUtf8CodePage (UINT page)
{
  return page == CP_UTF8;
}

static int Utf16LenIncludingNul (const uint16_t *src)
{
  int n = 0;
  while (src[n] != 0)
    n++;
  return n + 1;
}

static int Utf16ToUtf8Size (const uint16_t *src, int n)
{
  int bytes = 0;
  int i = 0;
  while (i < n)
  {
    uint32_t cp = src[i++];
    if (cp >= 0xD800 && cp <= 0xDBFF && i < n)
    {
      uint16_t lo = src[i];
      if (lo >= 0xDC00 && lo <= 0xDFFF)
      {
        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
        i++;
      }
    }
    if (cp < 0x80)
      bytes += 1;
    else if (cp < 0x800)
      bytes += 2;
    else if (cp < 0x10000)
      bytes += 3;
    else
      bytes += 4;
  }
  return bytes;
}

static int Utf16ToUtf8 (const uint16_t *src, int n, char *dst, int dstlen)
{
  int o = 0;
  int i = 0;
  while (i < n)
  {
    uint32_t cp = src[i++];
    if (cp >= 0xD800 && cp <= 0xDBFF && i < n)
    {
      uint16_t lo = src[i];
      if (lo >= 0xDC00 && lo <= 0xDFFF)
      {
        cp = 0x10000 + ((cp - 0xD800) << 10) + (lo - 0xDC00);
        i++;
      }
    }

    unsigned char tmp[4];
    int need = 0;
    int k;
    if (cp < 0x80)
    {
      tmp[0] = (unsigned char)cp;
      need = 1;
    }
    else if (cp < 0x800)
    {
      tmp[0] = (unsigned char)(0xC0 | (cp >> 6));
      tmp[1] = (unsigned char)(0x80 | (cp & 0x3F));
      need = 2;
    }
    else if (cp < 0x10000)
    {
      tmp[0] = (unsigned char)(0xE0 | (cp >> 12));
      tmp[1] = (unsigned char)(0x80 | ((cp >> 6) & 0x3F));
      tmp[2] = (unsigned char)(0x80 | (cp & 0x3F));
      need = 3;
    }
    else
    {
      tmp[0] = (unsigned char)(0xF0 | (cp >> 18));
      tmp[1] = (unsigned char)(0x80 | ((cp >> 12) & 0x3F));
      tmp[2] = (unsigned char)(0x80 | ((cp >> 6) & 0x3F));
      tmp[3] = (unsigned char)(0x80 | (cp & 0x3F));
      need = 4;
    }

    if (o + need > dstlen)
    {
      SetLastError (ERROR_INSUFFICIENT_BUFFER);
      return 0;
    }
    for (k = 0; k < need; k++)
      dst[o++] = (char)tmp[k];
  }
  return o;
}

static int Utf8ToUtf16Size (const unsigned char *src, int n)
{
  int units = 0;
  int i = 0;
  while (i < n)
  {
    unsigned char c = src[i];
    uint32_t cp;
    int adv = 1;
    if (c < 0x80)
      cp = c;
    else if ((c & 0xE0) == 0xC0 && i + 1 < n)
    {
      cp = ((c & 0x1F) << 6) | (src[i + 1] & 0x3F);
      adv = 2;
    }
    else if ((c & 0xF0) == 0xE0 && i + 2 < n)
    {
      cp = ((c & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
      adv = 3;
    }
    else if ((c & 0xF8) == 0xF0 && i + 3 < n)
    {
      cp = ((c & 0x07) << 18) | ((src[i + 1] & 0x3F) << 12) | ((src[i + 2] & 0x3F) << 6) |
           (src[i + 3] & 0x3F);
      adv = 4;
    }
    else
      cp = 0xFFFD;
    i += adv;
    units += (cp > 0xFFFF) ? 2 : 1;
  }
  return units;
}

static int Utf8ToUtf16 (const unsigned char *src, int n, uint16_t *dst, int dstlen)
{
  int o = 0;
  int i = 0;
  while (i < n)
  {
    unsigned char c = src[i];
    uint32_t cp;
    int adv = 1;
    if (c < 0x80)
      cp = c;
    else if ((c & 0xE0) == 0xC0 && i + 1 < n)
    {
      cp = ((c & 0x1F) << 6) | (src[i + 1] & 0x3F);
      adv = 2;
    }
    else if ((c & 0xF0) == 0xE0 && i + 2 < n)
    {
      cp = ((c & 0x0F) << 12) | ((src[i + 1] & 0x3F) << 6) | (src[i + 2] & 0x3F);
      adv = 3;
    }
    else if ((c & 0xF8) == 0xF0 && i + 3 < n)
    {
      cp = ((c & 0x07) << 18) | ((src[i + 1] & 0x3F) << 12) | ((src[i + 2] & 0x3F) << 6) |
           (src[i + 3] & 0x3F);
      adv = 4;
    }
    else
      cp = 0xFFFD;
    i += adv;

    if (cp > 0xFFFF)
    {
      if (o + 2 > dstlen)
      {
        SetLastError (ERROR_INSUFFICIENT_BUFFER);
        return 0;
      }
      cp -= 0x10000;
      dst[o++] = (uint16_t)(0xD800 + (cp >> 10));
      dst[o++] = (uint16_t)(0xDC00 + (cp & 0x3FF));
    }
    else
    {
      if (o + 1 > dstlen)
      {
        SetLastError (ERROR_INSUFFICIENT_BUFFER);
        return 0;
      }
      dst[o++] = (uint16_t)cp;
    }
  }
  return o;
}

int WideCharToMultiByte (UINT CodePage, DWORD dwFlags, const WCHAR *lpWideCharStr, int cchWideChar,
                         char *lpMultiByteStr, int cbMultiByte, const char *lpDefaultChar, PBOOL lpUsedDefaultChar)
{
  const uint16_t *in;

  (void)dwFlags;
  (void)lpDefaultChar;

  if (!IsUtf8CodePage (CodePage))
    return 0;

  if (!lpWideCharStr || cchWideChar == 0 || (!lpMultiByteStr && cbMultiByte))
  {
    SetLastError (ERROR_INVALID_PARAMETER);
    return 0;
  }

  in = (const uint16_t *)lpWideCharStr;
  if (cchWideChar < 0)
    cchWideChar = Utf16LenIncludingNul (in);
  if (!cbMultiByte)
    return Utf16ToUtf8Size (in, cchWideChar);

  if (lpUsedDefaultChar)
    *lpUsedDefaultChar = 0;

  return Utf16ToUtf8 (in, cchWideChar, lpMultiByteStr, cbMultiByte);
}

int MultiByteToWideChar (UINT CodePage, DWORD dwFlags, const char *lpMultiByteStr, int cbMultiByte,
                         WCHAR *lpWideCharStr, int cchWideChar)
{
  const unsigned char *in;

  (void)dwFlags;

  if (!IsUtf8CodePage (CodePage))
    return 0;

  if (!lpMultiByteStr || cbMultiByte == 0 || (!lpWideCharStr && cchWideChar))
  {
    SetLastError (ERROR_INVALID_PARAMETER);
    return 0;
  }

  if (cbMultiByte < 0)
    cbMultiByte = (int)strlen (lpMultiByteStr) + 1;

  in = (const unsigned char *)lpMultiByteStr;
  if (!cchWideChar)
    return Utf8ToUtf16Size (in, cbMultiByte);

  return Utf8ToUtf16 (in, cbMultiByte, (uint16_t *)lpWideCharStr, cchWideChar);
}
