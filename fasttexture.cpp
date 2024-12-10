//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------
USEFORM("main.cpp", MainForm);
USEFORM("childwin.cpp", MDIChild);
USEFORM("about.cpp", AboutBox);
USEFORM("newdlg.cpp", NewTextureDlg);
USEFORM("editunit.cpp", EditForm);
USEFORM("newtileunit.cpp", NewTileDlg);
USEFORM("textunit.cpp", TextDlg);
USEFORM("progressunit.cpp", ProgressForm);
USEFORM("bgcolorunit.cpp", BGColorDlg);
USEFORM("brightnessdlg.cpp", BDlg);
USEFORM("infodlg.cpp", TextureInfoDlg);
//---------------------------------------------------------------------------
void loadimglib();
void unloadimglib();
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
 loadimglib();
	Application->Initialize();
	Application->Title = "Fast Engine Texture Editor";
   Application->CreateForm(__classid(TMainForm), &MainForm);
                 Application->CreateForm(__classid(TAboutBox), &AboutBox);
                 Application->CreateForm(__classid(TNewTextureDlg), &NewTextureDlg);
                 Application->CreateForm(__classid(TNewTileDlg), &NewTileDlg);
                 Application->CreateForm(__classid(TTextDlg), &TextDlg);
                 Application->CreateForm(__classid(TBGColorDlg), &BGColorDlg);
                 Application->CreateForm(__classid(TBDlg), &BDlg);
                 Application->CreateForm(__classid(TTextureInfoDlg), &TextureInfoDlg);
                 Application->Run();
 unloadimglib();
	return 0;
}
//---------------------------------------------------------------------
