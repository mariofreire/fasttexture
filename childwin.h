//----------------------------------------------------------------------------
#ifndef ChildWinH
#define ChildWinH
//----------------------------------------------------------------------------
#include <vcl\Controls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Windows.hpp>
#include <vcl\System.hpp>
#include <StdCtrls.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ExtDlgs.hpp>
#include <PNGImage.hpp>
#include <JPEG.hpp>
#include <ToolWin.hpp>
#include <Menus.hpp>
//----------------------------------------------------------------------------
class TMDIChild : public TForm
{
__published:
        TPanel *Panel1;
        TListView *ListView1;
        TOpenPictureDialog *OpenPictureDialog1;
        TScrollBox *ScrollBox1;
        TSplitter *Splitter1;
        TImage *Image1;
  TToolBar *ToolBar1;
  TToolButton *Button1;
  TToolButton *Button2;
  TToolButton *Button3;
  TPopupMenu *PopupMenu1;
  TMenuItem *AddWP;
  TMenuItem *AddFC;
  TToolButton *ToolButton1;
  TToolButton *ToolButton2;
  TToolButton *ToolButton3;
  TToolButton *Button4;
  TSavePictureDialog *SavePictureDialog1;
  TPanel *bgc;
  TToolButton *ToolButton4;
  TLabel *Label2;
  TShape *bgcolor;
  TColorDialog *ColorDialog1;
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall Button1Click(TObject *Sender);
        void __fastcall ListView1SelectItem(TObject *Sender,
          TListItem *Item, bool Selected);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall ListView1Edited(TObject *Sender, TListItem *Item,
          AnsiString &S);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
  void __fastcall FormResize(TObject *Sender);
  void __fastcall Button2Click(TObject *Sender);
  void __fastcall Button3Click(TObject *Sender);
  void __fastcall FormActivate(TObject *Sender);
  void __fastcall AddWPClick(TObject *Sender);
  void __fastcall Button4Click(TObject *Sender);
  void __fastcall bgcolorMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
private:
        void __fastcall CheckFileSave(void);   
//protected:
//    virtual void __fastcall WndProc(TMessage& Msg);
public:
	   virtual __fastcall TMDIChild(TComponent *Owner);
    bool Modified;
    bool Empty;      
    bool IsNew;
    void __fastcall CreateTile(char* name, int width, int height, int bits, COLORREF backgroundcolor, bool backgroundtransparent);
    AnsiString __fastcall GetFilterExtension(AnsiString Filter, int FilterIndex);
};
//----------------------------------------------------------------------------
#endif
