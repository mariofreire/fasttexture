//----------------------------------------------------------------------------
#ifndef newdlgH
#define newdlgH
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
class TNewTextureDlg : public TForm
{
__published:        
	TButton *OKBtn;
	TButton *CancelBtn;
        TRadioGroup *RadioGroup1;
private:
public:
	virtual __fastcall TNewTextureDlg(TComponent* AOwner);
};
//----------------------------------------------------------------------------
extern PACKAGE TNewTextureDlg *NewTextureDlg;
//----------------------------------------------------------------------------
#endif    
