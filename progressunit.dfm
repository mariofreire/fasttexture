object ProgressForm: TProgressForm
  Left = 404
  Top = 262
  AlphaBlend = True
  AlphaBlendValue = 0
  BorderIcons = []
  BorderStyle = bsDialog
  Caption = 'Working'
  ClientHeight = 32
  ClientWidth = 256
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  Position = poScreenCenter
  Visible = True
  PixelsPerInch = 96
  TextHeight = 13
  object WaitLabel: TLabel
    Left = 0
    Top = 0
    Width = 256
    Height = 32
    Align = alClient
    Alignment = taCenter
    AutoSize = False
    Caption = 'Please Wait...'
    Transparent = True
    Layout = tlCenter
  end
  object Timer1: TTimer
    Enabled = False
    Interval = 300
    Left = 65512
  end
  object Timer2: TTimer
    Interval = 1
    OnTimer = Timer2Timer
    Top = 32
  end
end
