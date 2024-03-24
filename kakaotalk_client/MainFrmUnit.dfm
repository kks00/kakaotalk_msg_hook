object MainFrm: TMainFrm
  Left = 0
  Top = 0
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsSingle
  ClientHeight = 418
  ClientWidth = 705
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -12
  Font.Name = 'Segoe UI'
  Font.Style = []
  Menu = MainMenu
  Position = poScreenCenter
  TextHeight = 15
  object message_list: TListView
    Left = 8
    Top = 8
    Width = 689
    Height = 401
    Columns = <
      item
        Caption = 'time'
        Width = 170
      end
      item
        Caption = 'message'
        Width = 500
      end>
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -15
    Font.Name = 'Segoe UI'
    Font.Style = []
    ReadOnly = True
    RowSelect = True
    ParentFont = False
    TabOrder = 0
    ViewStyle = vsReport
  end
  object MainMenu: TMainMenu
    Left = 40
    Top = 456
    object Menu1: TMenuItem
      Caption = 'Menu'
      object InitModule1: TMenuItem
        Caption = 'Init Module'
        OnClick = InitModule1Click
      end
    end
  end
  object proc_t: TTimer
    Enabled = False
    Interval = 100
    OnTimer = proc_tTimer
    Left = 368
    Top = 456
  end
  object pipe_t: TTimer
    Enabled = False
    Interval = 100
    OnTimer = pipe_tTimer
    Left = 304
    Top = 456
  end
end
