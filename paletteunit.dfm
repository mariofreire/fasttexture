object PaletteBox: TPaletteBox
  Left = 609
  Top = 604
  AutoSize = True
  BorderStyle = bsToolWindow
  Caption = 'Palette'
  ClientHeight = 48
  ClientWidth = 148
  Color = clBtnFace
  UseDockManager = True
  DragKind = dkDock
  DragMode = dmAutomatic
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'MS Sans Serif'
  Font.Style = []
  FormStyle = fsStayOnTop
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object Panel1: TPanel
    Left = 0
    Top = 0
    Width = 148
    Height = 48
    BevelOuter = bvNone
    TabOrder = 0
    object DrawGrid1: TDrawGrid
      Left = 8
      Top = 6
      Width = 132
      Height = 36
      Color = clBlack
      ColCount = 32
      DefaultColWidth = 4
      DefaultRowHeight = 4
      FixedCols = 0
      RowCount = 8
      FixedRows = 0
      GridLineWidth = 0
      Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goTabs, goThumbTracking]
      ScrollBars = ssNone
      TabOrder = 0
    end
  end
end
