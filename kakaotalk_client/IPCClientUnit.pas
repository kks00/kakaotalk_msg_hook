unit IPCClientUnit;

interface

uses
  Windows, SysUtils, Dialogs;

const
  BUF_SIZE = 1024;
  PipeName: String = 'kakaotalk_msg_hook';

var
  hServer: THandle;

function ConnectPipe(Name: String): Boolean;
function ShareMemoryRead(hMapFile: THandle; var out_message: String): Boolean;
function SendCommand(Command: String): Boolean;

implementation

function OpenMapHandle(Name: String): THandle;
begin
  Result := OpenFileMapping(FILE_MAP_ALL_ACCESS, False, PWideChar(Name));
end;

function ConnectPipe(Name: String): Boolean;
begin
  hServer := OpenMapHandle(PipeName);
  if hServer <> 0 then
    Result := True
  else
    Result := False;
end;

function ShareMemoryRead(hMapFile: THandle; var out_message: String): Boolean;
var
  pBuf: Pointer;
  read_buffer: Pointer;
  read_ansi_str: AnsiString;
  read_str: String;
begin
  Result := False;
  out_message := '';

  try
    pBuf := MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
    if pBuf = nil then begin
      Exit;
    end;

    read_buffer := VirtualAlloc(nil, BUF_SIZE, MEM_COMMIT, PAGE_EXECUTE_READWRITE);
    if read_buffer = nil then begin
      UnmapViewOfFile(pBuf);
      Exit;
    end;
    CopyMemory(read_buffer, pBuf, BUF_SIZE);
    SetString(read_ansi_str, PAnsiChar(read_buffer), BUF_SIZE);
    VirtualFree(read_buffer, 0, MEM_RELEASE);

    read_str := UTF8Decode(read_ansi_str);

    if Pos('<MSG_END>', read_str) > 0 then begin
      out_message := read_str.Substring(0, Pos('<MSG_END>', read_str) - 1);

      ZeroMemory(pBuf, BUF_SIZE);

      UnmapViewOfFile(pBuf);
      Result := True;
    end;
  except
  end;
end;

function ShareMemoryWrite(hMapFile: THandle; wBuf: Pointer; BufSize: SIZE_T; WaitForRead: Boolean): Boolean;
var
  pBuf: PChar;
begin
  Result := False;
  try
    pBuf := MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, BUF_SIZE);
    if pBuf = nil then begin
      Exit;
    end;

    CopyMemory(pBuf, wBuf, BufSize);

    if WaitForRead then
      while (PByte(pBuf)^ <> 0) do
        Sleep(10);

    UnmapViewOfFile(pBuf);

    Result := True;
  except
  end;
end;

function SendCommand(Command: String): Boolean;
var
  aCommand: AnsiString;
begin
  aCommand := AnsiString(Command);
  Result := ShareMemoryWrite(hServer, Pointer(aCommand), Command.Length + 1, True);
end;

end.
