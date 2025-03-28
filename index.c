#include <windows.h>
#include <stdio.h>
#include <string.h>

typedef NTSTATUS(WINAPI* RtlAdjustPrivilege_t)(ULONG, BOOLEAN, BOOLEAN, PBOOLEAN);
typedef NTSTATUS(WINAPI* NtRaiseHardError_t)(NTSTATUS, ULONG, ULONG, PULONG, ULONG, PULONG);

int main() {
	char c[10];
	printf("COMPUTER SYSTEM FAILED!!"); // NtRaiseHardError BSOD Occurs
	HMODULE hNtdll = LoadLibraryA("ntdll.dll");
	if (!hNtdll) {
		printf("ntdll.dll MODULE LOAD FAILED, PROGRAMM IS SHUTTING DOWN\n"); // load failed..
		return 1;
	}

	RtlAdjustPrivilege_t RtlAdjustPrivilege = (RtlAdjustPrivilege_t)GetProcAddress(hNtdll, "RtlAdjustPrivilege");
	NtRaiseHardError_t NtRaiseHardError = (NtRaiseHardError_t)GetProcAddress(hNtdll, "NtRaiseHardError");
	
	if (!RtlAdjustPrivilege || !NtRaiseHardError) {
		printf("FUNCTION NOT FOUND\n"); // func load failed...
		FreeLibrary(hNtdll);
		return 1;
	}

	BOOLEAN prevPrivilege;
	ULONG response;

	NTSTATUS status = RtlAdjustPrivilege(19, TRUE, FALSE, &prevPrivilege);
	if (status != 0) {
		printf("Auth is missing..\n"); // failed to auth
		FreeLibrary(hNtdll);
		return 1;
	}

	NtRaiseHardError(0xC0000350, 0, 0, NULL, 6, &response);

	FreeLibrary(hNtdll);
	return 0;

}
