//---------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
                       
#include "bgcolorunit.h"
#include "ChildWin.h"
#include "progressunit.h"
#include "Main.h"
#include "newtileunit.h"
#include "editunit.h"
//---------------------------------------------------------------------
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
int curtexture=0;
Graphics::TBitmap *CheckerImage;
AnsiString DepthString = "Monochrome (1 bit)\n16 Colors (4 bits)\n256 Colors (8 bits)\nHigh Color (16 bits)\nTrue Color (24 bits)\nFull Colors (32 bits)";
extern BYTE palette[256][3];
BYTE FindColorInPalette(char cPalette[256][3], BYTE r, BYTE g, BYTE b);
typedef void (__stdcall *OptimizeBitmapF)(Graphics::TBitmap *Bitmap);
extern OptimizeBitmapF OptimizeBitmap;
AnsiString ExtractFileBase(AnsiString FileName);
//---------------------------------------------------------------------
__fastcall TMDIChild::TMDIChild(TComponent *Owner)
	: TForm(Owner)
{
  Modified = false;
  Empty = false;
  IsNew = true;

  COLORREF c_color;
  int x=0,y=0;
  int checker = 0;

  CheckerImage = new Graphics::TBitmap();

  CheckerImage->Width = 64;
  CheckerImage->Height = 64;

  for( y = 0; y < 64; y ++ )
  {
  for( x = 0; x < 64; x ++ )
  {
      checker = ((((y&0x8)==0)^((x&0x8))==0))*255;
      if (checker == 0) checker = 192;
      c_color = RGB(checker,checker,checker);
      CheckerImage->Canvas->Pixels[x][y] = TColor(c_color);
  }
  }

}
//---------------------------------------------------------------------
void __fastcall TMDIChild::FormClose(TObject *Sender, TCloseAction &Action)
{

    int mdicount=0;
    int t=curtexture;
    TEditForm *EditForm;
    bool has_edit = false;
    for (int i=0;i<MainForm->MDIChildCount;i++)
    {
      if (MainForm->MDIChildren[i]->ClassType() == __classid(TEditForm))
      {
          EditForm = dynamic_cast<TEditForm*>(MainForm->MDIChildren[i]);
          if (EditForm->texture == t)
          {
            EditForm->Close();
          }
      }
      if (MainForm->MDIChildren[i]->ClassType() == __classid(TMDIChild))
      {
         mdicount++;
      }
    }

    if (MainForm->MDIChildCount>0)
    {
      if (mdicount <= 1)
      {
        MainForm->ToolBar1->Menu = NULL;
        MainForm->NW->Visible = false;
        MainForm->Texture1->Visible = false;
        MainForm->Image1->Visible = false;
        MainForm->ToolBar1->Menu = MainForm->MainMenu1;
      }
      else
      {
        MainForm->ToolBar1->Menu = NULL;
        MainForm->Texture1->Visible = true;  
        MainForm->Image1->Visible = true;
        MainForm->NW->Visible = true;
        MainForm->ToolBar1->Menu = MainForm->MainMenu1;
      }
    }
    else
    {
      MainForm->ToolBar1->Menu = NULL;
      MainForm->NW->Visible = false;
      MainForm->Texture1->Visible = false;
      MainForm->Image1->Visible = false;
      MainForm->ToolBar1->Menu = MainForm->MainMenu1;
    }

	Action = caFree;



}
//---------------------------------------------------------------------


COLORREF blendcolorto(COLORREF s,COLORREF d,int a)
{
  COLORREF c=0;
  BYTE sr=0,sg=0,sb=0;
  BYTE dr=0,dg=0,db=0;
  BYTE r=0,g=0,b=0;
  BYTE alpha=0;
  alpha = 255-a;
  c = s;
  sr = GetRValue(c);
  sg = GetGValue(c);
  sb = GetBValue(c);
  dr = GetRValue(d);
  dg = GetGValue(d);
  db = GetBValue(d);
  dr = sr*(alpha/255.0)+dr*(1.0-(alpha/255.0));
  dg = sg*(alpha/255.0)+dg*(1.0-(alpha/255.0));
  db = sb*(alpha/255.0)+db*(1.0-(alpha/255.0));
  r = dr;
  g = dg;
  b = db;
  c = RGB(r,g,b);
  return c;
}



void __fastcall TMDIChild::Button1Click(TObject *Sender)
{
int i,j,x,y,w,h,bitsize=0;
int cnt = 0;
long index=0;
unsigned char* ScanLine;
unsigned char* AScanLine;
Graphics::TBitmap* bitmap = new Graphics::TBitmap;
TPNGObject* png = new TPNGObject;
TJPEGImage* jpeg = new TJPEGImage();
if (OpenPictureDialog1->Execute())
{
TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, Adding Image...");
try
{

for (i=0;i<OpenPictureDialog1->Files->Count;i++)
{
Modified = true;
Empty = false;
cnt = textures[curtexture].numimages;
if (ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".BMP")
{
bitmap->LoadFromFile(OpenPictureDialog1->Files->Strings[i]);
strcpy(textures[curtexture].image[cnt].name,ExtractFileBase(ExtractFileName(OpenPictureDialog1->Files->Strings[i])).c_str());
textures[curtexture].image[cnt].animation = 0;
textures[curtexture].image[cnt].bits = 32;
textures[curtexture].image[cnt].backgroundindex = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].background[1] = 255;
textures[curtexture].image[cnt].background[2] = 0;
textures[curtexture].image[cnt].background[3] = 0;
textures[curtexture].image[cnt].width = bitmap->Width;
textures[curtexture].image[cnt].height = bitmap->Height;

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}

textures[curtexture].image[cnt].data = (char*)malloc(w*h*bitsize);

/*
for (y=0;y<h;y++)
{
for (x=0;x<w;x++)
{
if (bitsize==4)
{
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+0] = (BYTE)GetRValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+1] = (BYTE)GetGValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+2] = (BYTE)GetBValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+3] = (BYTE)255;
} else if (bitsize==3)
{
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+0] = (BYTE)GetRValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+1] = (BYTE)GetGValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+2] = (BYTE)GetBValue(bitmap->Canvas->Pixels[x][y]);
}
}
}
*/

index=0;

for( y = 0; y < h; y ++ )
{
ScanLine = (unsigned char*)bitmap->ScanLine[y];
for( x = 0; x < w; x ++ )
{
  if (bitsize >= 2)
  {
    textures[curtexture].image[cnt].data[index + 0] = ScanLine[x * 3 + 2];
    textures[curtexture].image[cnt].data[index + 1] = ScanLine[x * 3 + 1];
    textures[curtexture].image[cnt].data[index + 2] = ScanLine[x * 3 + 0];
    if (bitsize == 4)
    {
     textures[curtexture].image[cnt].data[index + 3] = 255;
    }
  }
  else
  {
    textures[curtexture].image[cnt].data[index] = ScanLine[x];
  }
  index += bitsize;
}
}
}
else
if (ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".PNG")
{
png->LoadFromFile(OpenPictureDialog1->Files->Strings[i]);
strcpy(textures[curtexture].image[cnt].name,ExtractFileBase(ExtractFileName(OpenPictureDialog1->Files->Strings[i])).c_str());
textures[curtexture].image[cnt].animation = 0;
textures[curtexture].image[cnt].bits = 32;
textures[curtexture].image[cnt].backgroundindex = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].background[1] = 255;
textures[curtexture].image[cnt].background[2] = 0;
textures[curtexture].image[cnt].background[3] = 0;
textures[curtexture].image[cnt].width = png->Width;
textures[curtexture].image[cnt].height = png->Height;

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
//if (png->Header->ColorType != COLOR_RGBALPHA) bitsize = 3;
//else bitsize = 4;
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}


textures[curtexture].image[cnt].data = (unsigned char*)malloc(w*h*bitsize);

/*
for (y=0;y<h;y++)
{
for (x=0;x<w;x++)
{
if (bitsize==4)
{
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+0] = (BYTE)GetRValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+1] = (BYTE)GetGValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+2] = (BYTE)GetBValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+3] = (BYTE)255;
} else if (bitsize==3)
{
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+0] = (BYTE)GetRValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+1] = (BYTE)GetGValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+2] = (BYTE)GetBValue(bitmap->Canvas->Pixels[x][y]);
}
}
}
*/

index=0;

for( y = 0; y < h; y ++ )
{
ScanLine = (unsigned char*)png->Scanline[y];
if (bitsize == 4)
{
if (png->Header->ColorType == COLOR_RGBALPHA) AScanLine = (unsigned char*)png->AlphaScanline[y];
}
for( x = 0; x < w; x ++ )
{
  if (bitsize >= 2)
  {
    textures[curtexture].image[cnt].data[index + 0] = ScanLine[x * 3 + 2];
    textures[curtexture].image[cnt].data[index + 1] = ScanLine[x * 3 + 1];
    textures[curtexture].image[cnt].data[index + 2] = ScanLine[x * 3 + 0];
    if (bitsize == 4)
    {
     if (AScanLine && png->Header->ColorType == COLOR_RGBALPHA)
     {
      textures[curtexture].image[cnt].data[index + 3] = AScanLine[x];
     }
     else textures[curtexture].image[cnt].data[index + 3] = 255;
    }
  }
  else
  {
    textures[curtexture].image[cnt].data[index] = (BYTE)ScanLine[x];
  }
  index += bitsize;
}
}
}
else
if ((ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".JPG")||(ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".JPEG"))
{
jpeg->LoadFromFile(OpenPictureDialog1->Files->Strings[i]);
strcpy(textures[curtexture].image[cnt].name,ExtractFileBase(ExtractFileName(OpenPictureDialog1->Files->Strings[i])).c_str());
textures[curtexture].image[cnt].animation = 0;
textures[curtexture].image[cnt].bits = 32;
textures[curtexture].image[cnt].backgroundindex = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].background[1] = 255;
textures[curtexture].image[cnt].background[2] = 0;
textures[curtexture].image[cnt].background[3] = 0;
textures[curtexture].image[cnt].width = jpeg->Width;
textures[curtexture].image[cnt].height = jpeg->Height;

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}

textures[curtexture].image[cnt].data = (unsigned char*)malloc(w*h*bitsize);

/*
for (y=0;y<h;y++)
{
for (x=0;x<w;x++)
{
if (bitsize==4)
{
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+0] = (BYTE)GetRValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+1] = (BYTE)GetGValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+2] = (BYTE)GetBValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+3] = (BYTE)255;
} else if (bitsize==3)
{
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+0] = (BYTE)GetRValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+1] = (BYTE)GetGValue(bitmap->Canvas->Pixels[x][y]);
  textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+2] = (BYTE)GetBValue(bitmap->Canvas->Pixels[x][y]);
}
}
}
*/

Graphics::TBitmap *jpegbitmap = new Graphics::TBitmap();
jpegbitmap->Assign(jpeg);

index=0;

for( y = 0; y < h; y ++ )
{
ScanLine = (unsigned char*)jpegbitmap->ScanLine[y];
for( x = 0; x < w; x ++ )
{
  if (bitsize >= 2)
  {
    textures[curtexture].image[cnt].data[index + 0] = ScanLine[x * 3 + 2];
    textures[curtexture].image[cnt].data[index + 1] = ScanLine[x * 3 + 1];
    textures[curtexture].image[cnt].data[index + 2] = ScanLine[x * 3 + 0];
    if (bitsize == 4)
    {
     textures[curtexture].image[cnt].data[index + 3] = 255;
    }
  }
  else
  {
    textures[curtexture].image[cnt].data[index] = (BYTE)ScanLine[x];
  }
  index += bitsize;
}
}

delete jpegbitmap;

}


TListItem* item = ListView1->Items->Add();
item->Caption = textures[curtexture].image[textures[curtexture].numimages].name;
item->SubItems->Add(IntToStr(textures[curtexture].numimages));
item->SubItems->Add(IntToStr(textures[curtexture].image[textures[curtexture].numimages].width)+"x"+IntToStr(textures[curtexture].image[textures[curtexture].numimages].height));

textures[curtexture].numimages++;

ListView1->Selected = item;   
ListView1->ItemFocused = item;

}

}
catch(...)
{
MainForm->EndProgressWait(progressform);
}

MainForm->EndProgressWait(progressform);

}
delete png;
delete jpeg;
delete bitmap;
}
//---------------------------------------------------------------------------



void __fastcall TMDIChild::ListView1SelectItem(TObject *Sender,
      TListItem *Item, bool Selected)
{
  int x=0,y=0,w=0,h=0,bitsize=0;
  BYTE r=0,g=0,b=0,a=0,p=0;
  int cnt=StrToInt(Item->SubItems->Strings[0]);
  if (textures[curtexture].image[cnt].bits <= 8)
  {
    if (Selected) bgc->Visible = true;
    else bgc->Visible = false;
  }
  else
  {
    if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
    {
     if ((textures[curtexture].image[cnt].bits == 16)||(textures[curtexture].image[cnt].bits == 24)) bgc->Visible = true;
     else bgc->Visible = false;
    }
    else
    {
      if (Selected) bgc->Visible = true;
      else bgc->Visible = false;
    }
  }

if (!Selected)
{
Button3->Enabled = false;
Button4->Enabled = false;
return;
}
else
{
Button3->Enabled = true;
Button4->Enabled = true;
}

Graphics::TBitmap* Preview = new Graphics::TBitmap;

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}
if (bitsize > 1)
{
BYTE bg_r = textures[curtexture].image[cnt].background[0];
BYTE bg_g = textures[curtexture].image[cnt].background[1];
BYTE bg_b = textures[curtexture].image[cnt].background[2];
bgcolor->Brush->Color = TColor(RGB(bg_r,bg_g,bg_b));
}
else
{
BYTE bg_c = textures[curtexture].image[cnt].backgroundindex;
BYTE bg_r = textures[curtexture].image[cnt].palette[bg_c][0];
BYTE bg_g = textures[curtexture].image[cnt].palette[bg_c][1];
BYTE bg_b = textures[curtexture].image[cnt].palette[bg_c][2];
bgcolor->Brush->Color = TColor(RGB(bg_r,bg_g,bg_b));
}

long row, col,index=0;
int checker=0;
COLORREF i_color=0,c_color=0,bkcolor=0;

bkcolor = ColorToRGB(ScrollBox1->Color);

Preview->Width = w;
Preview->Height = h;

index = 0;

if (bitsize == 4)
{
for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  a = (int)textures[curtexture].image[cnt].data[index + 3];
  i_color = RGB(r,g,b);
  c_color = RGB(GetRValue(bkcolor),GetGValue(bkcolor),GetBValue(bkcolor));
//  Preview->Canvas->Pixels[x][y] = TColor(i_color);
  Preview->Canvas->Pixels[x][y] = TColor(blendcolorto(c_color,i_color,a));
  index += bitsize;
}
}
}
else if ((bitsize == 2)||(bitsize == 3))
{

for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  Preview->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
  index += bitsize;
}
}

}
else
{

for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  p = (BYTE)textures[curtexture].image[cnt].data[index];
  r = (BYTE)textures[curtexture].image[cnt].palette[p][0];
  g = (BYTE)textures[curtexture].image[cnt].palette[p][1];
  b = (BYTE)textures[curtexture].image[cnt].palette[p][2];
  Preview->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
  index += bitsize;
}
}

}

Image1->Picture->Bitmap = Preview;



/*
for (y=0;y<h;y++)
{
for (x=0;x<w;x++)
{
  r = textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+0];
  g = textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+1];
  b = textures[curtexture].image[cnt].data[(calcoffset(x,y,h)*bitsize)+2];
  Preview->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
}
} */
delete Preview;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::FormDestroy(TObject *Sender)
{
int i=curtexture,j=0;

textures[i].ptr[0] = 'F';
textures[i].ptr[1] = 'T';
textures[i].ptr[2] = 'E';
textures[i].ptr[3] = 'X';
textures[i].version = 1;
textures[i].format = FORMAT_TEXTURE_TILES_RGBA;
strcpy(textures[i].name,"");
strcpy(textures[i].description,"");
strcpy(textures[i].comments,"");
strcpy(textures[i].company,"");
strcpy(textures[i].copyright,"");
for(j=0;j<textures[i].numimages;j++)
{
 strcpy(textures[i].image[j].name,"");
 textures[i].image[j].animation = 0;
 textures[i].image[j].bits = 32;
 textures[i].image[j].backgroundindex = 0;
 textures[i].image[j].width = 0;
 textures[i].image[j].height = 0;
 if (textures[i].image[j].data) free(textures[i].image[j].data);
}
textures[i].numimages = 0;
numtextures--;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::ListView1Edited(TObject *Sender,
      TListItem *Item, AnsiString &S)
{
  int cnt=StrToInt(Item->SubItems->Strings[0]);
  strcpy(textures[curtexture].image[cnt].name,S.c_str());
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::FormCreate(TObject *Sender)
{
Modified = false;
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::CheckFileSave(void)
{
  if(Modified)
  {
    AnsiString ConfirmText = "Save Changes to " + Caption + "?";
    switch(MessageBox(Handle, ConfirmText.c_str(),
           "Confirm",MB_YESNOCANCEL | MB_ICONQUESTION))
    {
      case ID_YES:
          MainForm->FileSave1Execute(this);
          break;

      case ID_CANCEL:
          Abort();
    };
  }
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::FormCloseQuery(TObject *Sender, bool &CanClose)
{
  try
  {
    CheckFileSave();
  }
  catch(...)
  {
    CanClose = False;
  }
}
//---------------------------------------------------------------------------
/*
void __fastcall TMDIChild::WndProc(TMessage& Msg)
{
    switch (Msg.Msg)
    {
        case WM_SIZE:
        {
           if (MainForm->ActiveMDIChild)
           {
              if (MainForm->ActiveMDIChild->WindowState == wsMaximized)
              {
                  MainForm->ToolBar1->Indent = 21;
                  MainForm->ToolBar1->Repaint();
                  MainForm->ToolBar2->Repaint();
                  MainForm->mdi_child_icon_on = true;
              }
              else
              {
                MainForm->mdi_child_icon_on = false;
                MainForm->ToolBar1->Indent = 5;
                MainForm->ToolBar1->Repaint();
                MainForm->ToolBar2->Repaint();
              }
           }
           else
           {
             MainForm->mdi_child_icon_on = false;
             MainForm->ToolBar1->Indent = 5;
             MainForm->ToolBar1->Repaint();
             MainForm->ToolBar2->Repaint();
           }
        }
        break;
    }
    TForm::WndProc(Msg);
}
*/
//---------------------------------------------------------------------------

void proportionimage(int sizeWidth,int sizeHeight,int sourceWidth,int sourceHeight,int *destWidth,int *destHeight)
{
     int w=0,h=0;
     if (sourceWidth > sourceHeight)
     {
       w = sizeWidth;
       h = (sizeWidth * sourceHeight) / sourceWidth;
     }
     else
     {
       h = sizeHeight;
       w = (sizeHeight * sourceWidth) / sourceHeight;
     }

     *destWidth = w;
     *destHeight = h;
}

//---------------------------------------------------------------------------

void __fastcall TMDIChild::FormResize(TObject *Sender)
{
           if (MainForm->ActiveMDIChild)
           {
              if (MainForm->ActiveMDIChild->WindowState == wsMaximized)
              {
                  MainForm->ToolBar1->Indent = 21;
                  MainForm->ToolBar1->Repaint();
                  MainForm->ToolBar2->Repaint();
                  MainForm->mdi_child_icon_on = true;
              }
              else
              {
                MainForm->mdi_child_icon_on = false;
                MainForm->ToolBar1->Indent = 5;
                MainForm->ToolBar1->Repaint();
                MainForm->ToolBar2->Repaint();
              }
           }
           else
           {
             MainForm->mdi_child_icon_on = false;
             MainForm->ToolBar1->Indent = 5;
             MainForm->ToolBar1->Repaint();
             MainForm->ToolBar2->Repaint();
           }

}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::Button2Click(TObject *Sender)
{
NewTileDlg->T_Name->Text = "Texture"+IntToStr(textures[curtexture].numimages+1);
NewTileDlg->T_Width->Text = IntToStr(64);
NewTileDlg->T_Height->Text = IntToStr(64);
NewTileDlg->T_Depth->Items->Text = DepthString;
NewTileDlg->T_Transparent->Checked = false;
if (!((textures[curtexture].format == FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format == FORMAT_TEXTURE_COMPRESSED_TILES_RGBA)))
{
NewTileDlg->T_Depth->Items->Delete(5);
NewTileDlg->T_Depth->ItemIndex = 4;
NewTileDlg->T_Transparent->Enabled = false;
}
else
{
NewTileDlg->T_Depth->ItemIndex = 5;
NewTileDlg->T_Transparent->Enabled = true;
}
if (NewTileDlg->ShowModal() == mrOk)
{
   TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, Creating Image...");
   char name[256];
   strcpy(name, NewTileDlg->T_Name->Text.c_str());
   int width = StrToInt(NewTileDlg->T_Width->Text);
   int height = StrToInt(NewTileDlg->T_Height->Text);
   int bits = 32;
   COLORREF backgroundcolor = ColorToRGB(NewTileDlg->T_BackgroundColor->Color);
   bool backgroundtransparent = NewTileDlg->T_Transparent->Checked;
   switch(NewTileDlg->T_Depth->ItemIndex)
   {
     case 0:
     {
       bits = 1;
     }
     break;
     case 1:
     {
       bits = 4;
     }
     break;
     case 2:
     {
       bits = 8;
     }
     break;
     case 3:
     {
       bits = 16;
     }
     break;
     case 4:
     {
       bits = 24;
     }
     break;
     case 5:
     {
       bits = 32;
     }
     break;
   };
   CreateTile(name,width,height,bits,backgroundcolor,backgroundtransparent);
   MainForm->EndProgressWait(progressform);
}
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::CreateTile(char* name, int width, int height, int bits, COLORREF backgroundcolor, bool backgroundtransparent)
{
    int n=textures[curtexture].numimages;
    int bitsize = 1;
    switch(bits)
    {
      case 1:
      {
        bitsize = 1;
      }
      break;
      case 2:
      {
        bitsize = 1;
      }
      break;
      case 4:
      {
        bitsize = 1;
      }
      break;
      case 8:
      {
        bitsize = 1;
      }
      break;
      case 16:
      {
        if ((textures[curtexture].format == FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format == FORMAT_TEXTURE_COMPRESSED_TILES_RGBA)) bitsize = 4;
        else bitsize = 3; //2;
      }
      break;
      case 24:
      {
        if ((textures[curtexture].format == FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format == FORMAT_TEXTURE_COMPRESSED_TILES_RGBA)) bitsize = 4;
        else bitsize = 3;
      }
      break;
      case 32:
      {
        bitsize = 4;
      }
      break;
    };
    if (bitsize <= 1) MainForm->SetPaletteDefault();
    int size=width*height*bitsize;
    strcpy(textures[curtexture].image[n].name,name);
    textures[curtexture].image[n].width=width;
    textures[curtexture].image[n].height=height;
    textures[curtexture].image[n].bits=bits;
    textures[curtexture].image[n].backgroundindex=0;
    textures[curtexture].image[n].background[0] = GetRValue(backgroundcolor);
    textures[curtexture].image[n].background[1] = GetGValue(backgroundcolor);
    textures[curtexture].image[n].background[2] = GetBValue(backgroundcolor);
    textures[curtexture].image[n].background[3] = backgroundtransparent ? 0 : 255;
    textures[curtexture].image[n].data = (char*)malloc(size);
    memset(textures[curtexture].image[n].data,0,size);
    if (bitsize > 1)
    {
      for(int i=0;i<size;i+=bitsize)
      {
        textures[curtexture].image[n].data[i + 0] = textures[curtexture].image[n].background[0];
        textures[curtexture].image[n].data[i + 1] = textures[curtexture].image[n].background[1];
        textures[curtexture].image[n].data[i + 2] = textures[curtexture].image[n].background[2];
        if (bits > 24) textures[curtexture].image[n].data[i + 3] = backgroundtransparent ? 0 : textures[curtexture].image[n].background[3];
        else textures[curtexture].image[n].data[i + 3] = 255;
      }
    } else
    {
      for(int j=0;j<256;j++)
      {
        textures[curtexture].image[n].palette[j][0] = palette[j][0];
        textures[curtexture].image[n].palette[j][1] = palette[j][1];
        textures[curtexture].image[n].palette[j][2] = palette[j][2];
      }
      BYTE bg_r = GetRValue(backgroundcolor);
      BYTE bg_g = GetGValue(backgroundcolor);
      BYTE bg_b = GetBValue(backgroundcolor);
      textures[curtexture].image[n].backgroundindex = FindColorInPalette(textures[curtexture].image[n].palette,bg_r,bg_g,bg_b);
      BYTE b_r=textures[curtexture].image[n].background[0];
      BYTE b_g=textures[curtexture].image[n].background[1];
      BYTE b_b=textures[curtexture].image[n].background[2];
      BYTE c=FindColorInPalette(textures[curtexture].image[n].palette,b_r,b_g,b_b);
      for(int i=0;i<size;i++)
      {
        textures[curtexture].image[n].data[i] = c;
      }
    }
    TListItem* item = ListView1->Items->Add();
    item->Caption = textures[curtexture].image[n].name;
    item->SubItems->Add(IntToStr(n));
    item->SubItems->Add(IntToStr(textures[curtexture].image[n].width)+"x"+IntToStr(textures[curtexture].image[n].height));
    textures[curtexture].numimages++;
    Modified = true;
    Empty = false; 
    ListView1->Selected = item;
    ListView1->ItemFocused = item;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::Button3Click(TObject *Sender)
{
    if (!ListView1->Selected) return;
    int t=Tag;
    curtexture=t;
    TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, loading Image...");
    try
    {
    int img=StrToInt(ListView1->Selected->SubItems->Strings[0]);
    TEditForm *EditForm;
    bool has_edit = false;
    for (int i=0;i<MainForm->MDIChildCount;i++)
    {
      if (MainForm->MDIChildren[i]->ClassType() == __classid(TEditForm))
      {
          EditForm = dynamic_cast<TEditForm*>(MainForm->MDIChildren[i]);
          if ((EditForm->texture == t) && (EditForm->image == img))
          {
            has_edit = true;
            EditForm->BringToFront();
            if (textures[t].image[img].bits <= 8)
            {
                MainForm->PaletteTable->PageIndex = 1;
            } else MainForm->PaletteTable->PageIndex = 0;
          }
      }
    }

    if (textures[t].image[img].bits > 8)
    {
       MainForm->PaletteTable->PageIndex = 0;
       MainForm->SetPaletteDefault();
    } else MainForm->PaletteTable->PageIndex = 1;

    if (!has_edit)
    {
      int bitsize=3;

      if ((textures[t].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[t].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
      {
      if ((textures[t].image[img].bits==16)||(textures[t].image[img].bits==24)||(textures[t].image[img].bits==32))
      {
      bitsize = 4;
      } else if ((textures[t].image[img].bits==1)||(textures[t].image[img].bits==2)||(textures[t].image[img].bits==4)||(textures[t].image[img].bits==8))
      {
      bitsize = 1;
      }
      } else if ((textures[t].format==FORMAT_TEXTURE_TILES_RGB)||(textures[t].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
      {
      if ((textures[t].image[img].bits==16)||(textures[t].image[img].bits==24)||(textures[t].image[img].bits==32))
      {
      bitsize = 3;
      } else if ((textures[t].image[img].bits==1)||(textures[t].image[img].bits==2)||(textures[t].image[img].bits==4)||(textures[t].image[img].bits==8))
      {
      bitsize = 1;
      }
      }

      EditForm = new TEditForm(t,img,bitsize);
      EditForm->Caption = AnsiString(textures[t].image[img].name);
      EditForm->bits = textures[t].image[img].bits;
      int w = textures[t].image[img].width;
      int h = textures[t].image[img].height;
      EditForm->Image->Bitmap = new TBitmap32();
      EditForm->Image->Bitmap->DrawMode = dmBlend;
      EditForm->Image->Bitmap->MasterAlpha = 255;
      EditForm->Image->Bitmap->Width = w;
      EditForm->Image->Bitmap->Height = h;

      if (textures[t].image[img].bits <= 8)
      {
         EditForm->Bitmap8 = new Graphics::TBitmap();
         EditForm->Bitmap8->Width = w;
         EditForm->Bitmap8->Width = h;
      }
      else
      {
         MainForm->SetPaletteDefault();
      }

      int c = 0;
      int index=0;

      TBitmap32 *bmp = EditForm->Image->Bitmap;

      for (int y=0;y<h;y++)
      {
        for (int x=0;x<w;x++)
        {
          if (bitsize == 4)
          {
            BYTE r=textures[t].image[img].data[index + 0];
            BYTE g=textures[t].image[img].data[index + 1];
            BYTE b=textures[t].image[img].data[index + 2];
            BYTE a=textures[t].image[img].data[index + 3];
            c = Color32(r,g,b,a);
            EditForm->setPixel(bmp,x,y,c);
          }
          else if ((bitsize == 2)||(bitsize == 3))
          {
            BYTE r=textures[t].image[img].data[index + 0];
            BYTE g=textures[t].image[img].data[index + 1];
            BYTE b=textures[t].image[img].data[index + 2];
            c = Color32(r,g,b,255);
            EditForm->setPixel(bmp,x,y,c);
          }
          else
          {
            BYTE p=(BYTE)textures[t].image[img].data[index];
            BYTE r=(BYTE)textures[t].image[img].palette[p][0];
            BYTE g=(BYTE)textures[t].image[img].palette[p][1];
            BYTE b=(BYTE)textures[t].image[img].palette[p][2];
            c = Color32(r,g,b,255);
            EditForm->setPixel(bmp,x,y,c);
          }
          index += bitsize;
        }
      }
      EditForm->CreateDrawingLayer();

      if (textures[t].image[img].bits <= 8)
      {
          MainForm->PaletteTable->PageIndex = 1;
      } else MainForm->PaletteTable->PageIndex = 0;

    }

    }
    catch(...)
    {
      MainForm->EndProgressWait(progressform);
    }

    MainForm->EndProgressWait(progressform);

    MainForm->EditMenuUpdate();
}
//---------------------------------------------------------------------------


void __fastcall TMDIChild::FormActivate(TObject *Sender)
{
curtexture = Tag;
MainForm->EditMenuUpdate();
}
//---------------------------------------------------------------------------




void __fastcall TMDIChild::AddWPClick(TObject *Sender)
{
int i,j,x,y,w,h,bitsize=0;
int cnt = 0;
long index=0;
unsigned char* ScanLine;
unsigned char* AScanLine;
Graphics::TBitmap* bitmap = new Graphics::TBitmap;
TPNGObject* png = new TPNGObject;
TJPEGImage* jpeg = new TJPEGImage();
if (OpenPictureDialog1->Execute())
{
TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, Adding Image...");

try
{

for (i=0;i<OpenPictureDialog1->Files->Count;i++)
{
Modified = true;
Empty = false;
cnt = textures[curtexture].numimages;
if (ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".BMP")
{
bitmap->LoadFromFile(OpenPictureDialog1->Files->Strings[i]);
OptimizeBitmap(bitmap);
strcpy(textures[curtexture].image[cnt].name,ExtractFileBase(ExtractFileName(OpenPictureDialog1->Files->Strings[i])).c_str());
textures[curtexture].image[cnt].animation = 0;
textures[curtexture].image[cnt].bits = 8;
textures[curtexture].image[cnt].backgroundindex = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].background[0] = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].width = bitmap->Width;
textures[curtexture].image[cnt].height = bitmap->Height;

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;

  RGBQUAD tile_palette[256];
  GetDIBColorTable(bitmap->Canvas->Handle,0,256,tile_palette);
  for (int q=0;q<256;q++)
  {
    textures[curtexture].image[cnt].palette[q][0] = tile_palette[q].rgbRed;
    textures[curtexture].image[cnt].palette[q][1] = tile_palette[q].rgbGreen;
    textures[curtexture].image[cnt].palette[q][2] = tile_palette[q].rgbBlue;
  }

if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}

textures[curtexture].image[cnt].data = (char*)malloc(w*h*bitsize);


index=0;

for( y = 0; y < h; y ++ )
{
ScanLine = (unsigned char*)bitmap->ScanLine[y];
for( x = 0; x < w; x ++ )
{
  if (bitsize >= 2)
  {
    textures[curtexture].image[cnt].data[index + 0] = ScanLine[x * 3 + 2];
    textures[curtexture].image[cnt].data[index + 1] = ScanLine[x * 3 + 1];
    textures[curtexture].image[cnt].data[index + 2] = ScanLine[x * 3 + 0];
    if (bitsize == 4)
    {
     textures[curtexture].image[cnt].data[index + 3] = 255;
    }
  }
  else
  {
    textures[curtexture].image[cnt].data[index] = ScanLine[x];
  }
  index += bitsize;
}
}
}
else
if (ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".PNG")
{
png->LoadFromFile(OpenPictureDialog1->Files->Strings[i]);
strcpy(textures[curtexture].image[cnt].name,ExtractFileBase(ExtractFileName(OpenPictureDialog1->Files->Strings[i])).c_str());
textures[curtexture].image[cnt].animation = 0;
textures[curtexture].image[cnt].bits = 8;
textures[curtexture].image[cnt].backgroundindex = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].background[0] = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].width = png->Width;
textures[curtexture].image[cnt].height = png->Height;

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
//if (png->Header->ColorType != COLOR_RGBALPHA) bitsize = 3;
//else bitsize = 4;
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}


textures[curtexture].image[cnt].data = (unsigned char*)malloc(w*h*bitsize);

Graphics::TBitmap *pngbitmap = new Graphics::TBitmap();
pngbitmap->Assign(png);
OptimizeBitmap(pngbitmap);

  RGBQUAD tile_palette[256];
  GetDIBColorTable(pngbitmap->Canvas->Handle,0,256,tile_palette);
  for (int q=0;q<256;q++)
  {
    textures[curtexture].image[cnt].palette[q][0] = tile_palette[q].rgbRed;
    textures[curtexture].image[cnt].palette[q][1] = tile_palette[q].rgbGreen;
    textures[curtexture].image[cnt].palette[q][2] = tile_palette[q].rgbBlue;
  }

index=0;

for( y = 0; y < h; y ++ )
{
ScanLine = (unsigned char*)pngbitmap->ScanLine[y];
for( x = 0; x < w; x ++ )
{
  if (bitsize >= 2)
  {
    textures[curtexture].image[cnt].data[index + 0] = ScanLine[x * 3 + 2];
    textures[curtexture].image[cnt].data[index + 1] = ScanLine[x * 3 + 1];
    textures[curtexture].image[cnt].data[index + 2] = ScanLine[x * 3 + 0];
  }
  else
  {
    textures[curtexture].image[cnt].data[index] = (BYTE)ScanLine[x];
  }
  index += bitsize;
}
}


delete pngbitmap;

}
else
if ((ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".JPG")||(ExtractFileExt(UpperCase(OpenPictureDialog1->Files->Strings[i]))==".JPEG"))
{
jpeg->LoadFromFile(OpenPictureDialog1->Files->Strings[i]);
strcpy(textures[curtexture].image[cnt].name,ExtractFileBase(ExtractFileName(OpenPictureDialog1->Files->Strings[i])).c_str());
textures[curtexture].image[cnt].animation = 0;
textures[curtexture].image[cnt].bits = 8;
textures[curtexture].image[cnt].backgroundindex = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].background[0] = 255;
textures[curtexture].image[cnt].background[0] = 0;
textures[curtexture].image[cnt].width = jpeg->Width;
textures[curtexture].image[cnt].height = jpeg->Height;

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}

textures[curtexture].image[cnt].data = (unsigned char*)malloc(w*h*bitsize);

Graphics::TBitmap *jpegbitmap = new Graphics::TBitmap();
jpegbitmap->Assign(jpeg);

OptimizeBitmap(jpegbitmap);

  RGBQUAD tile_palette[256];
  GetDIBColorTable(jpegbitmap->Canvas->Handle,0,256,tile_palette);
  for (int q=0;q<256;q++)
  {
    textures[curtexture].image[cnt].palette[q][0] = tile_palette[q].rgbRed;
    textures[curtexture].image[cnt].palette[q][1] = tile_palette[q].rgbGreen;
    textures[curtexture].image[cnt].palette[q][2] = tile_palette[q].rgbBlue;
  }

index=0;

for( y = 0; y < h; y ++ )
{
ScanLine = (unsigned char*)jpegbitmap->ScanLine[y];
for( x = 0; x < w; x ++ )
{
  if (bitsize >= 2)
  {
    textures[curtexture].image[cnt].data[index + 0] = ScanLine[x * 3 + 2];
    textures[curtexture].image[cnt].data[index + 1] = ScanLine[x * 3 + 1];
    textures[curtexture].image[cnt].data[index + 2] = ScanLine[x * 3 + 0];
    if (bitsize == 4)
    {
     textures[curtexture].image[cnt].data[index + 3] = 255;
    }
  }
  else
  {
    textures[curtexture].image[cnt].data[index] = (BYTE)ScanLine[x];
  }
  index += bitsize;
}
}

delete jpegbitmap;

}


TListItem* item = ListView1->Items->Add();
item->Caption = textures[curtexture].image[textures[curtexture].numimages].name;
item->SubItems->Add(IntToStr(textures[curtexture].numimages));
item->SubItems->Add(IntToStr(textures[curtexture].image[textures[curtexture].numimages].width)+"x"+IntToStr(textures[curtexture].image[textures[curtexture].numimages].height));

textures[curtexture].numimages++;
}


}
catch(...)
{
MainForm->EndProgressWait(progressform);
}

MainForm->EndProgressWait(progressform);

}
delete png;
delete jpeg;
delete bitmap;
}
//---------------------------------------------------------------------------

void __fastcall TMDIChild::Button4Click(TObject *Sender)
{
    if (!ListView1->Selected) return;

int cnt=StrToInt(ListView1->Selected->SubItems->Strings[0]);


int j,x,y,w,h,bitsize=0;
long index=0;
unsigned char* ScanLine;
unsigned char* AScanLine;
Graphics::TBitmap* bitmap = new Graphics::TBitmap;
TPNGObject* png = new TPNGObject;
TJPEGImage* jpeg = new TJPEGImage();

w = textures[curtexture].image[cnt].width;
h = textures[curtexture].image[cnt].height;


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 4;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[cnt].bits==16)||(textures[curtexture].image[cnt].bits==24)||(textures[curtexture].image[cnt].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[cnt].bits==1)||(textures[curtexture].image[cnt].bits==2)||(textures[curtexture].image[cnt].bits==4)||(textures[curtexture].image[cnt].bits==8))
{
bitsize = 1;
}
}
if (bitsize == 1) SavePictureDialog1->FilterIndex = 4;
else if (bitsize == 4) SavePictureDialog1->FilterIndex = 1;
else SavePictureDialog1->FilterIndex = 4;
SavePictureDialog1->FileName = AnsiString(ExtractFileBase(AnsiString(textures[curtexture].image[cnt].name)));
if (SavePictureDialog1->Execute())
{
TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, Exporting Image...");
try
{

AnsiString SFExt = GetFilterExtension(SavePictureDialog1->Filter,SavePictureDialog1->FilterIndex);
AnsiString SFileName = ChangeFileExt(ExtractFileBase(SavePictureDialog1->FileName),SFExt);
SavePictureDialog1->FileName = SFileName;
AnsiString ext = UpperCase(SFExt);


if (ext == ".PNG")
{
if (png) delete png;
png = new TPNGObject(COLOR_RGBALPHA, 16, w, h);
png->CreateAlpha();

long row, col,index=0;
int checker=0;
COLORREF i_color=0,c_color=0;
BYTE r,g,b,a,p;

index = 0;

if (bitsize == 4)
{
for( y = 0; y < h; y ++ )
{
BYTE* AScanLine = (BYTE*)png->AlphaScanline[y];
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  a = textures[curtexture].image[cnt].data[index + 3];
  i_color = RGB(r,g,b);
  png->Pixels[x][y] = TColor(i_color);
  AScanLine[x] = a;
  index += bitsize;
}
}
}
else if ((bitsize == 2)||(bitsize == 3))
{

for( y = 0; y < h; y ++ )
{
BYTE* AScanLine = (BYTE*)png->AlphaScanline[y];
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  png->Pixels[x][y] = TColor(RGB(r,g,b));
  AScanLine[x] = 255;
  index += bitsize;
}
}

}
else
{

for( y = 0; y < h; y ++ )
{
BYTE* AScanLine = (BYTE*)png->AlphaScanline[y];
for( x = 0; x < w; x ++ )
{
  p = (BYTE)textures[curtexture].image[cnt].data[index];
  r = (BYTE)textures[curtexture].image[cnt].palette[p][0];
  g = (BYTE)textures[curtexture].image[cnt].palette[p][1];
  b = (BYTE)textures[curtexture].image[cnt].palette[p][2];
  png->Pixels[x][y] = TColor(RGB(r,g,b));
  AScanLine[x] = 255;
  index += bitsize;
}
}

}
png->SaveToFile(SavePictureDialog1->FileName);

}
else if ((ext == ".JPG")||(ext == ".JPEG"))
{
long row, col,index=0;
int checker=0;
COLORREF i_color=0,c_color=0;
BYTE r,g,b,a,p;

bitmap->Width = w;
bitmap->Height = h;

index = 0;

if (bitsize == 4)
{
for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  a = (int)textures[curtexture].image[cnt].data[index + 3];
  i_color = RGB(r,g,b);
  bitmap->Canvas->Pixels[x][y] = TColor(i_color);
  index += bitsize;
}
}
}
else if ((bitsize == 2)||(bitsize == 3))
{

for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  bitmap->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
  index += bitsize;
}
}

}
else
{

/*
jpeg->PixelFormat = jf8Bit;
bitmap->PixelFormat = pf8bit;
RGBQUAD img_palette[256];
TPaletteEntry palette_entry[256];
TLogPalette log_palette;
log_palette.palVersion = 0x300;
log_palette.palNumEntries = 1;
for (int i=0;i<256;i++)
{
    img_palette[i].rgbRed = (BYTE)textures[curtexture].image[cnt].palette[i][0];
    img_palette[i].rgbGreen = (BYTE)textures[curtexture].image[cnt].palette[i][1];
    img_palette[i].rgbBlue = (BYTE)textures[curtexture].image[cnt].palette[i][2];

    palette_entry[i].peRed = (BYTE)textures[curtexture].image[cnt].palette[i][0];
    palette_entry[i].peGreen = (BYTE)textures[curtexture].image[cnt].palette[i][1];
    palette_entry[i].peBlue = (BYTE)textures[curtexture].image[cnt].palette[i][2];
    palette_entry[i].peFlags = 0;
}
SetDIBColorTable(bitmap->Canvas->Handle, 0, 256, img_palette);
jpeg->Palette = CreatePalette(&log_palette);
ResizePalette(jpeg->Palette, 256);
SetPaletteEntries(jpeg->Palette, 0, 256, palette_entry);
*/

for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  p = (BYTE)textures[curtexture].image[cnt].data[index];
  r = (BYTE)textures[curtexture].image[cnt].palette[p][0];
  g = (BYTE)textures[curtexture].image[cnt].palette[p][1];
  b = (BYTE)textures[curtexture].image[cnt].palette[p][2];
  bitmap->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
  index += bitsize;
}
}

}
jpeg->Assign(bitmap);

jpeg->SaveToFile(SavePictureDialog1->FileName);
}
else if (ext == ".BMP")
{
long row, col,index=0;
int checker=0;
COLORREF i_color=0,c_color=0;
BYTE r,g,b,a,p;

bitmap->Width = w;
bitmap->Height = h;

index = 0;

if (bitsize == 4)
{
for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  a = (int)textures[curtexture].image[cnt].data[index + 3];
  i_color = RGB(r,g,b);
  bitmap->Canvas->Pixels[x][y] = TColor(i_color);
  index += bitsize;
}
}
}
else if ((bitsize == 2)||(bitsize == 3))
{

for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  r = textures[curtexture].image[cnt].data[index + 0];
  g = textures[curtexture].image[cnt].data[index + 1];
  b = textures[curtexture].image[cnt].data[index + 2];
  bitmap->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
  index += bitsize;
}
}

}
else
{
bitmap->PixelFormat = pf8bit;
RGBQUAD img_palette[256];
for (int i=0;i<256;i++)
{
    img_palette[i].rgbRed = (BYTE)textures[curtexture].image[cnt].palette[i][0];
    img_palette[i].rgbGreen = (BYTE)textures[curtexture].image[cnt].palette[i][1];
    img_palette[i].rgbBlue = (BYTE)textures[curtexture].image[cnt].palette[i][2];
}
SetDIBColorTable(bitmap->Canvas->Handle, 0, 256, img_palette);
for( y = 0; y < h; y ++ )
{
for( x = 0; x < w; x ++ )
{
  p = (BYTE)textures[curtexture].image[cnt].data[index];
  r = (BYTE)textures[curtexture].image[cnt].palette[p][0];
  g = (BYTE)textures[curtexture].image[cnt].palette[p][1];
  b = (BYTE)textures[curtexture].image[cnt].palette[p][2];
  bitmap->Canvas->Pixels[x][y] = TColor(RGB(r,g,b));
  index += bitsize;
}
}

}

bitmap->SaveToFile(SavePictureDialog1->FileName);
}


}
catch(...)
{
MainForm->EndProgressWait(progressform);
}

MainForm->EndProgressWait(progressform);

}
delete png;
delete jpeg;
delete bitmap;

}
//---------------------------------------------------------------------------
AnsiString __fastcall TMDIChild::GetFilterExtension(AnsiString Filter, int FilterIndex)
{
  AnsiString SFilterText = StringReplace(Filter,"|","\r\n",TReplaceFlags() << rfReplaceAll);
  TStringList *SFilter = new TStringList();
  SFilter->Text = SFilterText;
  int SFI = ((FilterIndex-1)*2)+1;
  AnsiString SFExt = LowerCase(ExtractFileExt(SFilter->Strings[SFI]));
  delete SFilter;
  return SFExt;
}
//---------------------------------------------------------------------------
void __fastcall TMDIChild::bgcolorMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
  if (!ListView1->Selected) return;
  int t=Tag;
  curtexture=t;
  int img=StrToInt(ListView1->Selected->SubItems->Strings[0]);
  if (textures[curtexture].image[img].bits > 8)
  {
    ColorDialog1->Color = bgcolor->Brush->Color;
    if (ColorDialog1->Execute())
    {
      bgcolor->Brush->Color = ColorDialog1->Color;
      BYTE bg_r = GetRValue(bgcolor->Brush->Color);
      BYTE bg_g = GetGValue(bgcolor->Brush->Color);
      BYTE bg_b = GetBValue(bgcolor->Brush->Color);
      textures[curtexture].image[img].background[0] = bg_r;
      textures[curtexture].image[img].background[1] = bg_g;
      textures[curtexture].image[img].background[2] = bg_b;
      textures[curtexture].image[img].background[3] = 255;
    }
  }
  else
  {
    BYTE bg_c = textures[curtexture].image[img].backgroundindex;
    BGColorDlg->DrawGrid1->Col = bg_c/8;
    BGColorDlg->DrawGrid1->Row = bg_c/32;
    for (int i=0;i<256;i++)
    {
        BGColorDlg->color_palette[i][0] = (BYTE)textures[curtexture].image[img].palette[i][0];
        BGColorDlg->color_palette[i][1] = (BYTE)textures[curtexture].image[img].palette[i][1];
        BGColorDlg->color_palette[i][2] = (BYTE)textures[curtexture].image[img].palette[i][2];
    }
    if (BGColorDlg->ShowModal() == mrOk)
    {
       BYTE bg_index = paletteoffset(BGColorDlg->DrawGrid1->Col,BGColorDlg->DrawGrid1->Row,32);
       textures[curtexture].image[img].backgroundindex = bg_index;
       BYTE bg_r = textures[curtexture].image[img].palette[bg_index][0];
       BYTE bg_g = textures[curtexture].image[img].palette[bg_index][1];
       BYTE bg_b = textures[curtexture].image[img].palette[bg_index][2];
       bgcolor->Brush->Color = TColor(RGB(bg_r,bg_g,bg_b));
    }
  }
}
//---------------------------------------------------------------------------

