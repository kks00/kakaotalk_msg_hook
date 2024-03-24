unit Memory;

interface

uses
  Windows, Classes, Math, SysUtils, PSAPI, TlHelp32;

function GetProcessID(Const ExeFileName: string): Integer;
function GetModuleBase(PHandle: THandle; MName: String): Pointer;

implementation

function GetProcessID(Const ExeFileName: string): Integer;
var
  ContinueLoop: BOOL;
  FSnapshotHandle: THandle;
  FProcessEntry32: TProcessEntry32;
begin
  Result := 0;
  FSnapshotHandle := CreateToolHelp32Snapshot(TH32CS_SNAPPROCESS, 0);
  FProcessEntry32.dwSize := SizeOf(FProcessEntry32);
  ContinueLoop := Process32First(FSnapshotHandle, FProcessEntry32);
  while Integer(ContinueLoop) <> 0 do begin
    if (StrIComp(PChar(ExtractFileName(FProcessEntry32.szExeFile)), PChar(ExeFileName)) = 0) or
      (StrIComp(FProcessEntry32.szExeFile, PChar(ExeFileName)) = 0) then begin
      Result := FProcessEntry32.th32ProcessID;
      break;
    end;
    ContinueLoop := Process32Next(FSnapshotHandle, FProcessEntry32);
  end;
  CloseHandle(FSnapshotHandle);
end;

function GetModuleBase(PHandle: THandle; MName: String): Pointer;
var
  Modules: Array of HMODULE;
  cbNeeded, i: Cardinal;
  ModuleInfo: TModuleInfo;
  ModuleName: Array [0 .. MAX_PATH] of Char;
begin
  Result := nil;
  SetLength(Modules, 1024);
  if (PHandle <> 0) then begin
    try
      EnumProcessModules(PHandle, @Modules[0], 1024 * SizeOf(HMODULE), cbNeeded);
    except
      Exit;
    end;
    SetLength(Modules, cbNeeded div SizeOf(HMODULE));
    if (Length(Modules) > 0) then begin
      for i := 0 to Length(Modules) - 1 do begin
        try
          GetModuleBaseName(PHandle, Modules[i], ModuleName, SizeOf(ModuleName));
          if (Pos(MName, ModuleName) > 0) then begin
            GetModuleInformation(PHandle, Modules[i], @ModuleInfo, SizeOf(ModuleInfo));
            Result := ModuleInfo.lpBaseOfDll;
            Exit;
          end;
        Except
        end;
      end;
    end;
  end;
end;

end.
