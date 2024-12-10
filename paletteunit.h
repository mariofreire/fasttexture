//---------------------------------------------------------------------------

#ifndef paletteunitH
#define paletteunitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
//---------------------------------------------------------------------------
class TPaletteBox : public TForm
{
__published:	// IDE-managed Components
  TPanel *Panel1;
  TDrawGrid *DrawGrid1;
private:	// User declarations
public:		// User declarations
  __fastcall TPaletteBox(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TPaletteBox *PaletteBox;
//---------------------------------------------------------------------------
#endif
