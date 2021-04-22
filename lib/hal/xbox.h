#ifndef HAL_XBOX_H
#define HAL_XBOX_H


#if defined(__cplusplus)
extern "C"
{
#endif

void XReboot(void);

void XLaunchXBE(char *xbePath);

#ifdef __cplusplus
}
#endif

#endif
