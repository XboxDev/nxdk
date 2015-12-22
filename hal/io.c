#include <hal/io.h>

void IoOutputByte(unsigned short address, unsigned char value) 
{
	__asm__ __volatile__ ("outb %b0,%w1": :"a" (value), "Nd" (address));
}

void IoOutputWord(unsigned short address, unsigned short value)
{
	__asm__ __volatile__ ("outw %0,%w1": :"a" (value), "Nd" (address));
}

void IoOutputDword(unsigned short address, unsigned int value) 
{
	__asm__ __volatile__ ("outl %0,%w1": :"a" (value), "Nd" (address));
}

unsigned char IoInputByte(unsigned short address) 
{
	unsigned char v;
	__asm__ __volatile__ ("inb %w1,%0":"=a" (v):"Nd" (address));
	return v;
}

unsigned short IoInputWord(unsigned short address) 
{
	unsigned short v;
	__asm__ __volatile__ ("inw %w1,%0":"=a" (v):"Nd" (address));
	return v;
}

unsigned int IoInputDword(unsigned short address) 
{
	unsigned int v;
	__asm__ __volatile__ ("inl %w1,%0":"=a" (v):"Nd" (address));
	return v;
}
