object TextureInfoDlg: TTextureInfoDlg
  Left = 245
  Top = 108
  BorderStyle = bsDialog
  Caption = 'Texture File Information'
  ClientHeight = 184
  ClientWidth = 313
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object Bevel1: TBevel
    Left = 8
    Top = 8
    Width = 297
    Height = 133
    Shape = bsFrame
  end
  object Label1: TLabel
    Left = 16
    Top = 16
    Width = 34
    Height = 13
    Caption = 'Name: '
  end
  object Label2: TLabel
    Left = 16
    Top = 40
    Width = 59
    Height = 13
    Caption = 'Description: '
  end
  object Label3: TLabel
    Left = 16
    Top = 64
    Width = 55
    Height = 13
    Caption = 'Comments: '
  end
  object Label4: TLabel
    Left = 16
    Top = 88
    Width = 50
    Height = 13
    Caption = 'Company: '
  end
  object Label5: TLabel
    Left = 16
    Top = 112
    Width = 50
    Height = 13
    Caption = 'Copyright: '
  end
  object OKBtn: TButton
    Left = 79
    Top = 150
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 159
    Top = 150
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object T_Name: TEdit
    Left = 88
    Top = 16
    Width = 209
    Height = 21
    MaxLength = 255
    TabOrder = 2
  end
  object T_Description: TEdit
    Left = 88
    Top = 40
    Width = 209
    Height = 21
    MaxLength = 255
    TabOrder = 3
  end
  object T_Comments: TEdit
    Left = 88
    Top = 64
    Width = 209
    Height = 21
    MaxLength = 255
    TabOrder = 4
  end
  object T_Company: TEdit
    Left = 88
    Top = 88
    Width = 209
    Height = 21
    MaxLength = 63
    TabOrder = 5
  end
  object T_Copyright: TEdit
    Left = 88
    Top = 112
    Width = 209
    Height = 21
    MaxLength = 63
    TabOrder = 6
  end
end
