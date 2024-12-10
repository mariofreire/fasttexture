//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "brightnessdlg.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TBDlg *BDlg;            
extern char britable[16][256];
//---------------------------------------------------------------------------
__fastcall TBDlg::TBDlg(TComponent* Owner)
  : TForm(Owner)
{
// default client height = 95
// client height = 289
}
//---------------------------------------------------------------------------

void __fastcall TBDlg::pvClick(TObject *Sender)
{
if (pv->Down) ClientHeight = BDLG_MAXHEIGHT;
else ClientHeight = BDLG_MINHEIGHT;
}
//---------------------------------------------------------------------------

void __fastcall TBDlg::FormShow(TObject *Sender)
{
   pv->Down = false;
   ClientHeight = BDLG_MINHEIGHT;
   ScrollBar1->Position = 0;
   Image1->Picture->Bitmap->Assign(Bitmap);
}
//---------------------------------------------------------------------------
void __fastcall TBDlg::ScrollBar1Change(TObject *Sender)
{
     if (Bitmap)
     {
            int value = ScrollBar1->Position;
            for (int y=0;y<Bitmap->Height;y++)
            {
              for (int x=0;x<Bitmap->Width;x++)
              {
                COLORREF c = ColorToRGB(Bitmap->Canvas->Pixels[x][y]);
                BYTE r,g,b;
                r = GetRValue(c);
                g = GetGValue(c);
                b = GetBValue(c);
                r = britable[value][r];
                g = britable[value][g];
                b = britable[value][b];
                Image1->Picture->Bitmap->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
              }
            }
     }
}
//---------------------------------------------------------------------------
void __fastcall TBDlg::FormCreate(TObject *Sender)
{
  Bitmap = new Graphics::TBitmap();  
}
//---------------------------------------------------------------------------
void __fastcall TBDlg::FormDestroy(TObject *Sender)
{
  delete Bitmap;
}
//---------------------------------------------------------------------------

