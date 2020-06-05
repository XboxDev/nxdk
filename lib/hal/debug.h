//
//	Debug output scrolling code submitted by Robin Mulloy
//
//
#ifndef HAL_DEBUG_H
#define HAL_DEBUG_H

#if defined(__cplusplus)
extern "C"
{
#endif

#define WHITE   0x00FFFFFF
#define BLACK   0x00000000
#define RED     0x00FF0000
#define GREEN   0x0000FF00
#define BLUE    0x000000FF

#define WHITE_16BPP   0xFFFF
#define BLACK_16BPP  0x0000

#define WHITE_15BPP   0x7FFF
#define BLACK_15BPP  0x0000

/**
 * Prints a message to whatever debug facilities might
 * be available.
 */
void debugPrint(const char *format, ...);
void debugPrintNum(int i);
void debugPrintBinary( int num );
void debugPrintHex(const char *buffer, int length);
void debugClearScreen( void );
void debugAdvanceScreen( void );


#ifdef __cplusplus
}
#endif

#endif
