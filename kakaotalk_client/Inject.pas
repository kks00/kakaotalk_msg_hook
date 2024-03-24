unit Inject;

interface

uses Windows, PsAPI, TlHelp32;

function InjectDLL(hProc: THandle; DLLPath: PWideChar): integer;

implementation

function InjectDLL(hProc: THandle; DLLPath: PWideChar): integer;
var
  dwThreadID: Cardinal;
  hThread, hKernel: THandle;
  BytesToWrite, BytesWritten: SIZE_T;
  pRemoteBuffer, pLoadLibrary: Pointer;
  OLDPROTECT: DWORD;
begin
  BytesToWrite := SizeOf(WideChar) * (Length(DLLPath) + 1);
  pRemoteBuffer := VirtualAllocEx(hProc, nil, BytesToWrite, MEM_COMMIT, PAGE_READWRITE);
  if pRemoteBuffer = nil then
    exit(0);
  VirtualProtectEx(hProc, pRemoteBuffer, 1, PAGE_READWRITE, &OLDPROTECT);
  try
    if not WriteProcessMemory(hProc, pRemoteBuffer, DLLPath, BytesToWrite, BytesWritten) then
      exit(0);
    hKernel := GetModuleHandle('kernel32.dll');
    pLoadLibrary := GetProcAddress(hKernel, 'LoadLibraryW');
    hThread := CreateRemoteThread(hProc, nil, 0, pLoadLibrary, pRemoteBuffer, 0, dwThreadID);
    try
      WaitForSingleObject(hThread, INFINITE);
    finally
      CloseHandle(hThread);
    end;
  finally
    VirtualFreeEx(hProc, pRemoteBuffer, 0, MEM_RELEASE);
  end;
  VirtualProtectEx(hProc, pRemoteBuffer, 1, OLDPROTECT, &OLDPROTECT);
end;

end.
