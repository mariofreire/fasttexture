//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "newtileunit.h"
//--------------------------------------------------------------------- 
#pragma resource "*.dfm"
TNewTileDlg *NewTileDlg;
//---------------------------------------------------------------------
__fastcall TNewTileDlg::TNewTileDlg(TComponent* AOwner)
	: TForm(AOwner)
{
}
//---------------------------------------------------------------------
void __fastcall TNewTileDlg::T_BackgroundColorClick(TObject *Sender)
{
ColorDialog1->Color = T_BackgroundColor->Color;
if (ColorDialog1->Execute())
{
T_BackgroundColor->Color = ColorDialog1->Color;
}
}
//---------------------------------------------------------------------------

void __fastcall TNewTileDlg::T_DepthChange(TObject *Sender)
{
if (NewTileDlg->T_Depth->ItemIndex == 5)
{
  T_Transparent->Enabled = true;
}
else T_Transparent->Enabled = false;
}
//---------------------------------------------------------------------------

