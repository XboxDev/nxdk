#ifndef HAL_IO_H
#define HAL_IO_H

#if defined(__cplusplus)
extern "C"
{
#endif

void IoOutputByte(unsigned short address, unsigned char value);
void IoOutputWord(unsigned short address, unsigned short value);
void IoOutputDword(unsigned short address, unsigned int value);
unsigned char IoInputByte(unsigned short address);
unsigned short IoInputWord(unsigned short address);
unsigned int IoInputDword(unsigned short address);

#ifdef __cplusplus
}
#endif

#endif
