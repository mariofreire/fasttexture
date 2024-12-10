//---------------------------------------------------------------------------

#ifndef editunitH
#define editunitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "GR32_Image.hpp"
#include <ExtCtrls.hpp>
#include <AppEvnts.hpp>   
//---------------------------------------------------------------------------
#define stackSize 16777216
//---------------------------------------------------------------------------
class TEditForm : public TForm
{
__published:	// IDE-managed Components
  TImgView32 *Image;
  TTimer *Timer1;
  void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
  void __fastcall FormActivate(TObject *Sender);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall ImagePaintStage(TObject *Sender, TBitmap32 *Buffer,
          DWORD StageNum);
  void __fastcall ImageMouseWheel(TObject *Sender, TShiftState Shift,
          int WheelDelta, TPoint &MousePos, bool &Handled);
  void __fastcall ImageMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, TCustomLayer *Layer);
  void __fastcall ImageMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y, TCustomLayer *Layer);
  void __fastcall ImageMouseMove(TObject *Sender, TShiftState Shift, int X,
          int Y, TCustomLayer *Layer);
  void __fastcall UpdateImage(TObject *Sender);
  void __fastcall PaintBox1Paint(TObject *Sender);
  void __fastcall Timer1Timer(TObject *Sender);
  void __fastcall ImageKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
  void __fastcall ImageMouseLeave(TObject *Sender);
  void __fastcall LayerMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FormDestroy(TObject *Sender);
  void __fastcall FormDeactivate(TObject *Sender);
private:	// User declarations
    TPoint Origin, MovePt;
    int stack[stackSize];
    int stackPointer;
    int ml_x,ml_y;
    bool mousedown;
    bool zoomout;
    bool Drawing;
    TCursor TmpCursor;
    TPositionedLayer* DrawingLayer;
    TBitmap32* DrawingBitmap;
    bool pop(int& x, int& y);
    bool push(int x, int y);
    void emptyStack();
    void __fastcall SetModified(bool value);
    bool __fastcall GetModified();
    void __fastcall SetEmpty(bool value);
    bool __fastcall GetEmpty();
    void __fastcall DrawShape(TPoint TopLeft, TPoint BottomRight, TPenMode AMode, TMouseButton Button, bool ADrawing);
    void __fastcall DrawingHandler(TObject * Sender, TBitmap32* Buffer);
    void CirclePlotPoints(TBitmap32* bitmap, int x, int y, int xc, int yc, unsigned int color);
    void FillCirclePlotPoints(TBitmap32* bitmap, int x, int y, int xc, int yc, unsigned int color);
public:		// User declarations
    int texture;
    int image;
    int bitsize;
    int bits;
    Graphics::TBitmap* Bitmap8;
    __fastcall TEditForm(TComponent* Owner);
    __fastcall TEditForm(int _texture, int _image, int _bitsize);
    unsigned int getPixel(TBitmap32* bitmap, int x, int y);
    void setPixel(TBitmap32* bitmap, int x, int y, unsigned int color);
    void drawLine(TBitmap32* bitmap, int x1, int y1, int x2, int y2, unsigned int color);
    void floodFill(TBitmap32* bitmap, int x, int y, unsigned int newColor, unsigned int oldColor);
    void drawHLine(TBitmap32* bitmap, int x1, int y1, int x2, unsigned int color);
    void drawVLine(TBitmap32* bitmap, int x1, int y1, int y2, unsigned int color);
    void frameRect(TBitmap32* bitmap, int x1, int y1, int x2, int y2, unsigned int color);
    void fillRect(TBitmap32* bitmap, int x1, int y1, int x2, int y2, unsigned int color);
    void drawCircle(TBitmap32* bitmap, int x, int y, int radius, unsigned int color);
    void drawEllipse(TBitmap32* bitmap, int x, int y, int width, int height, unsigned int color);
    void drawFilledEllipse(TBitmap32* bitmap, int x, int y, int width, int height, unsigned int color);
    void moveTo(TBitmap32* bitmap, int x, int y);
    void lineTo(TBitmap32* bitmap, int x, int y);
    void drawText(TBitmap32* bitmap, int x, int y, AnsiString text, unsigned int color);
    void __fastcall ShowToolCursor(void);
    TMDIChild * __fastcall GetDocumentForm(void);
    TPositionedLayer* __fastcall CreatePositionedLayer(TFloatRect R);
    void __fastcall CreateDrawingLayer(void);
    __property bool Modified  = { read=GetModified, write=SetModified };
    __property bool Empty  = { read=GetEmpty, write=SetEmpty };
};
//---------------------------------------------------------------------------
#endif
