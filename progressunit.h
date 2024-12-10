//---------------------------------------------------------------------------

#ifndef progressunitH
#define progressunitH
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
class TProgressForm : public TForm
{
__published:	// IDE-managed Components
  TLabel* WaitLabel;
  TTimer* Timer1;
  TTimer* Timer2;
  void __fastcall Timer2Timer(TObject *Sender);
private:	// User declarations
public:		// User declarations
  __fastcall TProgressForm(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
