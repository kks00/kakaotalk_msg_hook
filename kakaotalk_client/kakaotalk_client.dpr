program kakaotalk_client;

uses
  Vcl.Forms,
  MainFrmUnit in 'MainFrmUnit.pas' {MainFrm},
  IPCClientUnit in 'IPCClientUnit.pas',
  Inject in 'Inject.pas',
  Memory in 'Memory.pas';

{$R *.res}

begin
  Application.Initialize;
  Application.MainFormOnTaskbar := True;
  Application.CreateForm(TMainFrm, MainFrm);
  Application.Run;
end.
