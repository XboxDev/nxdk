#include <windows.h>
#include <assert.h>
#include <stdint.h>
#include <threads.h>

// Each thread gets a node to keep track of its FLS data.
// Threads must register themselves, so their FLS nodes can be put into a list
// for proper handling of destructors.
typedef struct fls_node_t_
{
    LIST_ENTRY listEntry;
    PVOID slots[FLS_MAXIMUM_AVAILABLE];
} fls_node_t;

static CRITICAL_SECTION fls_lock;
static thread_local fls_node_t fls_node;
static LIST_ENTRY fls_nodes_list;
static uint32_t fls_bitmap[FLS_MAXIMUM_AVAILABLE / 32];
static PFLS_CALLBACK_FUNCTION fls_dtors[FLS_MAXIMUM_AVAILABLE];

// Gets run by the CRT on startup.
static __cdecl VOID fls_init (VOID)
{
    InitializeCriticalSection(&fls_lock);
    InitializeListHead(&fls_nodes_list);
}
#pragma comment(linker, "/include:___fls_init_p")
__attribute__((section(".CRT$XXT"))) void (__cdecl *const __fls_init_p)(void) = fls_init;

VOID fls_register_thread (VOID)
{
    EnterCriticalSection(&fls_lock);
    InsertTailList(&fls_nodes_list, &fls_node.listEntry);
    LeaveCriticalSection(&fls_lock);
}

VOID fls_unregister_thread (VOID)
{
    EnterCriticalSection(&fls_lock);

    RemoveEntryList(&fls_node.listEntry);

    // If this thread used FLS, we need to run destructors to clean up
    for (DWORD dwFlsIndex = 0; dwFlsIndex < FLS_MAXIMUM_AVAILABLE; dwFlsIndex++) {
        // If all 32 slots are empty, we skip checking the remaining 31 slots
        if (fls_bitmap[dwFlsIndex / 32] == 0) {
            dwFlsIndex += 31;
            continue;
        }

        if (fls_bitmap[dwFlsIndex / 32] & (1 << (dwFlsIndex % 32))) {
            // This slot is allocated, if we have data, we must run a destructor
            if (fls_dtors[dwFlsIndex] != NULL && fls_node.slots[dwFlsIndex] != NULL) {
                fls_dtors[dwFlsIndex](fls_node.slots[dwFlsIndex]);
            }
        }
    }

    LeaveCriticalSection(&fls_lock);
}

DWORD FlsAlloc (PFLS_CALLBACK_FUNCTION lpCallback)
{
    DWORD retval = FLS_OUT_OF_INDEXES;

    EnterCriticalSection(&fls_lock);

    for (size_t i = 0; i < (FLS_MAXIMUM_AVAILABLE / 32); i++) {
        if (fls_bitmap[i] == 0xFFFFFFFF) continue;

        unsigned int index = __builtin_ctz(~fls_bitmap[i]);
        fls_bitmap[i] |= (1 << index);
        retval = i*32 + index;
        fls_dtors[retval] = lpCallback;
        break;
    }

    LeaveCriticalSection(&fls_lock);

    return retval;
}

BOOL FlsFree (DWORD dwFlsIndex)
{
    BOOL retval;

    assert(dwFlsIndex < FLS_MAXIMUM_AVAILABLE);

    EnterCriticalSection(&fls_lock);

    if (fls_bitmap[dwFlsIndex / 32] & (1 << (dwFlsIndex % 32))) {
        fls_bitmap[dwFlsIndex / 32] &= ~(1 << (dwFlsIndex % 32));

        // FlsFree needs to call destructors for all entries with this index -
        // this includes entries set by other threads. We use the linked list
        // to access their data.
        if (fls_dtors[dwFlsIndex] != NULL) {
            for (PLIST_ENTRY fls_it = fls_nodes_list.Flink; fls_it != &fls_nodes_list; fls_it = fls_it->Flink) {
                fls_node_t *fls_node = CONTAINING_RECORD(fls_it, fls_node_t, listEntry);

                if (fls_node->slots[dwFlsIndex] != NULL) {
                    fls_dtors[dwFlsIndex](fls_node->slots[dwFlsIndex]);
                }
            }
        }

        fls_dtors[dwFlsIndex] = NULL;
        retval = TRUE;
    } else {
        SetLastError(ERROR_INVALID_PARAMETER);
        retval = FALSE;
    }

    LeaveCriticalSection(&fls_lock);

    return retval;
}

PVOID FlsGetValue (DWORD dwFlsIndex)
{
    assert(dwFlsIndex < FLS_MAXIMUM_AVAILABLE);

    if (dwFlsIndex < FLS_MAXIMUM_AVAILABLE) {
        return fls_node.slots[dwFlsIndex];
    }

    SetLastError(ERROR_INVALID_PARAMETER);
    return NULL;
}

BOOL FlsSetValue (DWORD dwFlsIndex, PVOID lpFlsData)
{
    assert(dwFlsIndex < FLS_MAXIMUM_AVAILABLE);

    if (dwFlsIndex < FLS_MAXIMUM_AVAILABLE) {
        fls_node.slots[dwFlsIndex] = lpFlsData;
        return TRUE;
    }

    SetLastError(ERROR_INVALID_PARAMETER);
    return FALSE;
}
