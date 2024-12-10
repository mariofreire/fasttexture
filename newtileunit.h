//----------------------------------------------------------------------------
#ifndef newtileunitH
#define newtileunitH
//----------------------------------------------------------------------------
#include <vcl\System.hpp>
#include <vcl\Windows.hpp>
#include <vcl\SysUtils.hpp>
#include <vcl\Classes.hpp>
#include <vcl\Graphics.hpp>
#include <vcl\StdCtrls.hpp>
#include <vcl\Forms.hpp>
#include <vcl\Controls.hpp>
#include <vcl\Buttons.hpp>
#include <vcl\ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
//----------------------------------------------------------------------------
class TNewTileDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
  TLabel *Label1;
  TGroupBox *GroupBox1;
  TEdit *T_Name;
  TLabel *Label2;
  TEdit *T_Width;
  TLabel *Label3;
  TEdit *T_Height;
  TLabel *Label4;
  TComboBox *T_Depth;
  TLabel *Label5;
  TPanel *T_BackgroundColor;
  TColorDialog *ColorDialog1;
  TUpDown *UpDown1;
  TUpDown *UpDown2;
  TCheckBox *T_Transparent;
  void __fastcall T_BackgroundColorClick(TObject *Sender);
  void __fastcall T_DepthChange(TObject *Sender);
private:
public:
	virtual __fastcall TNewTileDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TNewTileDlg *NewTileDlg;
//----------------------------------------------------------------------------
#endif    
