#include <handleapi.h>
#include <winbase.h>
#include <xboxkrnl/xboxkrnl.h>

 BOOL CloseHandle (HANDLE hObject)
 {
     NTSTATUS status = NtClose(hObject);

     if (NT_SUCCESS(status)) {
         return TRUE;
     } else {
         SetLastError(RtlNtStatusToDosError(status));
         return FALSE;
     }
 }
