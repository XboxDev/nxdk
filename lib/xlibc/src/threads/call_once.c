#include <threads.h>

void call_once (once_flag *flag, void (*func)(void))
{
    if (__sync_bool_compare_and_swap(flag, 0, 1))
    {
        func();
    }
}
