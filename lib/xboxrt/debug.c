//
//	Debug output scrolling code submitted by Robin Mulloy
//
//
#include <stdarg.h>

#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <hal/xbox.h>
#include <hal/video.h>

#include "debug.h"

#define MARGIN         25
#define MARGINS        50 // MARGIN*2

int SCREEN_WIDTH	= 640;
int SCREEN_HEIGHT	= 480;
int SCREEN_BPP = 32;

int nextRow = MARGIN;
int nextCol = MARGIN; 

static const unsigned char systemFont[] =
{
#include "font_terminal.h"
};

static void drawChar(unsigned char c, int x, int y, int fgColour, int bgColour)
{
	unsigned char *videoBuffer = XVideoGetFB();
	videoBuffer += (y * SCREEN_WIDTH + x) * (SCREEN_BPP >> 3);

	unsigned char mask;
	const unsigned char *font = systemFont + (c * FONT_WIDTH);
	int colourToDraw;

	for (int h = 0; h < FONT_HEIGHT; h++)
	{
#if FONT_VMIRROR
		mask = 0x01;
#else
		mask = 0x80;
#endif
		for (int w = 0; w < FONT_WIDTH; w++)
		{
			if ((*font) & mask)
			{
				colourToDraw = fgColour;
			}
			else
			{
				colourToDraw = bgColour;
			}
			switch (SCREEN_BPP)
			{
				case 32:
					*((int*)videoBuffer) = colourToDraw;
					videoBuffer += sizeof(int);
					break;
				case 16:
					*((short*)videoBuffer) = colourToDraw & 0xFFFF;
					videoBuffer += sizeof(short);
					break;
			}
#if FONT_VMIRROR
			mask <<= 1;
#else
			mask >>= 1;
#endif
		}
		
		videoBuffer += (SCREEN_WIDTH-FONT_WIDTH)  * (SCREEN_BPP >> 3);
		font++;
	}
}

// given an unsigned nibble (4 bits) between 0x00 to 0x0F, return '0' to 'F'
static char n2c(int byte)
{
	switch(byte)
	{
		case 0x00: return '0';
		case 0x01: return '1';
		case 0x02: return '2';
		case 0x03: return '3';
		case 0x04: return '4';
		case 0x05: return '5';
		case 0x06: return '6';
		case 0x07: return '7';
		case 0x08: return '8';
		case 0x09: return '9';
		case 0x0A: return 'A';
		case 0x0B: return 'B';
		case 0x0C: return 'C';
		case 0x0D: return 'D';
		case 0x0E: return 'E';
		case 0x0F: return 'F';
		default: return '?';
	}
}

void debugPrintNum(int i)
{
	char num[11];
	num[0] = '0';
	num[1] = 'x';
	num[2] = n2c((i >> 28) & 0x0F);
	num[3] = n2c((i >> 24) & 0x0F);
	num[4] = n2c((i >> 20) & 0x0F);
	num[5] = n2c((i >> 16) & 0x0F);
	num[6] = n2c((i >> 12) & 0x0F);
	num[7] = n2c((i >> 8) & 0x0F);
	num[8] = n2c((i >> 4) & 0x0F);
	num[9] = n2c((i >> 0) & 0x0F);
	num[10] = 0;
	debugPrint(num);
}

void debugPrintBinary( int num )
{
   int x = 0;
   char binNum[50] = {0};
   for( int i=31;i>=0;i-- )
   {
	  binNum[x++] = 0x30 + ((num & (0x01 << i))?1:0);
	  if( (i%4) == 0 ) 
		 binNum[x++] = ' ';
   }
   binNum[x] = 0;
   debugPrint( binNum );
}

void debugPrint(const char *format, ...)
{
	char buffer[512];
	unsigned short len;
	va_list argList;
	va_start(argList, format);
	vsprintf(buffer, format, argList);
	va_end(argList);
	
	VIDEO_MODE vm = XVideoGetMode();
	SCREEN_WIDTH = vm.width;
	SCREEN_HEIGHT = vm.height;
	SCREEN_BPP = vm.bpp;
	int fgColour;
	int bgColour;
	switch (SCREEN_BPP) {
	case 32:
		fgColour = WHITE;
		bgColour = BLACK;
		break;
	case 16:
		fgColour = WHITE_16BPP;
		bgColour = BLACK_16BPP;
	}

	unsigned char *s = (unsigned char*)	buffer;
	while (*s)
	{
		if( nextRow >= (SCREEN_HEIGHT-MARGINS) ) {
			debugClearScreen();
			// advanceScreen();
		}
		
		if (*s == '\n')
		{
			nextRow += FONT_HEIGHT+1;
			nextCol = MARGIN;
		}
		else
		{
			drawChar( *s, nextCol, nextRow, fgColour, bgColour );

			nextCol += FONT_WIDTH+1;
			if( nextCol > (SCREEN_WIDTH-MARGINS))
			{
				nextRow += FONT_HEIGHT+1;
				nextCol  = MARGIN;
			}
		}

		s++;
	}
}

void advanceScreen( void )
{
	int pixelSize = SCREEN_BPP >> 3;
	int screenSize  = SCREEN_WIDTH * (SCREEN_HEIGHT - MARGINS)  * pixelSize;
	int lineSize    = SCREEN_WIDTH * (FONT_HEIGHT + 1) * pixelSize;
	
	unsigned char* thisScreen = XVideoGetFB() + (SCREEN_WIDTH * MARGIN)  * pixelSize;
	unsigned char* prevScreen = thisScreen+lineSize;
		
	memmove(thisScreen, prevScreen, screenSize);

	nextRow -= (FONT_HEIGHT+1);
	nextCol  = MARGIN; 
}

void debugClearScreen( void )
{
	unsigned char* videoBuffer = XVideoGetFB();

	memset( videoBuffer, 0, (SCREEN_BPP >> 3) * (SCREEN_WIDTH * SCREEN_HEIGHT) );
	nextRow = MARGIN;
	nextCol = MARGIN; 
}

void debugPrintHex(const char *buffer, int length)
{
	char tmp[10];
	for (int i = 0; i < length; i++)
	{
		sprintf(tmp, "%02x ", buffer[i]);
		debugPrint(tmp);
	}
}
