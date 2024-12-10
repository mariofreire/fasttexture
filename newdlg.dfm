object NewTextureDlg: TNewTextureDlg
  Left = 245
  Top = 108
  BorderStyle = bsDialog
  Caption = 'New Texture'
  ClientHeight = 158
  ClientWidth = 201
  Color = clBtnFace
  ParentFont = True
  OldCreateOrder = True
  Position = poScreenCenter
  PixelsPerInch = 96
  TextHeight = 13
  object OKBtn: TButton
    Left = 23
    Top = 124
    Width = 75
    Height = 25
    Caption = 'OK'
    Default = True
    ModalResult = 1
    TabOrder = 0
  end
  object CancelBtn: TButton
    Left = 103
    Top = 124
    Width = 75
    Height = 25
    Cancel = True
    Caption = 'Cancel'
    ModalResult = 2
    TabOrder = 1
  end
  object RadioGroup1: TRadioGroup
    Left = 8
    Top = 8
    Width = 185
    Height = 105
    Caption = 'Texture Format'
    ItemIndex = 1
    Items.Strings = (
      'Tiles uncompressed RGB'
      'Tiles uncompressed RGBA'
      'Tiles compressed RGB'
      'Tiles compressed RGBA')
    TabOrder = 2
  end
end
