//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
                   
#include "progressunit.h"
#include "main.h"
#include "ChildWin.h"                            
#include "textunit.h"
#include "editunit.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "GR32_Image"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
extern BYTE palette[256][3];
BYTE FindColorInPalette(char cPalette[256][3], BYTE r, BYTE g, BYTE b);
typedef void (__stdcall *OptimizeBitmapF)(Graphics::TBitmap *Bitmap);
extern OptimizeBitmapF OptimizeBitmap;
AnsiString ExtractFileBase(AnsiString FileName);
//---------------------------------------------------------------------------
__fastcall TEditForm::TEditForm(TComponent* Owner)
  : TForm(Owner)
{
texture = 0;
image = 0;
bits = 32;
bitsize = 4;
mousedown = false;
zoomout = false;
if (Screen->Cursor == crAppStart) TmpCursor = crDefault;
else TmpCursor = Screen->Cursor;
Bitmap8 = NULL;
}
//---------------------------------------------------------------------------
__fastcall TEditForm::TEditForm(int _texture, int _image, int _bitsize)
  : TForm(Application)
{
  texture = _texture;
  image = _image;
  bitsize = _bitsize;
  bits = bitsize*8;
  mousedown = false;       
  zoomout = false;
  if (Screen->Cursor == crAppStart) TmpCursor = crDefault;
  else TmpCursor = Screen->Cursor;
  Bitmap8 = NULL;
}
//---------------------------------------------------------------------------

bool TEditForm::pop(int& x, int& y)
{
  if(stackPointer > 0)
  {
    int p = stack[stackPointer];
    x = p / Image->Bitmap->Height;
    y = p % Image->Bitmap->Height;
    stackPointer--;
    return 1;
  }
  else
  {
    return 0;
  }
}

bool TEditForm::push(int x, int y)
{
  if(stackPointer < stackSize - 1)
  {
    stackPointer++;
    stack[stackPointer] = Image->Bitmap->Height * x + y;
    return 1;
  }
  else
  {
    return 0;
  }
}

void TEditForm::emptyStack()
{
  int x, y;
  while(pop(x, y));
}
//---------------------------------------------------------------------------
unsigned int TEditForm::getPixel(TBitmap32* bitmap, int x, int y)
{
    int w = bitmap->Width;
    int h = bitmap->Height;
    if (x < 0 || x >= w || y < 0 || y >= h)
        return NULL;
    return bitmap->Pixel[x][y];
}
//---------------------------------------------------------------------------
void TEditForm::setPixel(TBitmap32* bitmap, int x, int y, unsigned int color)
{
    int w = bitmap->Width;
    int h = bitmap->Height;
    if (x < 0 || x >= w || y < 0 || y >= h)
        return;
    bitmap->Pixel[x][y] = color;   
    bitmap->Changed();
}
//---------------------------------------------------------------------------
void TEditForm::drawLine(TBitmap32* bitmap, int x1, int y1, int x2, int y2, unsigned int color)
{
  int tmp;
  bool steep = abs(y2-y1) > abs(x2-x1);
  if(steep){
  	tmp=x1; x1=y1; y1=tmp;
    tmp=x2; x2=y2; y2=tmp;
  }
  int sign = 1;
  if(x1>x2){
    sign = -1;
    x1 *= -1;
    x2 *= -1;
  }
  int dx = x2-x1;
  int dy = abs(y2-y1);
  int err = ((dx/2));
  int ystep = y1 < y2 ? 1:-1;
  int y = y1;

  for(int x=x1;x<=x2;x++){
    if (steep) setPixel(bitmap,y,sign*x,color);
    else setPixel(bitmap,sign*x,y,color);
    err = (err - dy);
    if(err < 0){
      y+=ystep;
      err+=dx;
    }
  }
}
//---------------------------------------------------------------------------
void TEditForm::moveTo(TBitmap32* bitmap, int x, int y)
{
  ml_x = x;
  ml_y = y;
  // bitmap->MoveTo(x,y);
}
//---------------------------------------------------------------------------
void TEditForm::lineTo(TBitmap32* bitmap, int x, int y)
{
  unsigned int color = bitmap->PenColor;
  drawLine(bitmap,ml_x,ml_y,x,y,color);
  ml_x = x;
  ml_y = y;
  // bitmap->LineToTS(x,y);
}
//---------------------------------------------------------------------------
void TEditForm::floodFill(TBitmap32* bitmap, int x, int y, unsigned int newColor, unsigned int oldColor)
{
    int w = bitmap->Width;
    int h = bitmap->Height;
    if (x < 0 || x >= w || y < 0 || y >= h)
        return;
    if (getPixel(bitmap, x,y) != oldColor)
        return;
    setPixel(bitmap, x, y, newColor);
    floodFill(bitmap, x+1, y, newColor, oldColor);
    floodFill(bitmap, x-1, y, newColor, oldColor);
    floodFill(bitmap, x, y+1, newColor, oldColor);
    floodFill(bitmap, x, y-1, newColor, oldColor);
}
//---------------------------------------------------------------------------
void TEditForm::drawHLine(TBitmap32* bitmap, int x1, int y1, int x2, unsigned int color)
{
    for (int x=x1;x<=x2;x++) setPixel(bitmap,x,y1,color);
}
//---------------------------------------------------------------------------
void TEditForm::drawVLine(TBitmap32* bitmap, int x1, int y1, int y2, unsigned int color)
{
    for (int y=y1;y<=y2;y++) setPixel(bitmap,x1,y,color);
}
//---------------------------------------------------------------------------
void TEditForm::frameRect(TBitmap32* bitmap, int x1, int y1, int x2, int y2, unsigned int color)
{
        drawHLine(bitmap, x1, y1, x2-1, color);
        drawHLine(bitmap, x1, y2-1, x2-1, color);
        drawVLine(bitmap, x1, y1, y2-1, color);
        drawVLine(bitmap, x2-1, y1, y2-1, color);
        // bitmap->FrameRectTS(x1,y1,x2,y2,color);
}
//---------------------------------------------------------------------------
void TEditForm::fillRect(TBitmap32* bitmap, int x1, int y1, int x2, int y2, unsigned int color)
{
    for (int y=y1;y<y2;y++)
    {
      for (int x=x1;x<x2;x++)
      {     
         setPixel(bitmap, x, y, color);
      }
    }
    // bitmap->FillRectTS(x1,y1,x2,y2,color);
}
//---------------------------------------------------------------------------
void TEditForm::CirclePlotPoints(TBitmap32* bitmap, int x, int y, int xc, int yc, unsigned int color)
{
        setPixel(bitmap, x + xc, y + yc, color);
        setPixel(bitmap, x - xc, y + yc, color);
        setPixel(bitmap, x + xc, y - yc, color);
        setPixel(bitmap, x - xc, y - yc, color);
}
//---------------------------------------------------------------------------
void TEditForm::FillCirclePlotPoints(TBitmap32* bitmap, int x, int y, int xc, int yc, unsigned int color)
{
        bitmap->FillRectTS((x - (xc+1))+1, (y - (yc+1))+1, x + (xc+1), y + (yc+1), color);
}
//---------------------------------------------------------------------------
void TEditForm::drawCircle(TBitmap32* bitmap, int x, int y, int radius, unsigned int color)
{
    int w = bitmap->Width;
    int h = bitmap->Height;
    if (x < 0 || x >= w || y < 0 || y >= h)
        return;
    int c_x = 0;
    int c_y = radius/2;
    int delta = 2 - 2 * (radius/2);
    int error = 0;

    while(c_y >= 0)
    {
        CirclePlotPoints(bitmap, x+(radius/2), y+(radius/2), c_x, c_y, color);

        error = 2 * (delta + c_y) - 1;
        if(delta < 0 && error <= 0) {
            ++c_x;
            delta += 2 * c_x + 1;
            continue;
        }
        error = 2 * (delta - c_x) - 1;
        if(delta > 0 && error > 0) {
            --c_y;
            delta += 1 - 2 * c_y;
            continue;
        }
        ++c_x;
        delta += 2 * (c_x - c_y);
        --c_y;
    }
}
//---------------------------------------------------------------------------
void TEditForm::drawEllipse(TBitmap32* bitmap, int x, int y, int width, int height, unsigned int color)
{
    int w = bitmap->Width;
    int h = bitmap->Height;
    if (x < 0 || x >= w || y < 0 || y >= h)
        return;
    if (w < 0 || h < 0)
        return;
    int cw = width;
    int ch = height;
    int cw2 = cw/2;
    int ch2 = ch/2;
    if (cw2 <= 0) cw2 = 1;
    if (ch2 <= 0) ch2 = 1;   
    int cx = x+cw2;
    int cy = y+ch2;
    int a2 = cw2 * cw2;
    int b2 = ch2 * ch2;
    int fa2 = 4 * a2, fb2 = 4 * b2;
    int c_x, c_y, sigma;
    for (c_x = 0, c_y = ch2, sigma = 2*b2+a2*(1-2*ch2); b2*c_x <= a2*c_y; c_x++)
    {
        CirclePlotPoints(bitmap, cx, cy, c_x, c_y, color);
        if (sigma >= 0)
        {
            sigma += fa2 * (1 - c_y);
            c_y--;
        }
        sigma += b2 * ((4 * c_x) + 6);
    }
    for (c_x = cw2, c_y = 0, sigma = 2*a2+b2*(1-2*cw2); a2*c_y <= b2*c_x; c_y++)
    {
        CirclePlotPoints(bitmap, cx, cy, c_x, c_y, color);
        if (sigma >= 0)
        {
            sigma += fb2 * (1 - c_x);
            c_x--;
        }
        sigma += a2 * ((4 * c_y) + 6);
    }
}
//---------------------------------------------------------------------------
void TEditForm::drawFilledEllipse(TBitmap32* bitmap, int x, int y, int width, int height, unsigned int color)
{
    int w = bitmap->Width;
    int h = bitmap->Height;
    if (x < 0 || x >= w || y < 0 || y >= h)
        return;
    if (w < 0 || h < 0)
        return;
    int cw = width;
    int ch = height;
    int cw2 = cw/2;
    int ch2 = ch/2;
    if (cw2 <= 0) cw2 = 1;
    if (ch2 <= 0) ch2 = 1;   
    int cx = x+cw2;
    int cy = y+ch2;
    int a2 = cw2 * cw2;
    int b2 = ch2 * ch2;
    int fa2 = 4 * a2, fb2 = 4 * b2;
    int c_x, c_y, sigma;
    for (c_x = 0, c_y = ch2, sigma = 2*b2+a2*(1-2*ch2); b2*c_x <= a2*c_y; c_x++)
    {
        FillCirclePlotPoints(bitmap, cx, cy, c_x, c_y, color);
        if (sigma >= 0)
        {
            sigma += fa2 * (1 - c_y);
            c_y--;
        }
        sigma += b2 * ((4 * c_x) + 6);
    }
    for (c_x = cw2, c_y = 0, sigma = 2*a2+b2*(1-2*cw2); a2*c_y <= b2*c_x; c_y++)
    {
        FillCirclePlotPoints(bitmap, cx, cy, c_x, c_y, color);
        if (sigma >= 0)
        {
            sigma += fb2 * (1 - c_x);
            c_x--;
        }
        sigma += a2 * ((4 * c_y) + 6);
    }
  
}
//---------------------------------------------------------------------------
void TEditForm::drawText(TBitmap32* bitmap, int x, int y, AnsiString text, unsigned int color)
{
    int w = bitmap->Width;
    int h = bitmap->Height;
    if (x < 0 || x >= w || y < 0 || y >= h)
        return;
    bitmap->RenderText(x,y,text,0,color);
    bitmap->Changed();
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::FormClose(TObject *Sender, TCloseAction &Action)
{
 UpdateImage(Sender);
	Action = caFree;
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::FormActivate(TObject *Sender)
{
 curtexture = texture;
 if (textures[texture].image[image].bits > 8)
 {
    MainForm->PaletteTable->PageIndex = 0;
    MainForm->SetPaletteDefault();
 } else
 {
    MainForm->PaletteTable->PageIndex = 1;
    MainForm->SetPalette(textures[texture].image[image].palette);
 }
 MainForm->EditMenuUpdate();
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::FormCreate(TObject *Sender)
{
  HINSTANCE HInst;
  HInst = reinterpret_cast<HINSTANCE>(HInstance);
  Screen->Cursors[crFill] = LoadCursor(HInst, "FILL");
  Screen->Cursors[crPlus] = LoadCursor(HInst, "PLUS");
  Screen->Cursors[crDraw] = LoadCursor(HInst, "DRAW");
  Screen->Cursors[crErase] = LoadCursor(HInst, "ERASE");
  Screen->Cursors[crPick] = LoadCursor(HInst, "PICK");
  Screen->Cursors[crZoomIn] = LoadCursor(HInst, "ZOOMIN");
  Screen->Cursors[crZoomOut] = LoadCursor(HInst, "ZOOMOUT");
  Screen->Cursors[crGrab] = LoadCursor(HInst, "GRAB");
  Screen->Cursors[crGrabbing] = LoadCursor(HInst, "GRABBING");
  Image->Cursor = TCursor(crDraw);
  if (Image->PaintStages->Items[0]->Stage == PST_CLEAR_BACKGND) Image->PaintStages->Items[0]->Stage = PST_CUSTOM;
  Image->RepaintMode = rmOptimizer;

  MainForm->EditMenuUpdate();
    

}
//---------------------------------------------------------------------------
void __fastcall TEditForm::ImagePaintStage(TObject *Sender,
      TBitmap32 *Buffer, DWORD StageNum)
{
  if (bits < 32)
  {                
    Types::TRect R = Image->GetViewportRect();
    Buffer->FillRectS(R, Color32(Image->Color));
    return;
  }
  TColor32 Colors [2] = {0xFFFFFFFF, 0xFFE0E0E0};
  Types::TRect R;
  int I, J;
  int OddY;
  int TilesHorz, TilesVert;
  int TileX, TileY;
  int TileHeight, TileWidth;

  TileHeight = 13;
  TileWidth = 13;

  TilesHorz = Buffer->Width / TileWidth;
  TilesVert = Buffer->Height / TileHeight;
  TileY = 0;

  for (int j=0;j<=TilesVert;j++)
  {
    TileX = 0;
    OddY = j & 0x1;
    for (int i=0;i<=TilesHorz;i++)
    {
      R.Left = TileX;
      R.Top = TileY;
      R.Right = TileX + TileWidth;
      R.Bottom = TileY + TileHeight;
      Buffer->FillRectS(R, Colors[1-((((i&0x1)==OddY)^((j&0x1)==OddY)))]);
      TileX += TileWidth;
    }
    TileY += TileHeight;
  }
}
//---------------------------------------------------------------------------


void __fastcall TEditForm::ImageMouseWheel(TObject *Sender,
      TShiftState Shift, int WheelDelta, TPoint &MousePos, bool &Handled)
{
Image->Scale += (float)(WheelDelta/120)/16;
if ((float)Image->Scale >= 20) Image->Scale = 20;
if ((float)Image->Scale <= 0.019) Image->Scale = 0.01;
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::ImageMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer)
{
 if ((MainForm->DrawingTool == dtZoom) && (Button == mbRight)) zoomout = true;
 else zoomout = false;
 ShowToolCursor();
 TPoint P = Image->ControlToBitmap(TPoint(X,Y));
 switch(MainForm->DrawingTool)
 {
   case dtZoom:
   {
     if (zoomout) Image->Scale -= (float)(0.25f);
     else Image->Scale += (float)(0.25f);
     if ((float)Image->Scale >= 20) Image->Scale = 20;
     if ((float)Image->Scale <= 0.019) Image->Scale = 0.01;
   }
   break;
   case dtFloodFill:
   {
     Drawing = true;
     floodFill(Image->Bitmap,P.x,P.y,Color32(MainForm->FGColor),getPixel(Image->Bitmap,P.x,P.y));
     Image->Bitmap->Changed();
   }
   break;
   case dtColorPicker:
   {
     BYTE c_r,c_g,c_b;
     Gr32::Color32ToRGB(getPixel(Image->Bitmap,P.x,P.y),c_r,c_g,c_b);
     if (Button == mbLeft) MainForm->FGColor = TColor(RGB(c_r,c_g,c_b));
     else if (Button == mbRight) MainForm->BGColor = TColor(RGB(c_r,c_g,c_b));
   }
   break;
   case dtErase:
   {
     Types::TRect R;
     int e_x1 = P.x - 7;
     int e_y1 = P.y - 7;
     int e_x2 = e_x1 + 13;
     int e_y2 = e_y1 + 13;
     Drawing = true;
     fillRect(Image->Bitmap,e_x1,e_y1,e_x2,e_y2,Color32(MainForm->BGColor));
   }
   break;
   case dtText:
   {
     Drawing = true;
     TextDlg->TextFont = Image->Bitmap->Font;
     TextDlg->Text = "";
     if (TextDlg->ShowModal() == mrOk)
     {
       AnsiString input_text = TextDlg->Text; // InputBox("Insert Text","Input Text: ","");
       Image->Bitmap->Font = TextDlg->TextFont;
       drawText(Image->Bitmap,P.x,P.y,input_text,Color32(MainForm->FGColor));
     }
   }
   break;
   case dtPencil:
   {
     Drawing = true;
     Image->Bitmap->PenColor = Color32(MainForm->FGColor);
     moveTo(Image->Bitmap,P.x,P.y);
     setPixel(Image->Bitmap,P.x,P.y,Image->Bitmap->PenColor);
   }
   break;
   case dtLine:
   case dtRect:
   case dtFillRect:
   case dtEllipse:
   case dtFillEllipse:
   {        
     mousedown = true;                       
     Drawing = true;
     if (Button == mbLeft) Image->Bitmap->PenColor = Color32(MainForm->FGColor);
     else if (Button == mbRight) Image->Bitmap->PenColor = Color32(MainForm->BGColor);
     else Image->Bitmap->PenColor = Color32(MainForm->FGColor);
     moveTo(Image->Bitmap,P.x,P.y);
     Origin = TPoint(P.x,P.y);
     MovePt = Origin;
   }
   break;
 };
 mousedown = true;
 
 if (DrawingLayer && (MainForm->DrawingTool != dtText))
 {
   DrawingLayer->Location = FloatRect(0,0,Image->Bitmap->Width,Image->Bitmap->Height);
   DrawingBitmap->Clear();
   DrawingBitmap->Width = Image->Bitmap->Width;
   DrawingBitmap->Height = Image->Bitmap->Height;    
   DrawingBitmap->Changed();
 }

}
//---------------------------------------------------------------------------

void __fastcall TEditForm::ImageMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y,
      TCustomLayer *Layer)
{
 zoomout = false;
 ShowToolCursor();
 if (DrawingLayer)
 {
   DrawingLayer->Location = FloatRect(0,0,0,0);
   DrawingBitmap->Clear();
   DrawingBitmap->Width = 1;
   DrawingBitmap->Height = 1;  
   DrawingBitmap->Clear();
   DrawingBitmap->Changed();
 }
 int _x,_y;
 TPoint P = Image->ControlToBitmap(TPoint(X,Y));
 if (Drawing)
 {
   _x = P.x;
   _y = P.y;
   DrawShape(Origin,TPoint(_x, _y),pmCopy,Button, false);
   Drawing = false;
 }
 mousedown = false;
 UpdateImage(Sender);
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::ImageMouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y, TCustomLayer *Layer)
{
 ShowToolCursor();
 if (mousedown)
 {
   TPoint P = Image->ControlToBitmap(TPoint(X,Y));
   switch(MainForm->DrawingTool)
   {
     case dtErase:
     {
       int e_x1 = P.x - 7;
       int e_y1 = P.y - 7;
       int e_x2 = e_x1 + 13;
       int e_y2 = e_y1 + 13;
       fillRect(Image->Bitmap,e_x1,e_y1,e_x2,e_y2,Color32(MainForm->BGColor));
     }
     break;
     case dtPencil:
     {
       lineTo(Image->Bitmap,P.x,P.y);
     }
     break;
     case dtLine:
     case dtRect:
     case dtFillRect:
     case dtEllipse:
     case dtFillEllipse:
     {
       int _x,_y;
       if (Drawing)
       {
         _x = P.x;
         _y = P.y;
         DrawShape(Origin, MovePt, pmNotXor, mbLeft, true);
         MovePt = TPoint(_x, _y);
         DrawShape(Origin, MovePt, pmNotXor, mbLeft, true);
       }
     }
     break;
   };
 }                


}
//---------------------------------------------------------------------------

void __fastcall TEditForm::UpdateImage(TObject *Sender)
{
      int index = 0;
      int idx = 0;
      int w = textures[texture].image[image].width;
      int h = textures[texture].image[image].height;
      for (int y=0;y<h;y++)
      {
        for (int x=0;x<w;x++)
        {
          if (bitsize == 4)
          {
            int c = getPixel(Image->Bitmap,x,y);
            BYTE r,g,b,a;
            Gr32::Color32ToRGBA(c,r,g,b,a);

            textures[texture].image[image].data[index + 0] = r;
            textures[texture].image[image].data[index + 1] = g;
            textures[texture].image[image].data[index + 2] = b;
            textures[texture].image[image].data[index + 3] = a;

          }
          else if ((bitsize == 2) || (bitsize == 3))
          {             
            int c = getPixel(Image->Bitmap,x,y);
            BYTE r,g,b,a;
            Gr32::Color32ToRGBA(c,r,g,b,a);
            textures[texture].image[image].data[index + 0] = r;
            textures[texture].image[image].data[index + 1] = g;
            textures[texture].image[image].data[index + 2] = b;
          }
          else
          {             
            int c = getPixel(Image->Bitmap,x,y);
            BYTE r,g,b,a;
            Gr32::Color32ToRGBA(c,r,g,b,a);
            BYTE p = FindColorInPalette(textures[texture].image[image].palette,r,g,b);
            textures[texture].image[image].data[index] = p;
          }
          index += bitsize;
          idx += 3;
        }
      }
      for (int i=0;i<MainForm->MDIChildCount;i++)
      {
        if (MainForm->MDIChildren[i]->ClassType() == __classid(TMDIChild))
        {
            if (MainForm->MDIChildren[i]->Tag == texture)
            {
              TMDIChild* Child = dynamic_cast<TMDIChild *>(MainForm->MDIChildren[i]);
              Child->Modified = true;
              Child->Empty = false;
              Child->ListView1SelectItem(Sender,Child->ListView1->Selected,true);
            }
        }
      }
      ShowToolCursor();  
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::PaintBox1Paint(TObject *Sender)
{
/*
PaintBox1->Canvas->FillRect(PaintBox1->ClientRect);
PaintBox1->Canvas->Brush->Color = clBlue;
PaintBox1->Canvas->FillRect(Rect(16,16,32,32));
     floodFill(24,24,clRed,(PaintBox1->Canvas->Pixels[24][24]));
     for (int i=0;i<16;i++) PaintBox1->Canvas->Pixels[24+i][24] = clYellow;
     */
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::ShowToolCursor(void)
{
    switch(MainForm->DrawingTool)
    {
           case dtZoom:
           {
              if (zoomout) Image->Cursor = TCursor(crZoomOut);
              else Image->Cursor = TCursor(crZoomIn);
           }
           break;
           case dtFloodFill:
           {
              Image->Cursor = TCursor(crFill);
           }
           break;
           case dtColorPicker:
           {
              Image->Cursor = TCursor(crPick);
           }
           break;
           case dtErase:
           {
              Image->Cursor = TCursor(crErase);
           }
           break;
           case dtText:
           {
              Image->Cursor = TCursor(crIBeam);
           }
           break;
           case dtPencil:
           {
              Image->Cursor = TCursor(crDraw);
           }
           break;
           case dtLine:
           {
              Image->Cursor = TCursor(crPlus);
           }
           break;
           case dtRect:
           {
              Image->Cursor = TCursor(crPlus);
           }
           break;
           case dtFillRect:
           {
              Image->Cursor = TCursor(crPlus);
           }
           break;
           case dtEllipse:
           {
              Image->Cursor = TCursor(crPlus);
           }
           break;
           case dtFillEllipse:
           {
              Image->Cursor = TCursor(crPlus);
           }
           break;
    };
    if (MainForm->DrawingTool == dtZoom)
    {
      if (((Mouse->CursorPos.x >= (MainForm->Left+Left+Image->Left))&&
      (Mouse->CursorPos.y >= (MainForm->Top+Top+Image->Top))) &&
      ((Mouse->CursorPos.x <= (MainForm->Left+Left+Image->Left+Image->ClientWidth)) &&
      (Mouse->CursorPos.y <= (MainForm->Top+Top+Image->Top+Image->ClientHeight))))
        Screen->Cursor=Image->Cursor;
      else Screen->Cursor=TmpCursor;
    }
    else Screen->Cursor=TmpCursor;
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::Timer1Timer(TObject *Sender)
{
 ShowToolCursor();
}
//---------------------------------------------------------------------------

void __fastcall TEditForm::ImageKeyUp(TObject *Sender, WORD &Key,
      TShiftState Shift)
{

    if (Key == 'Y')
    {
      switch(MainForm->DrawingTool)
      {
             case dtZoom:
             {
                 Image->Scale = 1;
             }
             break;
      };
    }
}
//---------------------------------------------------------------------------



void __fastcall TEditForm::ImageMouseLeave(TObject *Sender)
{
 zoomout = false;
}
//---------------------------------------------------------------------------



void __fastcall TEditForm::SetModified(bool value)
{               
    TMDIChild *TextureDocForm = GetDocumentForm();
    if (TextureDocForm)
    {
      if(TextureDocForm->Modified != value) {
          TextureDocForm->Modified = value;
      }
    }
}
bool __fastcall TEditForm::GetModified()
{
    TMDIChild *TextureDocForm = GetDocumentForm();
    if (TextureDocForm)
    {
      return TextureDocForm->Modified;
    }
    return false;
}

void __fastcall TEditForm::SetEmpty(bool value)
{
    TMDIChild *TextureDocForm = GetDocumentForm();
    if (TextureDocForm)
    {
      if(TextureDocForm->Empty != value) {
          TextureDocForm->Empty = value;
      }
    }
}
bool __fastcall TEditForm::GetEmpty()
{
    TMDIChild *TextureDocForm = GetDocumentForm();
    if (TextureDocForm)
    {
      return TextureDocForm->Empty;
    }
    return false;
}

TMDIChild * __fastcall TEditForm::GetDocumentForm(void)
{
      for (int i=0;i<MainForm->MDIChildCount;i++)
      {
        if (MainForm->MDIChildren[i]->ClassType() == __classid(TMDIChild))
        {
            if (MainForm->MDIChildren[i]->Tag == texture)
            {
              TMDIChild* Child = dynamic_cast<TMDIChild *>(MainForm->MDIChildren[i]);
              return Child;
            }
        }
      }
      return NULL;
}

void __fastcall TEditForm::DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode, TMouseButton Button, bool ADrawing)
{
    switch(MainForm->DrawingTool)
    {
       case dtLine:
       {
          TBitmap32 *Buffer = Image->Bitmap;
          if (ADrawing)
          {
            Buffer = DrawingBitmap;
            Buffer->Clear(SetAlpha(Color32(MainForm->BGColor),0));
          }
          if (Button == mbLeft) Buffer->PenColor = Color32(MainForm->FGColor);
          if (Button == mbRight) Buffer->PenColor = Color32(MainForm->BGColor);
          else Buffer->PenColor = Color32(MainForm->FGColor);
          moveTo(Buffer, TopLeft.x, TopLeft.y);
          lineTo(Buffer, BottomRight.x, BottomRight.y);
          Buffer->Changed();
          Image->Bitmap->Changed();
       }
       break;
       case dtRect:
       {
          bool FillRect_ON = false;
          TBitmap32 *Buffer = Image->Bitmap;
          if (ADrawing)
          {
            Buffer = DrawingBitmap;
            Buffer->Clear(SetAlpha(Color32(MainForm->BGColor),0));
          }
          if (Button == mbLeft) Buffer->PenColor = Color32(MainForm->FGColor);
          if (Button == mbRight) Buffer->PenColor = Color32(MainForm->BGColor);
          else Buffer->PenColor = Color32(MainForm->FGColor);
          TColor32 BrushColor;
          if (Button == mbLeft) BrushColor = Color32(MainForm->BGColor);
          if (Button == mbRight) BrushColor = Color32(MainForm->FGColor);
          else BrushColor = Color32(MainForm->BGColor);
          frameRect(Buffer, TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y, Buffer->PenColor);
          if (FillRect_ON) fillRect(Buffer, TopLeft.x+1, TopLeft.y+1, BottomRight.x-1, BottomRight.y-1, BrushColor);
          Buffer->Changed();
          Image->Bitmap->Changed();
       }
       break;
       case dtFillRect:
       {
          TBitmap32 *Buffer = Image->Bitmap;
          if (ADrawing)
          {
            Buffer = DrawingBitmap;
            Buffer->Clear(SetAlpha(Color32(MainForm->BGColor),0));
          }
          if (Button == mbLeft) Buffer->PenColor = Color32(MainForm->FGColor);
          if (Button == mbRight) Buffer->PenColor = Color32(MainForm->BGColor);
          else Buffer->PenColor = Color32(MainForm->FGColor);
          fillRect(Buffer, TopLeft.x, TopLeft.y, BottomRight.x, BottomRight.y, Buffer->PenColor);
          Buffer->Changed();
          Image->Bitmap->Changed();
       }
       break;
       case dtEllipse:
       {
          bool FillEllipse_ON = false;
          TBitmap32 *Buffer = Image->Bitmap;
          if (ADrawing)
          {
            Buffer = DrawingBitmap;
            Buffer->Clear(SetAlpha(Color32(MainForm->BGColor),0));
          }
          if (Button == mbLeft) Buffer->PenColor = Color32(MainForm->FGColor);
          if (Button == mbRight) Buffer->PenColor = Color32(MainForm->BGColor);
          else Buffer->PenColor = Color32(MainForm->FGColor);
          TColor32 BrushColor;
          if (Button == mbLeft) BrushColor = Color32(MainForm->BGColor);
          if (Button == mbRight) BrushColor = Color32(MainForm->FGColor);
          else BrushColor = Color32(MainForm->BGColor);
          int c_x = TopLeft.x;
          int c_y = TopLeft.y;
          int c_w = BottomRight.x-TopLeft.x;
          int c_h = BottomRight.y-TopLeft.y;
          if (FillEllipse_ON) drawFilledEllipse(Buffer, c_x, c_y, c_w, c_h, BrushColor);
          drawEllipse(Buffer, c_x, c_y, c_w, c_h, Buffer->PenColor);
          Buffer->Changed();
          Image->Bitmap->Changed();
       }
       break;
       case dtFillEllipse:
       {
          TBitmap32 *Buffer = Image->Bitmap;
          if (ADrawing)
          {
            Buffer = DrawingBitmap;
            Buffer->Clear(SetAlpha(Color32(MainForm->BGColor),0));
          }
          if (Button == mbLeft) Buffer->PenColor = Color32(MainForm->FGColor);
          if (Button == mbRight) Buffer->PenColor = Color32(MainForm->BGColor);
          else Buffer->PenColor = Color32(MainForm->FGColor);
          int c_x = TopLeft.x;
          int c_y = TopLeft.y;
          int c_w = BottomRight.x-TopLeft.x;
          int c_h = BottomRight.y-TopLeft.y;
          drawFilledEllipse(Buffer, c_x, c_y, c_w, c_h, Buffer->PenColor);
          Buffer->Changed();
          Image->Bitmap->Changed();
       }
       break;
    };
}
TPositionedLayer* __fastcall TEditForm::CreatePositionedLayer(TFloatRect R)
{
   TPositionedLayer* Result = new TPositionedLayer(Image->Layers);
   Result->Location = R;
   Result->Scaled = true;
   Result->MouseEvents = true;
   Result->OnMouseDown = LayerMouseDown;
   return Result;
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::DrawingHandler(TObject * Sender, TBitmap32* Buffer)
{
    if (Sender->ClassType() == __classid(TPositionedLayer))
    {
      TPositionedLayer* L = dynamic_cast<TPositionedLayer*>(Sender);
      TFloatRect R = L->GetAdjustedLocation();
      int x1=R.Left;
      int x2=R.Right;
      int y1=R.Top;
      int y2=R.Bottom;
      Types::TRect r = Rect(x1,y1,x2,y2);
      Buffer->Draw(r,Rect(0,0,DrawingBitmap->Width,DrawingBitmap->Height),DrawingBitmap);
    }
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::LayerMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (Sender != NULL)
  {
   TPositionedLayer* L = dynamic_cast<TPositionedLayer*>(Sender);
  }
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::FormDestroy(TObject *Sender)
{
 if (DrawingLayer) delete DrawingLayer;
 if (DrawingBitmap) delete DrawingBitmap;
 if (Bitmap8) delete Bitmap8;
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::CreateDrawingLayer(void)
{
  DrawingBitmap = new TBitmap32();
  DrawingBitmap->Width = 1;
  DrawingBitmap->Height = 1;
  DrawingBitmap->DrawMode = dmBlend;
  DrawingBitmap->MasterAlpha = 255;    
  DrawingBitmap->Changed();
  DrawingLayer = CreatePositionedLayer(FloatRect(0,0,0,0));
  DrawingLayer->OnPaint = DrawingHandler;
}
//---------------------------------------------------------------------------
void __fastcall TEditForm::FormDeactivate(TObject *Sender)
{
  MainForm->EditMenuUpdate();
}
//---------------------------------------------------------------------------

