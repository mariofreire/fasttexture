//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "bgcolorunit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBGColorDlg *BGColorDlg;
//---------------------------------------------------------------------------
__fastcall TBGColorDlg::TBGColorDlg(TComponent* Owner)
  : TForm(Owner)
{
}
//---------------------------------------------------------------------------
#define paletteoffset(_x, _y, _w) ((_w * _y) + _x)



void __fastcall TBGColorDlg::DrawGrid1DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
  BYTE r=color_palette[paletteoffset(ACol,ARow,32)][0];
  BYTE g=color_palette[paletteoffset(ACol,ARow,32)][1];
  BYTE b=color_palette[paletteoffset(ACol,ARow,32)][2];
//  DrawGrid1->Canvas->Pen->Mode = pmCopy;
  DrawGrid1->Canvas->Brush->Color = TColor(RGB(r,g,b));
  DrawGrid1->Canvas->FillRect(Rect);
  /*
  if (State.Contains(gdSelected))
  {                                     
    DrawGrid1->Canvas->Pen->Mode = pmXor;
    DrawGrid1->Canvas->Brush->Color = clBlack;
    DrawGrid1->Canvas->FrameRect(Rect);
  }
  */
}
//---------------------------------------------------------------------------

