unit MainFrmUnit;

interface

uses
  Winapi.Windows, Winapi.Messages, System.SysUtils, System.Variants, System.Classes, Vcl.Graphics,
  Vcl.Controls, Vcl.Forms, Vcl.Dialogs, Vcl.Menus, Vcl.ComCtrls, Vcl.ExtCtrls;

type
  TMainFrm = class(TForm)
    message_list: TListView;
    MainMenu: TMainMenu;
    Menu1: TMenuItem;
    InitModule1: TMenuItem;
    proc_t: TTimer;
    pipe_t: TTimer;
    procedure proc_tTimer(Sender: TObject);
    procedure pipe_tTimer(Sender: TObject);
    procedure InitModule1Click(Sender: TObject);
  private
    { Private declarations }
  public
    { Public declarations }
  end;

var
  MainFrm: TMainFrm;

  hProcess: THandle;

implementation

{$R *.dfm}

uses Inject, Memory, IPCClientUnit;

procedure TMainFrm.InitModule1Click(Sender: TObject);
var
  kakaotalk_pid: Integer;

  module_dir: String;
begin
  kakaotalk_pid := GetProcessID('KakaoTalk.exe');
  if kakaotalk_pid = 0 then begin
    ShowMessage('카카오톡 프로세스를 찾을 수 없습니다.');
    exit;
  end;

  hProcess := OpenProcess(PROCESS_ALL_ACCESS, False, kakaotalk_pid);
  if (hProcess = INVALID_HANDLE_VALUE) then begin
    ShowMessage('카카오톡 프로세스를 열 수 없습니다.');
    exit;
  end;

  module_dir := GetCurrentDir + '\kakaotalk_module.dll';
  InjectDLL(hProcess, PWideChar(module_dir));

  pipe_t.Enabled := True;
  proc_t.Enabled := False;
  InitModule1.Enabled := False;
end;

procedure TMainFrm.pipe_tTimer(Sender: TObject);
begin
  if ConnectPipe(PipeName) then begin
    MainFrm.Caption := 'Connected: ' + DWORD(hServer).ToHexString;
    proc_t.Enabled := True;
    pipe_t.Enabled := False;
  end;
end;

procedure TMainFrm.proc_tTimer(Sender: TObject);
var
  read_str: String;
begin
  if DWORD(GetModuleBase(hProcess, 'kakaotalk_module.dll')) <> 0 then begin
    if ShareMemoryRead(hServer, read_str) then begin
      with message_list.Items.Add do begin
        Caption := DateTimeToStr(NOW);
        SubItems.Add(read_str);
      end;
    end;
  end else begin
    MainFrm.Caption := 'Disconnected';
    proc_t.Enabled := False;
    pipe_t.Enabled := False;
    InitModule1.Enabled := True;
  end;
end;

end.
