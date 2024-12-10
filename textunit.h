//---------------------------------------------------------------------------

#ifndef textunitH
#define textunitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <ExtCtrls.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <AppEvnts.hpp>
#include <StdActns.hpp>
#include <Dialogs.hpp>
#include <CheckLst.hpp>
#include <Tabs.hpp>
#include <MPlayer.hpp>
#include <OleCtnrs.hpp>
#include <msxmldom.hpp>
#include <XMLDoc.hpp>
#include <xmldom.hpp>
#include <XMLIntf.hpp>
#include <ExtDlgs.hpp>
#include <Graphics.hpp>
#include <JPEG.hpp>
#include <PNGImage.hpp>
//---------------------------------------------------------------------------
class TTextDlg : public TForm
{
__published:	// IDE-managed Components
  TSpeedButton* BoldBtn;
  TSpeedButton* ItalicBtn;
  TSpeedButton* UnderlineBtn;
  TLabel* Label1;
  TComboBox* FontComboBox;
  TEdit* Size;
  TUpDown* UpDown1;
  TButton* Button1;
  TButton* Button2;
  TEdit* Memo;
  void __fastcall FormResize(TObject *Sender);
  void __fastcall FontComboBoxChange(TObject *Sender);
  void __fastcall SizeChange(TObject *Sender);
  void __fastcall ChangeStyle(TObject *Sender);
private:
    void __fastcall SetFontName(AnsiString value);
    AnsiString __fastcall GetFontName();
    void __fastcall SetTextFont(TFont *value);
    TFont* __fastcall GetTextFont();
    void __fastcall SetText(AnsiString value);
    AnsiString __fastcall GetText();	// User declarations
public:		// User declarations
  __fastcall TTextDlg(TComponent* Owner);
    void __fastcall SelectFont(AnsiString fontname);
    void __fastcall ResizeWin(void);
    void __fastcall UpdateStyle(void);
    __property AnsiString FontName  = { read=GetFontName, write=SetFontName };
    __property TFont* TextFont  = { read=GetTextFont, write=SetTextFont };
    __property AnsiString Text  = { read=GetText, write=SetText };
};
//---------------------------------------------------------------------------
extern PACKAGE TTextDlg *TextDlg;
//---------------------------------------------------------------------------
#endif
