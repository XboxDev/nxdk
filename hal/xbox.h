#ifndef HAL_XBOX_H
#define HAL_XBOX_H

void XReboot();
int  XGetTickCount();
void XSleep(int milliseconds);

void XLaunchXBE(char *xbePath);

// the thread callback function
typedef void (*XThreadCallback)(void *args1, void *args2);
int XCreateThread(XThreadCallback callback, void *args1, void *args2);

#endif
