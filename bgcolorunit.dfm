object BGColorDlg: TBGColorDlg
  Left = 245
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Background Color'
  ClientHeight = 158
  ClientWidth = 323
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 308
    Height = 108
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 127
    Height = 13
    Caption = 'Choose Background Color:'
  end
  object OKBtn: TButton
    Left = 87
    Top = 126
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 167
    Top = 126
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object DrawGrid1: TDrawGrid
    Left = 16
    Top = 32
    Width = 291
    Height = 75
    ColCount = 32
    DefaultColWidth = 8
    DefaultRowHeight = 8
    FixedCols = 0
    RowCount = 8
    FixedRows = 0
    ScrollBars = ssNone
    TabOrder = 2
    OnDrawCell = DrawGrid1DrawCell
  end
end
