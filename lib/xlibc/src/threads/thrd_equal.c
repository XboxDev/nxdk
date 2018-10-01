#include <threads.h>
#include <stdbool.h>
#include "xboxkrnl/xboxkrnl.h"

int thrd_equal (thrd_t thr0, thrd_t thr1)
{
    // Compare thread-IDs
    return (thr0.id == thr1.id);
}
