//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "textunit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TTextDlg *TextDlg;
//---------------------------------------------------------------------------
__fastcall TTextDlg::TTextDlg(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::FormResize(TObject *Sender)
{
  Button1->Top = ClientHeight-33;
  Button2->Top = ClientHeight-33;
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::SelectFont(AnsiString fontname)
{
  FontComboBox->Items->Clear();
  for (int i=0;i<Screen->Fonts->Count;i++)
  {
    FontComboBox->Items->Add(Screen->Fonts->operator [](i));
  }
  FontComboBox->ItemIndex = FontComboBox->Items->IndexOf(fontname);
  if (FontComboBox->ItemIndex > 0) Memo->Font->Name = FontComboBox->Items->Strings[FontComboBox->ItemIndex];
  SizeChange(this);
  UpdateStyle();
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::SetFontName(AnsiString value)
{
    if(FontComboBox->Items->Strings[FontComboBox->ItemIndex] != value) {
        SelectFont(value);
    }
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTextDlg::GetFontName()
{
    if (FontComboBox->ItemIndex < 0) return FontComboBox->Text;
    return FontComboBox->Items->Strings[FontComboBox->ItemIndex];
}
//---------------------------------------------------------------------------

void __fastcall TTextDlg::FontComboBoxChange(TObject *Sender)
{                                   
    if (FontComboBox->ItemIndex < 0) FontName = Font->Name;
    Memo->Font->Name = FontName;
    ResizeWin();
}
//---------------------------------------------------------------------------

void __fastcall TTextDlg::SizeChange(TObject *Sender)
{
   Memo->Font->Size = StrToInt(Size->Text);
   ResizeWin();
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::ResizeWin(void)
{
   ClientHeight = Memo->Top+Memo->Height+44;
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::UpdateStyle(void)
{
    TFontStyles style = TFontStyles();
    if (BoldBtn->Down) style = style << fsBold;
    else style = style >> fsBold;
    if (ItalicBtn->Down) style = style << fsItalic;
    else style = style >> fsItalic;
    if (UnderlineBtn->Down) style = style << fsUnderline;
    else style = style >> fsUnderline;
    Memo->Font->Style = style;
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::ChangeStyle(TObject *Sender)
{
    UpdateStyle();
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::SetTextFont(TFont *value)
{
    FontName = value->Name;
    TFontStyles style = value->Style;
    if (style.Contains(fsBold)) BoldBtn->Down = true;
    else BoldBtn->Down = false;
    if (style.Contains(fsItalic)) ItalicBtn->Down = true;
    else ItalicBtn->Down = false;
    if (style.Contains(fsUnderline)) UnderlineBtn->Down = true;
    else UnderlineBtn->Down = false;
    FontComboBox->Text = value->Name;
    FontComboBox->ItemIndex = FontComboBox->Items->IndexOf(value->Name);
    Size->Text = IntToStr(value->Size);
    UpDown1->Position = value->Size;
    Memo->Font = value;
}
TFont* __fastcall TTextDlg::GetTextFont()
{
    return Memo->Font;
}
//---------------------------------------------------------------------------
void __fastcall TTextDlg::SetText(AnsiString value)
{
    Memo->Text = value;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTextDlg::GetText()
{
    return Memo->Text;
}
//---------------------------------------------------------------------------

