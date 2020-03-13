#ifndef HAL_XBOX_H
#define HAL_XBOX_H


#if defined(__cplusplus)
extern "C"
{
#endif

void XReboot();

__attribute__((deprecated))
int  XGetTickCount();

__attribute__((deprecated))
void XSleep(int milliseconds);

void XLaunchXBE(char *xbePath);

// the thread callback function
typedef void (*XThreadCallback)(void *args1, void *args2);
__attribute__((deprecated))
int XCreateThread(XThreadCallback callback, void *args1, void *args2);


#ifdef __cplusplus
}
#endif

#endif
