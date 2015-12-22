//#include "sys/types.h"
//#include "sys/nttypes.h"
//#include "sys/xboxkrnl.h"
//#include "stdlib.h"

#include <stddef.h>
// #include <sys/types.h>
// #include <openxdk/openxdk.h>
#include <xboxkrnl/xboxkrnl.h>

/* a retail Xbox has 64 MB of RAM */
#define RAMSIZE (64 * 1024*1024)
#define RAMSIZE_USE (RAMSIZE - 4096*1024)
/* position of protected mode kernel */
#define PM_KERNEL_DEST 0x100000
/* Lowest allowable address of the kernel (at or above 1 meg) */
#define MIN_KERNEL PM_KERNEL_DEST
/* Highest allowable address */
#define MAX_KERNEL (RAMSIZE_USE-1)


void *malloc2(size_t  s)
{
	PVOID *virt_addr;
	
	//virt_addr = MmAllocateContiguousMemory(s);

	virt_addr = MmAllocateContiguousMemoryEx(s, MIN_KERNEL, MAX_KERNEL, 0, PAGE_READWRITE);
	return virt_addr;
}

void free2(void *x)
{
	MmFreeContiguousMemory(x);
}
