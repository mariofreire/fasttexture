//----------------------------------------------------------------------------
#ifndef MainH
#define MainH
//----------------------------------------------------------------------------
#include "ChildWin.h"
#include <vcl\ComCtrls.hpp>
#include <vcl\ExtCtrls.hpp>
#include <vcl\Messages.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\Dialogs.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Menus.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <ActnList.hpp>
#include <ImgList.hpp>
#include <StdActns.hpp>
#include <ToolWin.hpp>
#include <AppEvnts.hpp>
#include <Grids.hpp>
#include <ClipBrd.hpp>


#define MAXIMAGES   1024
#define MAXTEXTURES 24

#define FORMAT_TEXTURE_TILES_RGB                   0x1A
#define FORMAT_TEXTURE_TILES_RGBA                  0xA0
#define FORMAT_TEXTURE_COMPRESSED_TILES_RGB        0xA1
#define FORMAT_TEXTURE_COMPRESSED_TILES_RGBA       0x0A
#define FORMAT_TEXTURE_STRUCT                      0xF0
#define FORMAT_TEXTURE_RESOURCE                    0xA4
#define FORMAT_TEXTURE_LIBRARY                     0xC4

#define calcoffset(_x, _y, _h) ((_h * _x) + _y)
#define paletteoffset(_x, _y, _w) ((_w * _y) + _x)

typedef struct
{
  char  name[256];
  int   animation;
  int   bits;
  char  palette[256][3];
  char  backgroundindex;
  char  background[4];
  int   csize;
  int   width,height;
  char *data;
} image_t;

typedef struct
{
  char ptr[4];
  int  version;
  int  format;
  char name[256];
  char description[256];
  char comments[256];
  char company[64];
  char copyright[64];
  int numimages;
  image_t image[MAXIMAGES];
} texture_t;


extern int numtextures;
extern texture_t textures[MAXTEXTURES];
extern int curtexture;

typedef enum { dtZoom,dtFloodFill,dtColorPicker,dtErase,dtText,dtPencil,dtLine,
dtRect,dtFillRect,dtEllipse,dtFillEllipse } TDrawingTool;

const int crFill  = 5;
const int crPlus  = 6;
const int crDraw  = 7;
const int crErase = 8;
const int crPick = 9;
const int crZoomIn = 10;
const int crZoomOut = 11;
const int crGrab = 12;
const int crGrabbing = 13;

//----------------------------------------------------------------------------
class TMainForm : public TForm
{
__published:
	TMainMenu *MainMenu1;
	TMenuItem *File1;
	TMenuItem *FileNewItem;
	TMenuItem *FileOpenItem;
	TMenuItem *FileCloseItem;
	TMenuItem *Window1;
	TMenuItem *Help1;
	TMenuItem *N1;
	TMenuItem *FileExitItem;
	TMenuItem *WindowCascadeItem;
	TMenuItem *WindowTileItem;
	TMenuItem *WindowArrangeItem;
	TMenuItem *HelpAboutItem;
	TOpenDialog *OpenDialog;
	TMenuItem *FileSaveItem;
	TMenuItem *FileSaveAsItem;
	TMenuItem *Edit1;
	TMenuItem *CutItem;
	TMenuItem *CopyItem;
	TMenuItem *PasteItem;
	TMenuItem *WindowMinimizeItem;
        TStatusBar *StatusBar;
        TActionList *ActionList1;
        TEditCut *EditCut1;
        TEditCopy *EditCopy1;
        TEditPaste *EditPaste1;
        TAction *FileNew1;
        TAction *FileSave1;
        TAction *FileExit1;
        TAction *FileOpen1;
        TAction *FileSaveAs1;
        TWindowCascade *WindowCascade1;
        TWindowTileHorizontal *WindowTileHorizontal1;
        TWindowArrange *WindowArrangeAll1;
        TWindowMinimizeAll *WindowMinimizeAll1;
        TAction *HelpAbout1;
        TWindowClose *FileClose1;
        TWindowTileVertical *WindowTileVertical1;
        TMenuItem *WindowTileItem2;
        TToolBar *ToolBar2;
        TToolButton *ToolButton1;
        TToolButton *ToolButton2;
        TToolButton *ToolButton3;
        TToolButton *ToolButton4;
        TToolButton *ToolButton5;
        TToolButton *ToolButton6;
        TToolButton *ToolButton7;
        TToolButton *ToolButton8;
        TToolButton *ToolButton9;
        TToolButton *ToolButton10;
        TToolButton *ToolButton11;
        TImageList *ImageList1;
        TCoolBar *CoolBar1;
        TCoolBar *CoolBar2;
        TToolBar *ToolBar1;
        TSaveDialog *SaveDialog;
        TApplicationEvents *ApplicationEvents1;
        TAction *TextureTilesRGB;
        TAction *TextureTilesRGBA;
        TAction *TextureTilesCompressedRGB;
        TAction *TextureTilesCompressedRGBA;
        TMenuItem *Texture1;
        TMenuItem *TilesuncompressedRGB1;
        TMenuItem *TilesuncompressedRGBA1;
        TMenuItem *TilescompressedRGB1;
        TMenuItem *TilescompressedRGBA1;
        TMenuItem *Format1;
        TMenuItem *Add1;
        TMenuItem *N2;
        TAction *Add;
  TPanel *Panel1;
  TSpeedButton *SpeedButton1;
  TSpeedButton *SpeedButton2;
  TSpeedButton *SpeedButton3;
  TControlBar *ControlBar3;
  TPanel *Panel2;
  TPanel *Panel3;
  TShape *BGShape;
  TShape *FGShape;
  TColorDialog *FGColorDialog;
  TColorDialog *BGColorDialog;
  TPanel *Panel01;
  TSpeedButton *ZoomBtn;
  TSpeedButton *FillBtn;
  TSpeedButton *ColorPickerBtn;
  TSpeedButton *EraseBtn;
  TSpeedButton *TextBtn;
  TSpeedButton *PenBtn;
  TSpeedButton *LineBtn;
  TSpeedButton *RectBtn;
  TSpeedButton *FRectBtn;
  TSpeedButton *EllipseBtn;
  TSpeedButton *FEllipseBtn;
  TMenuItem *NW;
  TNotebook *PaletteTable;
  TDrawGrid *DrawGrid1;
  TDrawGrid *DrawGrid2;
  TAction *Brightness1;
  TMenuItem *Adjust1;
  TMenuItem *Brightness2;
  TAction *NewTile;
  TMenuItem *NewTile1;
  TAction *TextureInfo;
  TMenuItem *Information1;
  TMenuItem *Image1;
  TAction *Export;
  TAction *Edit2;
  TMenuItem *Export1;
  TMenuItem *Edit3;
  TMenuItem *NE;
  TMenuItem *N3;
  TAction *Import;
  TMenuItem *Import1;
        void __fastcall FileNew1Execute(TObject *Sender);
        void __fastcall FileOpen1Execute(TObject *Sender);
        void __fastcall HelpAbout1Execute(TObject *Sender);
        void __fastcall FileExit1Execute(TObject *Sender);
        void __fastcall FileSave1Execute(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall ApplicationEvents1Idle(TObject *Sender,
          bool &Done);
        void __fastcall AddExecute(TObject *Sender);
        void __fastcall TextureActionUpdate(TObject *Sender);
        void __fastcall TextureTilesRGBExecute(TObject *Sender);
        void __fastcall TextureTilesRGBAExecute(TObject *Sender);
        void __fastcall TextureTilesCompressedRGBExecute(TObject *Sender);
        void __fastcall TextureTilesCompressedRGBAExecute(TObject *Sender);
        void __fastcall TextureTilesRGBUpdate(TObject *Sender);
        void __fastcall TextureTilesRGBAUpdate(TObject *Sender);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall SpeedButton1Click(TObject *Sender);
  void __fastcall SpeedButton2Click(TObject *Sender);
  void __fastcall SpeedButton3Click(TObject *Sender);
  void __fastcall ToolBar1CustomDraw(TToolBar *Sender, const TRect &ARect,
          bool &DefaultDraw);
  void __fastcall ToolBar1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall DrawGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall DrawGrid1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall BGShapeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FGShapeMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall Panel2Resize(TObject *Sender);
  void __fastcall ControlBar3DockDrop(TObject *Sender,
          TDragDockObject *Source, int X, int Y);
  void __fastcall ControlBar3DockOver(TObject *Sender,
          TDragDockObject *Source, int X, int Y, TDragState State,
          bool &Accept);
  void __fastcall FormCreate(TObject *Sender);
  void __fastcall FormShow(TObject *Sender);
  void __fastcall FileSaveAs1Execute(TObject *Sender);
  void __fastcall FileSave1Update(TObject *Sender);
  void __fastcall DrawGrid2DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
  void __fastcall DrawGrid2SelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
  void __fastcall DrawGrid2MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
  void __fastcall DrawGrid2MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall DrawGrid2MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
  void __fastcall FileSaveAs1Update(TObject *Sender);
  void __fastcall EditUpdate(TObject *Sender);
  void __fastcall EditPaste1Execute(TObject *Sender);
  void __fastcall EditCut1Execute(TObject *Sender);
  void __fastcall EditCopy1Execute(TObject *Sender);
  void __fastcall Brightness1Execute(TObject *Sender);
  void __fastcall NewTileExecute(TObject *Sender);
  void __fastcall TextureInfoExecute(TObject *Sender);
  void __fastcall ExportExecute(TObject *Sender);
  void __fastcall EditExecute(TObject *Sender);
  void __fastcall ImportExecute(TObject *Sender);
  void __fastcall ImageItemUpdate(TObject *Sender);
  void __fastcall ImportUpdate(TObject *Sender);
  void __fastcall ImageEditUpdate(TObject *Sender);
private:
    TDrawingTool FDrawingTool;
	void __fastcall CreateMDIChild(bool New, const String Name);
    void __fastcall SetBGColor(TColor value);
    TColor __fastcall GetBGColor();
    void __fastcall SetFGColor(TColor value);
    TColor __fastcall GetFGColor();
    void __fastcall SetDrawingTool(TDrawingTool value);
    TDrawingTool __fastcall GetDrawingTool();
public:
	virtual __fastcall TMainForm(TComponent *Owner);
 bool mdi_child_icon_on;
 void __fastcall FormPopupCreate(int X, int Y);
    TMDIChild * __fastcall GetCurrentMDIChild(void);
    void __fastcall SetPaletteDefault(void);
    void __fastcall SetPalette(char cpalette[256][3]);
    TColor __fastcall FixColor8(TColor color);
    TProgressForm * __fastcall BeginProgressWait(AnsiString message);
    void __fastcall EndProgressWait(TProgressForm * progressform);
    void __fastcall EditMenuUpdate(void);
    void __fastcall ImageEditMenuVisible(TAction* mAction, bool mVisible);
    __property TColor BGColor  = { read=GetBGColor, write=SetBGColor };
    __property TColor FGColor  = { read=GetFGColor, write=SetFGColor };
    __property TDrawingTool DrawingTool  = { read=GetDrawingTool, write=SetDrawingTool };
};
//----------------------------------------------------------------------------
extern TMainForm *MainForm;
extern TMDIChild *__fastcall MDIChildCreate(void);
//----------------------------------------------------------------------------
#endif    
