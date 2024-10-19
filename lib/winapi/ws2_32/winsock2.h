
typedef unsigned long u_long;

// We do not support this
#define gethostbyaddr(...) NULL
#define WSACleanup(...) Pktdrv_Quit()

#define SOMAXCONN 128

#define MSG_PARTIAL 0

#include <lwip/sockets.h>
#include <lwip/netdb.h>

#include <windows.h>

#define WSAAPI

#define MAKEDWORD(...) 1
#define HIBYTE(...) 1
#define LOBYTE(...) 1
#define MAKEWORD(...) 1

typedef int SOCKET;
#define INVALID_SOCKET ((SOCKET)-1)
#define SOCKET_ERROR -1

typedef struct WSAData {
  WORD           wVersion;
  WORD           wHighVersion;
  //FIXME: There's potentially more here
} WSADATA;

typedef struct sockaddr sockaddr;

typedef WSADATA *LPWSADATA;

typedef void *LPWSAOVERLAPPED_COMPLETION_ROUTINE; //FIXME: !!!
typedef void *LPWSAOVERLAPPED; //FIXME: !!!

int WSAStartup(
  WORD      wVersionRequired,
  LPWSADATA lpWSAData
);

typedef struct _WSABUF {
  ULONG len;
  CHAR  *buf;
} WSABUF, *LPWSABUF;

int WSAGetLastError();

void WSASetLastError(
  int iError
);

int WSAAPI WSASendTo(
  SOCKET                             s,
  LPWSABUF                           lpBuffers,
  DWORD                              dwBufferCount,
  LPDWORD                            lpNumberOfBytesSent,
  DWORD                              dwFlags,
  const sockaddr                     *lpTo,
  int                                iTolen,
  LPWSAOVERLAPPED                    lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);

int WSAAPI WSARecvFrom(
  SOCKET                             s,
  LPWSABUF                           lpBuffers,
  DWORD                              dwBufferCount,
  LPDWORD                            lpNumberOfBytesRecvd,
  LPDWORD                            lpFlags,
  sockaddr                           *lpFrom,
  LPINT                              lpFromlen,
  LPWSAOVERLAPPED                    lpOverlapped,
  LPWSAOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine
);
