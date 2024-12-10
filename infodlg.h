//----------------------------------------------------------------------------
#ifndef infodlgH
#define infodlgH
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
//----------------------------------------------------------------------------
class TTextureInfoDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
	TBevel *Bevel1;
  TLabel *Label1;
  TLabel *Label2;
  TLabel *Label3;
  TLabel *Label4;
  TLabel *Label5;
  TEdit *T_Name;
  TEdit *T_Description;
  TEdit *T_Comments;
  TEdit *T_Company;
  TEdit *T_Copyright;
private:
public:
	virtual __fastcall TTextureInfoDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TTextureInfoDlg *TextureInfoDlg;
//----------------------------------------------------------------------------
#endif    
