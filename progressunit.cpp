//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "progressunit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TProgressForm::TProgressForm(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TProgressForm::Timer2Timer(TObject *Sender)
{
AlphaBlendValue = 255;
}
//---------------------------------------------------------------------------

