//---------------------------------------------------------------------------

#ifndef brightnessdlgH
#define brightnessdlgH
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
#define BDLG_MINHEIGHT 95
#define BDLG_MAXHEIGHT 289
//---------------------------------------------------------------------------
class TBDlg : public TForm
{
__published:	// IDE-managed Components
  TBevel* Bevel1;
  TSpeedButton* pv;
  TLabel* Label1;
  TImage* Image1;
  TButton* Button1;
  TButton* Button2;
  TScrollBar* ScrollBar1;
  void __fastcall pvClick(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall ScrollBar1Change(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall FormDestroy(TObject *Sender);
private:	// User declarations
public:		// User declarations
  Graphics::TBitmap* Bitmap;
  __fastcall TBDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBDlg *BDlg;
//---------------------------------------------------------------------------
#endif
