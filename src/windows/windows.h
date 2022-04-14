_Static_assert(!hc_ILP32, "Pointers not 64 bit");

// winnt.h
#define DLL_PROCESS_ATTACH 1
#define DLL_THREAD_ATTACH 2
#define DLL_THREAD_DETACH 3
#define DLL_PROCESS_DETACH 0
#define DLL_PROCESS_VERIFIER 4

// winbase.h
#define INVALID_HANDLE_VALUE ((void *)-1)
#define INVALID_FILE_SIZE ((uint32_t)0xffffffff)
#define INVALID_SET_FILE_POINTER ((uint32_t)-1)
#define INVALID_FILE_ATTRIBUTES ((uint32_t)-1)

#define STD_INPUT_HANDLE ((uint32_t)-10)
#define STD_OUTPUT_HANDLE ((uint32_t)-11)
#define STD_ERROR_HANDLE ((uint32_t)-12)

// winuser.h
#define MB_OK 0x00000000
#define MB_OKCANCEL 0x00000001
#define MB_ABORTRETRYIGNORE 0x00000002
#define MB_YESNOCANCEL 0x00000003
#define MB_YESNO 0x00000004
#define MB_RETRYCANCEL 0x00000005
#define MB_CANCELTRYCONTINUE 0x00000006
#define MB_ICONHAND 0x00000010
#define MB_ICONQUESTION 0x00000020
#define MB_ICONEXCLAMATION 0x00000030
#define MB_ICONASTERISK 0x00000040
#define MB_USERICON 0x00000080
#define MB_ICONWARNING MB_ICONEXCLAMATION
#define MB_ICONERROR MB_ICONHAND
#define MB_ICONINFORMATION MB_ICONASTERISK
#define MB_ICONSTOP MB_ICONHAND
#define MB_DEFBUTTON1 0x00000000
#define MB_DEFBUTTON2 0x00000100
#define MB_DEFBUTTON3 0x00000200
#define MB_DEFBUTTON4 0x00000300
#define MB_APPLMODAL 0x00000000
#define MB_SYSTEMMODAL 0x00001000
#define MB_TASKMODAL 0x00002000
#define MB_HELP 0x00004000
#define MB_NOFOCUS 0x00008000
#define MB_SETFOREGROUND 0x00010000
#define MB_DEFAULT_DESKTOP_ONLY 0x00020000
#define MB_TOPMOST 0x00040000
#define MB_RIGHT 0x00080000
#define MB_RTLREADING 0x00100000
#define MB_SERVICE_NOTIFICATION 0x00200000
#define MB_TYPEMASK 0x0000000F
#define MB_ICONMASK 0x000000F0
#define MB_DEFMASK 0x00000F00
#define MB_MODEMASK 0x00003000
#define MB_MISCMASK 0x0000C000

// kernel32.lib
hc_DLLIMPORT int32_t AllocConsole(void);
hc_DLLIMPORT int32_t FreeConsole(void);
hc_DLLIMPORT int32_t AttachConsole(uint32_t processId);

hc_DLLIMPORT noreturn void ExitProcess(uint32_t exitCode);
hc_DLLIMPORT int32_t TerminateProcess(void *handle, uint32_t exitCode);
hc_DLLIMPORT void *GetCurrentProcess(void);

hc_DLLIMPORT void *GetStdHandle(uint32_t type);
hc_DLLIMPORT int32_t WriteFile(void *fileHandle, const void *buffer, uint32_t numberOfBytesToWrite, uint32_t *numberOfBytesWritten, void *overlapped);

// user32.lib
hc_DLLIMPORT int32_t MessageBoxA(void *windowHandle, const char *text, const char *caption, uint32_t type);
