object NewTileDlg: TNewTileDlg
  Left = 245
  Top = 108
  BorderStyle = bsDialog
  Caption = 'New Tile'
  ClientHeight = 166
  ClientWidth = 256
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object GroupBox1: TGroupBox
    Left = 8
    Top = 3
    Width = 241
    Height = 120
    Caption = 'Texture Properties'
    TabOrder = 2
    object Label1: TLabel
      Left = 8
      Top = 26
      Width = 34
      Height = 13
      Caption = 'Name: '
    end
    object Label2: TLabel
      Left = 8
      Top = 50
      Width = 34
      Height = 13
      Caption = 'Width: '
    end
    object Label3: TLabel
      Left = 128
      Top = 50
      Width = 37
      Height = 13
      Caption = 'Height: '
    end
    object Label4: TLabel
      Left = 8
      Top = 74
      Width = 62
      Height = 13
      Caption = 'Color Depth: '
    end
    object Label5: TLabel
      Left = 8
      Top = 96
      Width = 91
      Height = 13
      Caption = 'Background Color: '
    end
    object T_Name: TEdit
      Left = 48
      Top = 24
      Width = 185
      Height = 21
      TabOrder = 0
    end
    object T_Width: TEdit
      Left = 48
      Top = 48
      Width = 49
      Height = 21
      TabOrder = 1
      Text = '64'
    end
    object T_Height: TEdit
      Left = 168
      Top = 48
      Width = 49
      Height = 21
      TabOrder = 2
      Text = '64'
    end
    object T_Depth: TComboBox
      Left = 72
      Top = 72
      Width = 161
      Height = 21
      Style = csDropDownList
      ItemHeight = 13
      TabOrder = 3
      OnChange = T_DepthChange
    end
    object T_BackgroundColor: TPanel
      Left = 104
      Top = 96
      Width = 16
      Height = 16
      BevelOuter = bvNone
      BorderStyle = bsSingle
      Color = clLime
      TabOrder = 4
      OnClick = T_BackgroundColorClick
    end
    object UpDown1: TUpDown
      Left = 97
      Top = 48
      Width = 16
      Height = 21
      Associate = T_Width
      Min = 0
      Max = 9216
      Position = 64
      TabOrder = 5
      Wrap = False
    end
    object UpDown2: TUpDown
      Left = 217
      Top = 48
      Width = 16
      Height = 21
      Associate = T_Height
      Min = 0
      Max = 9216
      Position = 64
      TabOrder = 6
      Wrap = False
    end
    object T_Transparent: TCheckBox
      Left = 136
      Top = 96
      Width = 97
      Height = 17
      Caption = 'Transparent'
      TabOrder = 7
    end
  end
  object OKBtn: TButton
    Left = 55
    Top = 132
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 135
    Top = 132
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object ColorDialog1: TColorDialog
    Ctl3D = True
    Color = clLime
    Left = 216
    Top = 128
  end
end
