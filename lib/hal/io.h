#ifndef HAL_IO_H
#define HAL_IO_H

#if defined(__cplusplus)
extern "C"
{
#endif

__attribute__((deprecated))
void IoOutputByte(unsigned short address, unsigned char value);
__attribute__((deprecated))
void IoOutputWord(unsigned short address, unsigned short value);
__attribute__((deprecated))
void IoOutputDword(unsigned short address, unsigned int value);
__attribute__((deprecated))
unsigned char IoInputByte(unsigned short address);
__attribute__((deprecated))
unsigned short IoInputWord(unsigned short address);
__attribute__((deprecated))
unsigned int IoInputDword(unsigned short address);

#ifdef __cplusplus
}
#endif

#endif
