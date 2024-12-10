//---------------------------------------------------------------------------

#ifndef bgcolorunitH
#define bgcolorunitH
//---------------------------------------------------------------------------        
#include <Classes.hpp>
#include <Controls.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TBGColorDlg : public TForm
{
__published:	// IDE-managed Components
  TBevel* Bevel1;
  TLabel* Label1;
  TButton* OKBtn;
  TButton* CancelBtn;
  TDrawGrid* DrawGrid1;
        void __fastcall DrawGrid1DrawCell(TObject *Sender, int ACol,
          int ARow, TRect &Rect, TGridDrawState State);
private:	// User declarations
public:		// User declarations
  BYTE color_palette[256][3];
  __fastcall TBGColorDlg(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TBGColorDlg *BGColorDlg;
//---------------------------------------------------------------------------
#endif
