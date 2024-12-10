object BDlg: TBDlg
  Left = 192
  Top = 107
  BorderIcons = [biSystemMenu, biMinimize]
  BorderStyle = bsDialog
  Caption = 'Brightness'
  ClientHeight = 95
  ClientWidth = 312
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  OldCreateOrder = False
  Position = poScreenCenter
  OnCreate = FormCreate
  OnDestroy = FormDestroy
  OnShow = FormShow
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 297
    Height = 49
    Shape = bsFrame
  end
  object pv: TSpeedButton
    Left = 184
    Top = 64
    Width = 65
    Height = 25
    AllowAllUp = True
    GroupIndex = 1
    Caption = 'Preview'
    OnClick = pvClick
  end
  object Label1: TLabel
    Left = 16
    Top = 24
    Width = 49
    Height = 13
    Caption = 'Brightness'
  end
  object Image1: TImage
    Left = 8
    Top = 96
    Width = 297
    Height = 185
    Stretch = True
  end
  object Button1: TButton
    Left = 8
    Top = 64
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object Button2: TButton
    Left = 96
    Top = 64
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object ScrollBar1: TScrollBar
    Left = 80
    Top = 24
    Width = 217
    Height = 16
    Max = 15
    PageSize = 0
    TabOrder = 2
    OnChange = ScrollBar1Change
  end
end
