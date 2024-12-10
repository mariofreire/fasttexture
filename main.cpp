//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <assert.h>
#include "zlib.h"
                      
#include "progressunit.h"
#include "Main.h"
#include "About.h"
#include "newdlg.h"   
#include "brightnessdlg.h"    
#include "infodlg.h"
//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TMainForm *MainForm;
//---------------------------------------------------------------------------
#define PATHSEPARATOR(c) ((c) == '\\' || (c) == '/')

BYTE palette[256][3];
char britable[16][256];
int numtextures=0;
texture_t textures[MAXTEXTURES];

const unsigned char Def_RGB[] = {
				0x00, 0x00, 0x00, 0x80, 0x80, 0x80, 0x80, 0x00,
				0x00, 0xFF, 0x00, 0x00, 0x00, 0x80, 0x00, 0x00,
				0xFF, 0x00, 0x80, 0x80, 0x00, 0xFF, 0xFF, 0x00,
				0x00, 0x00, 0x80, 0x00, 0x00, 0xFF, 0x80, 0x00,
				0x80, 0xFF, 0x00, 0xFF, 0x00, 0x80, 0x80, 0x00,
				0xFF, 0xFF, 0xC0, 0xC0, 0xC0, 0xFF, 0xFF, 0xFF,
				0x60, 0x60, 0x60, 0xE0, 0xE0, 0xE0, 0x80, 0x40,
				0x00, 0xFF, 0x80, 0x00, 0x00, 0x60, 0x20, 0x00,
				0xC0, 0x40, 0xC0, 0x80, 0x00, 0xFF, 0xFF, 0x80,
				0x00, 0x40, 0x80, 0x00, 0x80, 0xFF, 0x60, 0x00,
				0xC0, 0xE0, 0x00, 0x80, 0x00, 0x60, 0x80, 0x00,
				0xC0, 0xFF, 0xD8, 0xD8, 0xD8, 0xF8, 0xF8, 0xF8,
				0x40, 0x00, 0x00, 0xFF, 0x80, 0x40, 0x00, 0x40,
				0x40, 0x00, 0x80, 0x40, 0x80, 0x40, 0x40, 0x80,
				0x80, 0x40, 0x80, 0xFF, 0x00, 0x80, 0xFF, 0x80,
				0x00, 0xFF, 0x80, 0x80, 0xFF, 0xFF, 0x00, 0x00,
				0xA0, 0x80, 0x80, 0xFF, 0x40, 0x00, 0x80, 0x80,
				0x00, 0xFF, 0x00, 0x00, 0x40, 0x00, 0x80, 0xC0,
				0x00, 0x10, 0xE0, 0x20, 0xA4, 0xFF, 0xC0, 0x80,
				0x00, 0xFF, 0xC0, 0x00, 0x94, 0x18, 0x00, 0xFF,
				0x40, 0x10, 0x20, 0x60, 0x00, 0x00, 0xC0, 0x00,
				0xFF, 0xE8, 0x60, 0xFF, 0xF8, 0xC0, 0x40, 0xE0,
				0xFF, 0xC0, 0xFF, 0xFF, 0xF8, 0xA4, 0xF8, 0xFF,
				0xE0, 0xFF, 0xE8, 0xE8, 0xE8, 0xFC, 0xFC, 0xFC,
				0x00, 0x30, 0xFF, 0x10, 0x3D, 0xFF, 0x21, 0x4B,
				0xFF, 0x31, 0x58, 0xFF, 0x41, 0x65, 0xFF, 0x52,
				0x72, 0xFF, 0x62, 0x80, 0xFF, 0x72, 0x8D, 0xFF,
				0x83, 0x9A, 0xFF, 0x93, 0xA7, 0xFF, 0xA3, 0xB5,
				0xFF, 0xB4, 0xC2, 0xFF, 0xC4, 0xCF, 0xFF, 0xD4,
				0xDC, 0xFF, 0xE5, 0xEA, 0xFF, 0xF5, 0xF7, 0xFF,
				0xFF, 0x18, 0x18, 0xFF, 0x27, 0x27, 0xFF, 0x36,
				0x36, 0xFF, 0x45, 0x45, 0xFF, 0x54, 0x54, 0xFF,
				0x63, 0x63, 0xFF, 0x72, 0x72, 0xFF, 0x81, 0x81,
				0xFF, 0x91, 0x91, 0xFF, 0xA0, 0xA0, 0xFF, 0xAF,
				0xAF, 0xFF, 0xBE, 0xBE, 0xFF, 0xCD, 0xCD, 0xFF,
				0xDC, 0xDC, 0xFF, 0xEB, 0xEB, 0xFF, 0xFA, 0xFA,
				0x18, 0xFF, 0x18, 0x27, 0xFF, 0x27, 0x35, 0xFF,
				0x35, 0x44, 0xFF, 0x44, 0x53, 0xFF, 0x53, 0x62,
				0xFF, 0x62, 0x70, 0xFF, 0x70, 0x7F, 0xFF, 0x7F,
				0x8E, 0xFF, 0x8E, 0x9D, 0xFF, 0x9D, 0xAB, 0xFF,
				0xAB, 0xBA, 0xFF, 0xBA, 0xC9, 0xFF, 0xC9, 0xD8,
				0xFF, 0xD8, 0xE6, 0xFF, 0xE6, 0xF5, 0xFF, 0xF5,
				0xFF, 0xFF, 0x18, 0xFF, 0xFF, 0x27, 0xFF, 0xFF,
				0x35, 0xFF, 0xFF, 0x44, 0xFF, 0xFF, 0x53, 0xFF,
				0xFF, 0x62, 0xFF, 0xFF, 0x70, 0xFF, 0xFF, 0x7F,
				0xFF, 0xFF, 0x8E, 0xFF, 0xFF, 0x9D, 0xFF, 0xFF,
				0xAB, 0xFF, 0xFF, 0xBA, 0xFF, 0xFF, 0xC9, 0xFF,
				0xFF, 0xD8, 0xFF, 0xFF, 0xE6, 0xFF, 0xFF, 0xF5,
				0xFF, 0x18, 0xFF, 0xFF, 0x27, 0xFF, 0xFF, 0x35,
				0xFF, 0xFF, 0x44, 0xFF, 0xFF, 0x53, 0xFF, 0xFF,
				0x62, 0xFF, 0xFF, 0x70, 0xFF, 0xFF, 0x7F, 0xFF,
				0xFF, 0x8E, 0xFF, 0xFF, 0x9D, 0xFF, 0xFF, 0xAB,
				0xFF, 0xFF, 0xBA, 0xFF, 0xFF, 0xC9, 0xFF, 0xFF,
				0xD8, 0xFF, 0xFF, 0xE6, 0xFF, 0xFF, 0xF5, 0xFF,
				0x04, 0x10, 0x60, 0x06, 0x19, 0x6B, 0x08, 0x22,
				0x75, 0x0A, 0x2A, 0x80, 0x0B, 0x33, 0x8A, 0x0D,
				0x3C, 0x95, 0x0F, 0x45, 0xA0, 0x11, 0x4E, 0xAA,
				0x13, 0x56, 0xB5, 0x15, 0x5F, 0xBF, 0x17, 0x68,
				0xCA, 0x19, 0x71, 0xD5, 0x1A, 0x7A, 0xDF, 0x1C,
				0x82, 0xEA, 0x1E, 0x8B, 0xF4, 0x20, 0x94, 0xFF,
				0x60, 0x08, 0x00, 0x6B, 0x0A, 0x00, 0x75, 0x0B,
				0x00, 0x80, 0x0D, 0x00, 0x8A, 0x0E, 0x00, 0x95,
				0x10, 0x00, 0xA0, 0x12, 0x00, 0xAA, 0x13, 0x00,
				0xB5, 0x15, 0x00, 0xBF, 0x16, 0x00, 0xCA, 0x18,
				0x00, 0xD5, 0x1A, 0x00, 0xDF, 0x1B, 0x00, 0xEA,
				0x1D, 0x00, 0xF4, 0x1E, 0x00, 0xFF, 0x20, 0x00,
				0x10, 0x60, 0x00, 0x0F, 0x69, 0x02, 0x0E, 0x71,
				0x04, 0x0D, 0x7A, 0x06, 0x0C, 0x82, 0x09, 0x0B,
				0x8B, 0x0B, 0x0A, 0x93, 0x0D, 0x09, 0x9C, 0x0F,
				0x07, 0xA4, 0x11, 0x06, 0xAD, 0x13, 0x05, 0xB5,
				0x15, 0x04, 0xBE, 0x17, 0x03, 0xC6, 0x1A, 0x02,
				0xCF, 0x1C, 0x01, 0xD7, 0x1E, 0x00, 0xE0, 0x20,
				0x60, 0x30, 0x00, 0x6B, 0x37, 0x02, 0x75, 0x3D,
				0x04, 0x80, 0x44, 0x06, 0x8A, 0x4B, 0x09, 0x95,
				0x51, 0x0B, 0xA0, 0x58, 0x0D, 0xAA, 0x5F, 0x0F,
				0xB5, 0x65, 0x11, 0xBF, 0x6C, 0x13, 0xCA, 0x73,
				0x15, 0xD5, 0x79, 0x17, 0xDF, 0x80, 0x1A, 0xEA,
				0x87, 0x1C, 0xF4, 0x8D, 0x1E, 0xFF, 0x94, 0x20,
				0x60, 0x08, 0x60, 0x6A, 0x07, 0x6A, 0x74, 0x07,
				0x74, 0x7E, 0x06, 0x7E, 0x89, 0x06, 0x89, 0x93,
				0x05, 0x93, 0x9D, 0x05, 0x9D, 0xA7, 0x04, 0xA7,
				0xB1, 0x04, 0xB1, 0xBB, 0x03, 0xBB, 0xC5, 0x03,
				0xC5, 0xCF, 0x02, 0xCF, 0xDA, 0x02, 0xDA, 0xE4,
				0x01, 0xE4, 0xEE, 0x01, 0xEE, 0xF8, 0x00, 0xF8,
				0xFF, 0xA4, 0x08, 0xFF, 0xAA, 0x0F, 0xFF, 0xB0,
				0x16, 0xFF, 0xB6, 0x1E, 0xFF, 0xBC, 0x25, 0xFF,
				0xC2, 0x2C, 0xFF, 0xC8, 0x33, 0xFF, 0xCE, 0x3A,
				0xFF, 0xD5, 0x42, 0xFF, 0xDB, 0x49, 0xFF, 0xE1,
				0x50, 0xFF, 0xE7, 0x57, 0xFF, 0xED, 0x5E, 0xFF,
				0xF3, 0x66, 0xFF, 0xF9, 0x6D, 0xFF, 0xFF, 0x74,
				0x18, 0x18, 0x18, 0x26, 0x26, 0x26, 0x35, 0x35,
				0x35, 0x43, 0x43, 0x43, 0x52, 0x52, 0x52, 0x60,
				0x60, 0x60, 0x6E, 0x6E, 0x6E, 0x7D, 0x7D, 0x7D,
				0x8B, 0x8B, 0x8B, 0x9A, 0x9A, 0x9A, 0xA8, 0xA8,
				0xA8, 0xB6, 0xB6, 0xB6, 0xC5, 0xC5, 0xC5, 0xD3,
				0xD3, 0xD3, 0xE2, 0xE2, 0xE2, 0xF0, 0xF0, 0xF0};

//---------------------------------------------------------------------------
bool clp_mm=false;
TMouseButton clp_mb;
//---------------------------------------------------------------------------

typedef void (__stdcall *OptimizeBitmapF)(Graphics::TBitmap *Bitmap);
OptimizeBitmapF OptimizeBitmap;
HINSTANCE hinstimglibDLL;
const char imglib_dll[] = {"imagelib.dll"};
void loadimglib()
{
     if ((hinstimglibDLL=LoadLibrary(imglib_dll))) {
        OptimizeBitmap = (OptimizeBitmapF)GetProcAddress(hinstimglibDLL, "OptimizeBitmap");
     }
     else
     {
       MessageBox(Application->Handle,AnsiString().sprintf("The Program can't start because %s is missing from your computer. Try reinstalling the program to fix this problem.",strupr((char*)imglib_dll)).c_str(),AnsiString("Error").c_str(),MB_OK|MB_ICONERROR);
       Application->Terminate();
       exit(0);
     }
}
//---------------------------------------------------------------------------
void unloadimglib()
{
    FreeLibrary(hinstimglibDLL);
}
//---------------------------------------------------------------------------
void calcbritable(void)
{
    double a,b;
    for (long i=0;i<16;i++)
    {
        a = (double)8 / ((double)i+8);
        b = (double)255 / pow((double)255,a);
        for (long j=0;j<256;j++)
            britable[i][j] = (unsigned char)(pow((double)j,a)*b);
    }
}
//---------------------------------------------------------------------------
__fastcall TMainForm::TMainForm(TComponent *Owner)
	: TForm(Owner)
{
int i,j;
for(i=0;i<MAXTEXTURES;i++)
{
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
textures[i].numimages = 0;
for(j=0;j<MAXIMAGES;j++)
{
 strcpy(textures[i].image[j].name,"");
 textures[i].image[j].animation = 0;
 textures[i].image[j].bits = 32;
 textures[i].image[j].backgroundindex = 0;
 textures[i].image[j].width = 0;
 textures[i].image[j].height = 0;
}
}
   
mdi_child_icon_on = false;

SetPaletteDefault();
calcbritable();

}
//---------------------------------------------------------------------------
bool __fastcall in_area(int mx,int my,int x,int y,int w,int h)
{
if (((mx >= x)&&(mx <= x+w))&&((my >= y)&&(my <= y+h))) return true;
return false;
}
//---------------------------------------------------------------------------
void zCompress(const char *szMessage, unsigned char *szOut) {
	unsigned long long nBuffer = 0;
	char nBitsInBuffer = 0;
	while (*szMessage != 0) {
		nBuffer |= (unsigned long long)(*szMessage++ & 0x7F) << nBitsInBuffer;
		nBitsInBuffer += 7;
		if (nBitsInBuffer == 7 * 8) {
			while (nBitsInBuffer > 0) {
				*szOut++ = nBuffer & 0xFF;
				nBuffer >>= 8;
				nBitsInBuffer -= 8;
			}
			nBitsInBuffer = 0;
			nBuffer = 0;
		}
	}
	while (nBitsInBuffer > 0) {
		*szOut++ = nBuffer & 0xFF;
		nBuffer >>= 8;
		nBitsInBuffer -= 8;
	}
}
 
void zUncompress(char *szOut,unsigned long *nUncompressedSize, const unsigned char *szCompressed, unsigned nCompressedLen) {
	unsigned long long nBuffer = 0;
	char nBitsInBuffer = 0;
	while (nCompressedLen) {
		while (nCompressedLen && nBitsInBuffer < 7 * 8) {
			nBuffer |= (unsigned long long)*szCompressed++ << nBitsInBuffer;
			nBitsInBuffer += 8;
			--nCompressedLen;
		}
		while (nBitsInBuffer > 0) {
			*szOut++ = nBuffer & 0x7F;
   *nUncompressedSize++;
			nBuffer >>= 7;
			nBitsInBuffer -= 7;
		}
		nBitsInBuffer = 0;
		nBuffer = 0;
	}
}



int compress(const unsigned char* &inbuf,unsigned long insize,unsigned char* &outbuf,unsigned long* outsize)
{
if (outbuf)
{
  free(outbuf);
  outbuf = NULL;
}
long siz=Z_CHECKLENGTH;
long bufferSize=insize;
outbuf = (unsigned char*)malloc(Z_CHECKLENGTH);
int r = zcompress(outbuf,&siz,inbuf,bufferSize);
if (r == Z_BUF_ERROR_)
{
  if (outbuf)
  {
    free(outbuf);
    outbuf = NULL;
    siz = 0;
  }
  siz = bufferSize;
  outbuf = (unsigned char*)malloc(siz);
  r = zcompress(outbuf,&siz,inbuf,bufferSize);
}
*outsize = siz;
if (r == Z_STREAM_ERROR_) return r;
if (r == Z_DATA_ERROR_) return r;
if (r == Z_MEM_ERROR_) return r;
if (r == Z_BUF_ERROR_) return r;
return r;
}
int uncompress(const unsigned char* &inbuf,unsigned long insize,unsigned char* &outbuf,unsigned long* outsize)
{
 if (outbuf)
 {
    free(outbuf);
    outbuf = NULL;
 }
 long siz=Z_CHECKLENGTH;
 long bufferSize=insize;
 outbuf = (unsigned char*)malloc(Z_CHECKLENGTH);
	int r = zuncompress(outbuf,&siz,inbuf,bufferSize);
	if (r == Z_BUF_ERROR_)
	{
  if (outbuf)
  {
    free(outbuf);
    outbuf = NULL;
    outbuf = (unsigned char*)malloc(siz);
  		r = zuncompress(outbuf,&siz,inbuf,bufferSize);
  }
  else
  {
    r = Z_BUF_ERROR_;
    return r;
  }
	}
if (r == Z_STREAM_ERROR_) return r;
if (r == Z_DATA_ERROR_) return r;
if (r == Z_MEM_ERROR_) return r;
if (r == Z_BUF_ERROR_) return r;
*outsize = siz;
return r;
}




int loadtexture(char *filename)
{
FILE *fp;
int i=0,j=0;
char *buf = NULL;
char *cbuf = NULL;
long csiz = 0;
char *dbuf = NULL;
long dsiz = 0;
unsigned long siz=0;

fp = fopen(filename,"rb");
if (!fp) return 0;

fread(&textures[numtextures].ptr[0],4,1,fp);
if ((textures[numtextures].ptr[0]!='F')&&(textures[numtextures].ptr[1]!='T')&&(textures[numtextures].ptr[2]!='E')&&(textures[numtextures].ptr[3]!='X'))
{
  MessageBox(0,"Invalid file format.","Error",MB_OK|MB_ICONERROR);
  fclose(fp);
  return 0;
}
fread(&textures[numtextures].version,2,1,fp);
if (textures[numtextures].version!=1)
{
  MessageBox(0,"Invalid FastTexture file version.","Error",MB_OK|MB_ICONERROR);
  fclose(fp);
  return 0;
}
fread(&textures[numtextures].format,4,1,fp);
if (!((textures[numtextures].format==FORMAT_TEXTURE_TILES_RGB)||(textures[numtextures].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB)||(textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA)))
{
  MessageBox(0,"Invalid FastTexture file format.","Error",MB_OK|MB_ICONERROR);
  fclose(fp);
  return 0;
}
fread(&textures[numtextures].name[0],256,1,fp);
fread(&textures[numtextures].description[0],256,1,fp);
fread(&textures[numtextures].comments[0],256,1,fp);
fread(&textures[numtextures].company[0],64,1,fp);
fread(&textures[numtextures].copyright[0],64,1,fp);
fread(&textures[numtextures].numimages,4,1,fp);

for (i=0;i<textures[numtextures].numimages;i++)
{
fread(&textures[numtextures].image[i].name,256,1,fp);
fread(&textures[numtextures].image[i].animation,4,1,fp);
fread(&textures[numtextures].image[i].bits,4,1,fp);
if ((textures[numtextures].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[numtextures].image[i].bits==4)||(textures[numtextures].image[i].bits==8))
{
for (j=0;j<256;j++)
{
fread(&textures[numtextures].image[i].palette[j][0],2,1,fp);
fread(&textures[numtextures].image[i].palette[j][1],2,1,fp);
fread(&textures[numtextures].image[i].palette[j][2],2,1,fp);
}
fread(&textures[numtextures].image[i].backgroundindex,4,1,fp);
fread(&textures[numtextures].image[i].width,4,1,fp);
fread(&textures[numtextures].image[i].height,4,1,fp);
siz = textures[numtextures].image[i].width*textures[numtextures].image[i].height;
textures[numtextures].image[i].data = (char*)malloc(siz);
fread(&textures[numtextures].image[i].data[0],siz,1,fp);
} else if ((textures[numtextures].image[i].bits==1)||(textures[numtextures].image[i].bits==2))
{
fread(&textures[numtextures].image[i].width,4,1,fp);
fread(&textures[numtextures].image[i].height,4,1,fp);
siz = textures[numtextures].image[i].width*textures[numtextures].image[i].height;
textures[numtextures].image[i].data = (char*)malloc(siz);
fread(&textures[numtextures].image[i].data[0],siz,1,fp);
} else if ((textures[numtextures].image[i].bits==16)||(textures[numtextures].image[i].bits==24)||(textures[numtextures].image[i].bits==32))
{

fread(&textures[numtextures].image[i].background[0],4,1,fp);

if (textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA) fread(&textures[numtextures].image[i].csize,4,1,fp);

fread(&textures[numtextures].image[i].width,4,1,fp);
fread(&textures[numtextures].image[i].height,4,1,fp);
siz = textures[numtextures].image[i].width*textures[numtextures].image[i].height*4;

//csiz = textures[numtextures].image[i].csize;//siz*sizeof(long);

//textures[numtextures].image[i].data = (char*)malloc(siz);
//buf = (char*)malloc(textures[curtexture].image[i].csize);
//cbuf  = (char*)calloc((long)csiz,1);
//fread(&cbuf[0],csiz,1,fp);
//cbuf  = (char*)calloc((long)csiz,1);

//dsiz = csiz;

//cbuf = buf;
//dbuf  = (char*)calloc((long)dsiz,1);
//inf(cbuf,dbuf,dsiz);

//textures[numtextures].image[i].data = dbuf;
if (textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA)
{
 unsigned long csize = textures[numtextures].image[i].csize;
 unsigned char *cdata = (unsigned char*)malloc(csize+65535);
 fread(&cdata[0],csize,1,fp);
 textures[numtextures].image[i].data = (char*)malloc(siz);
 uncompress((unsigned char *)cdata,csize,(unsigned char *)textures[numtextures].image[i].data,&siz);
 free(cdata);
}
else
{
textures[numtextures].image[i].data = (char*)malloc(siz);
fread(&textures[numtextures].image[i].data[0],siz,1,fp);
}
//free(cbuf);
//free(dbuf);
//free(buf);


//textures[numtextures].image[i].data = (char*)malloc(siz);
//fread(&textures[numtextures].image[i].data[0],siz,1,fp);
}
} else if ((textures[numtextures].format==FORMAT_TEXTURE_TILES_RGB)||(textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[numtextures].image[i].bits==4)||(textures[numtextures].image[i].bits==8))
{
for (j=0;j<256;j++)
{
fread(&textures[numtextures].image[i].palette[j][0],2,1,fp);
fread(&textures[numtextures].image[i].palette[j][1],2,1,fp);
fread(&textures[numtextures].image[i].palette[j][2],2,1,fp);
}
fread(&textures[numtextures].image[i].backgroundindex,4,1,fp);
fread(&textures[numtextures].image[i].width,4,1,fp);
fread(&textures[numtextures].image[i].height,4,1,fp);
siz = textures[numtextures].image[i].width*textures[numtextures].image[i].height;
textures[numtextures].image[i].data = (char*)malloc(siz);
fread(&textures[numtextures].image[i].data[0],siz,1,fp);
} else if ((textures[numtextures].image[i].bits==1)||(textures[numtextures].image[i].bits==2))
{
fread(&textures[numtextures].image[i].width,4,1,fp);
fread(&textures[numtextures].image[i].height,4,1,fp);
siz = textures[numtextures].image[i].width*textures[numtextures].image[i].height;
textures[numtextures].image[i].data = (char*)malloc(siz);
fread(&textures[numtextures].image[i].data[0],siz,1,fp);
} else if ((textures[numtextures].image[i].bits==16)||(textures[numtextures].image[i].bits==24)||(textures[numtextures].image[i].bits==32))
{
fread(&textures[numtextures].image[i].background[0],4,1,fp);

if (textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB) fread(&textures[numtextures].image[i].csize,4,1,fp);

fread(&textures[numtextures].image[i].width,4,1,fp);
fread(&textures[numtextures].image[i].height,4,1,fp);
siz = textures[numtextures].image[i].width*textures[numtextures].image[i].height*3;
if ((textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB)||(textures[numtextures].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
 int csize = textures[numtextures].image[i].csize;
 unsigned char *cdata = (unsigned char*)malloc(csize+65535);
 fread(&cdata[0],csize,1,fp);
 textures[numtextures].image[i].data = (char*)malloc(siz);
 uncompress((unsigned char *)cdata,csize,(unsigned char *)textures[numtextures].image[i].data,&siz);
 free(cdata);
}
else
{
textures[numtextures].image[i].data = (char*)malloc(siz);
fread(&textures[numtextures].image[i].data[0],siz,1,fp);
}
}
}
}
fclose(fp);

numtextures++;

return 1;
}

int savetexture(char* filename)
{
FILE* fp;
int i=0,j=0;
char *buf = NULL;
char *cbuf = NULL;
long csiz = 0;
char *dbuf = NULL;
int bitsize=0;
unsigned long siz=0;

fp = fopen(filename,"wb");

if (!fp) return 0;


fwrite(&textures[curtexture].ptr[0],4,1,fp);
fwrite(&textures[curtexture].version,2,1,fp);
fwrite(&textures[curtexture].format,4,1,fp);
fwrite(&textures[curtexture].name[0],256,1,fp);
fwrite(&textures[curtexture].description[0],256,1,fp);
fwrite(&textures[curtexture].comments[0],256,1,fp);
fwrite(&textures[curtexture].company[0],64,1,fp);
fwrite(&textures[curtexture].copyright[0],64,1,fp);
fwrite(&textures[curtexture].numimages,4,1,fp);

for (i=0;i<textures[curtexture].numimages;i++)
{
fwrite(&textures[curtexture].image[i].name,256,1,fp);
fwrite(&textures[curtexture].image[i].animation,4,1,fp);
fwrite(&textures[curtexture].image[i].bits,4,1,fp);


if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
if ((textures[curtexture].image[i].bits==16)||(textures[curtexture].image[i].bits==24)||(textures[curtexture].image[i].bits==32))
{
bitsize = 4;
} else if ((textures[curtexture].image[i].bits==1)||(textures[curtexture].image[i].bits==2)||(textures[curtexture].image[i].bits==4)||(textures[curtexture].image[i].bits==8))
{
bitsize = 1;
}
} else if ((textures[curtexture].format==FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB))
{
if ((textures[curtexture].image[i].bits==16)||(textures[curtexture].image[i].bits==24)||(textures[curtexture].image[i].bits==32))
{
bitsize = 3;
} else if ((textures[curtexture].image[i].bits==1)||(textures[curtexture].image[i].bits==2)||(textures[curtexture].image[i].bits==4)||(textures[curtexture].image[i].bits==8))
{
bitsize = 1;
}
}


if ((textures[curtexture].image[i].bits==4)||(textures[curtexture].image[i].bits==8))
{
for (j=0;j<256;j++)
{
fwrite(&textures[curtexture].image[i].palette[j][0],2,1,fp);
fwrite(&textures[curtexture].image[i].palette[j][1],2,1,fp);
fwrite(&textures[curtexture].image[i].palette[j][2],2,1,fp);
}
fwrite(&textures[curtexture].image[i].backgroundindex,4,1,fp);
fwrite(&textures[curtexture].image[i].width,4,1,fp);
fwrite(&textures[curtexture].image[i].height,4,1,fp);
siz = textures[curtexture].image[i].width*textures[curtexture].image[i].height;
fwrite(&textures[curtexture].image[i].data[0],siz,1,fp);
} else if ((textures[curtexture].image[i].bits==1)||(textures[curtexture].image[i].bits==2))
{
fwrite(&textures[curtexture].image[i].width,4,1,fp);
fwrite(&textures[curtexture].image[i].height,4,1,fp);
siz = textures[curtexture].image[i].width*textures[curtexture].image[i].height;
fwrite(&textures[curtexture].image[i].data[0],siz,1,fp);
} else if ((textures[curtexture].image[i].bits==16)||(textures[curtexture].image[i].bits==24)||(textures[curtexture].image[i].bits==32))
{
fwrite(&textures[curtexture].image[i].background[0],4,1,fp);

siz = textures[curtexture].image[i].width*textures[curtexture].image[i].height*bitsize;
//textures[curtexture].image[i].csize = siz*sizeof(long);
if ((textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGB)||(textures[curtexture].format==FORMAT_TEXTURE_COMPRESSED_TILES_RGBA))
{
 unsigned char * cdata = (char *)malloc(siz+65535);
 unsigned long csize=0;
 compress((unsigned char *)textures[curtexture].image[i].data, siz, (unsigned char *)cdata,&csize);
 textures[curtexture].image[i].csize = csize;
 fwrite(&textures[curtexture].image[i].csize,4,1,fp);
 fwrite(&textures[curtexture].image[i].width,4,1,fp);
 fwrite(&textures[curtexture].image[i].height,4,1,fp);
 fwrite(&cdata[0],csize,1,fp);
 free(cdata);
}
else
{
fwrite(&textures[curtexture].image[i].width,4,1,fp);
fwrite(&textures[curtexture].image[i].height,4,1,fp);

fwrite(&textures[curtexture].image[i].data[0],siz,1,fp);
}

//fwrite(&cbuf[0],csiz,1,fp);

//textures[curtexture].image[i].data = buf;

//free(cbuf);
//free(buf);
}
}
fclose(fp);

return 1;

}

//---------------------------------------------------------------------------
void ExtractFileBaseA(char *path, char *dest)
{
	char    *src;
	src = path + strlen(path) - 1;
	while (src != path && !PATHSEPARATOR(*(src-1)))
		src--;
	while (*src && *src != '.')
	{
		*dest++ = *src++;
	}
	*dest = 0;
}
//---------------------------------------------------------------------------
AnsiString ExtractFileBase (AnsiString FileName)
{
  char sfn[256];
  char dfn[256];
  strcpy(sfn,FileName.c_str());
  strcpy(dfn,"");
  ExtractFileBaseA(sfn,dfn);
  AnsiString dest = dfn;
  return dest;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::CreateMDIChild(bool New, String Name)
{
	TMDIChild *Child;
        int i=0;

	//--- create a new MDI child window ----
	Child = new TMDIChild(Application);
	Child->Caption = ExtractFileName(Name);
 Child->Tag = numtextures;
 Child->Modified = false;  
 Child->IsNew = New;

	if (FileExists (Name) && !New)
        {
   TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, loading textures...");
	  loadtexture(Name.c_str());
   curtexture = Child->Tag;
	 for (i=0;i<textures[curtexture].numimages;i++)
	 {
	   TListItem* item = Child->ListView1->Items->Add();
           item->Caption = textures[curtexture].image[i].name;
	   item->SubItems->Add(IntToStr(i));
           item->SubItems->Add(IntToStr(textures[curtexture].image[i].width)+"x"+IntToStr(textures[curtexture].image[i].height));
	 }
    MainForm->EndProgressWait(progressform);
        } else
        {
         curtexture = Child->Tag;
         numtextures++;
         Child->Empty = true;
        }
        
    if (MainForm->MDIChildCount>0)
    {
      MainForm->ToolBar1->Menu = NULL;
      MainForm->Texture1->Visible = true;
      MainForm->Image1->Visible = true;
      MainForm->NW->Visible = true;
      MainForm->ToolBar1->Menu = MainMenu1;
    }
    else
    {                                 
      MainForm->ToolBar1->Menu = NULL;
      MainForm->NW->Visible = false;
      MainForm->Texture1->Visible = false;  
      MainForm->Image1->Visible = false;
      MainForm->ToolBar1->Menu = MainMenu1;
    }
    
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileNew1Execute(TObject *Sender)
{

if (NewTextureDlg->ShowModal() == mrOk)
{
  switch (NewTextureDlg->RadioGroup1->ItemIndex)
  {
    case 0:
    {
      textures[numtextures].format = FORMAT_TEXTURE_TILES_RGB;
    }
    break;
    case 1:
    {
      textures[numtextures].format = FORMAT_TEXTURE_TILES_RGBA;
    }
    break;
    case 2:
    {
      textures[numtextures].format = FORMAT_TEXTURE_COMPRESSED_TILES_RGB;
    }
    break;
    case 3:
    {
      textures[numtextures].format = FORMAT_TEXTURE_COMPRESSED_TILES_RGBA;
    }
    break;
  };
  CreateMDIChild(true, "Untitled" + IntToStr(MDIChildCount + 1)+".ftx");
}

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileOpen1Execute(TObject *Sender)
{
	if (OpenDialog->Execute())
		CreateMDIChild(false, OpenDialog->FileName);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::HelpAbout1Execute(TObject *Sender)
{
	AboutBox->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileExit1Execute(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------



void __fastcall TMainForm::FileSave1Execute(TObject *Sender)
{
 if (MDIChildCount > 0)
 {
  TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, saving textures...");
  TMDIChild *Child = GetCurrentMDIChild();
  if (Child)
  {
    SaveDialog->FileName = Child->Caption;
    if (FileExists(SaveDialog->FileName) && !Child->IsNew)
    {
        if (savetexture(SaveDialog->FileName.c_str()))
        {
          Child->Caption = ExtractFileName(SaveDialog->FileName);
          Child->Modified = false;
          Child->IsNew = false;
        }
    }
    else
    {
      if (SaveDialog->Execute())
        if (savetexture(SaveDialog->FileName.c_str()))
        {
          Child->Caption = ExtractFileName(SaveDialog->FileName);  
          Child->Modified = false;   
          Child->IsNew = false;
        }
    }
  }
  MainForm->EndProgressWait(progressform);
 }
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormDestroy(TObject *Sender)
{
int i=0,j=0;
for(i=0;i<MAXTEXTURES;i++)
{
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
}
numtextures = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextureActionUpdate(TObject *Sender)
{
TAction* A = dynamic_cast<TAction *>(Sender);
if (MDIChildCount > 0) A->Enabled = true;
else A->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::TextureTilesRGBExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
textures[curtexture].format = FORMAT_TEXTURE_TILES_RGB;
Child->Modified = true;
}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextureTilesRGBAExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
textures[curtexture].format = FORMAT_TEXTURE_TILES_RGBA;
Child->Modified = true;
}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextureTilesCompressedRGBExecute(
      TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
textures[curtexture].format = FORMAT_TEXTURE_COMPRESSED_TILES_RGB;
Child->Modified = true;
}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextureTilesCompressedRGBAExecute(
      TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
textures[curtexture].format = FORMAT_TEXTURE_COMPRESSED_TILES_RGBA;
Child->Modified = true;
}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextureTilesRGBUpdate(TObject *Sender)
{
TAction* A = dynamic_cast<TAction *>(Sender);
if (MDIChildCount > 0)
{
 if ((textures[curtexture].format == FORMAT_TEXTURE_TILES_RGB)||(textures[curtexture].format == FORMAT_TEXTURE_COMPRESSED_TILES_RGB)) A->Enabled = true;
 else A->Enabled = false;
}
else A->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextureTilesRGBAUpdate(TObject *Sender)
{
TAction* A = dynamic_cast<TAction *>(Sender);
if (MDIChildCount > 0)
{
 if ((textures[curtexture].format == FORMAT_TEXTURE_TILES_RGBA)||(textures[curtexture].format == FORMAT_TEXTURE_COMPRESSED_TILES_RGBA)) A->Enabled = true;
 else A->Enabled = false;
}
else A->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormResize(TObject *Sender)
{
Panel1->Top = 4;
Panel1->Left = (ClientWidth-Panel1->Width)-5;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton1Click(TObject *Sender)
{
if (ActiveMDIChild) ActiveMDIChild->WindowState = wsMinimized;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton2Click(TObject *Sender)
{
if (ActiveMDIChild) ActiveMDIChild->WindowState = wsNormal;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::SpeedButton3Click(TObject *Sender)
{
if (ActiveMDIChild) ActiveMDIChild->Close();
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ToolBar1CustomDraw(TToolBar *Sender,
      const TRect &ARect, bool &DefaultDraw)
{
if (ActiveMDIChild)
{
if (ActiveMDIChild->WindowState == wsMaximized)
{
DrawIconEx(GetDC(ToolBar1->Handle),2,2,CopyImage(ActiveMDIChild->Icon->Handle,IMAGE_ICON,16,16,LR_COPYFROMRESOURCE),16, 16, 0, NULL, DI_NORMAL|DI_COMPAT|DI_IMAGE);
mdi_child_icon_on = true;    
//////ToolBar2->Repaint();
}
else
{
//////ToolBar1->Repaint();
//ToolBar2->Repaint();
mdi_child_icon_on = false;
}
}
else
{
//////ToolBar1->Repaint();
//ToolBar2->Repaint();
mdi_child_icon_on = false;
}  
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ToolBar1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
 if (Button == mbLeft)
 {
  if (ActiveMDIChild)
  {
   if (ActiveMDIChild->WindowState == wsMaximized)
   {
    if (mdi_child_icon_on == true)
    {
      if (in_area(X,Y,2,2,16,16))
      {
          FormPopupCreate(Left+ToolBar1->Left+12,Top+ToolBar1->Top+(Height-ClientHeight)+(ToolBar1->Height-7));
      }
    }
   }
  }
 }  
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FormPopupCreate(int X, int Y)
{
HMENU m = CreatePopupMenu();
HMENU sm = GetSystemMenu (ActiveMDIChild->Handle, false);
int cnt=GetMenuItemCount(sm);
for (int i=0;i<cnt;i++)
{
int id=GetMenuItemID(sm,i);
char str[256];
GetMenuString(sm,id,str,256,0);
MENUITEMINFO mi,smi;
int s=GetMenuState(sm,id,0);
int info = GetMenuItemInfo(sm,id,false,&smi);
GetMenuItemInfo(sm,id,false,&mi);
AppendMenu(m,s ,id,str);
SetMenuItemInfo(m, id, false, &mi);
switch(id)
{
case SC_RESTORE:
{
mi.cbSize = sizeof(MENUITEMINFO);
mi.fMask = MIIM_STATE|MIIM_BITMAP;
mi.fState = MFS_ENABLED;
mi.fType=MFT_BITMAP;
mi.hbmpItem = HBMMENU_MBAR_RESTORE;
SetMenuItemInfo(m, SC_RESTORE, false, &mi);
}
break;
case SC_MAXIMIZE:
{
mi.cbSize = sizeof(MENUITEMINFO);
mi.fMask = MIIM_STATE|MIIM_BITMAP;  
mi.fState = MFS_DISABLED;
mi.fType=MFT_BITMAP;
mi.hbmpItem = HBMMENU_POPUP_MAXIMIZE;
SetMenuItemInfo(m, SC_MAXIMIZE, false, &mi);
}
break;
case SC_MINIMIZE:
{
mi.cbSize = sizeof(MENUITEMINFO);
mi.fMask = MIIM_STATE|MIIM_BITMAP;    
mi.fState = MFS_ENABLED;
mi.fType=MFT_BITMAP;
mi.hbmpItem = HBMMENU_MBAR_MINIMIZE;
SetMenuItemInfo(m, SC_MINIMIZE, false, &mi);
}
break;
case SC_MOVE:
{
mi.cbSize = sizeof(MENUITEMINFO);
mi.fMask = MIIM_STATE;
mi.fState = MFS_DISABLED;
mi.fType = MFT_STRING;
mi.hbmpItem = NULL;
SetMenuItemInfo(m, SC_MOVE, false, &mi);
}
break;
case SC_SIZE:
{
mi.cbSize = sizeof(MENUITEMINFO);
mi.fMask = MIIM_STATE;
mi.fState = MFS_DISABLED;
mi.fType = MFT_STRING;
mi.hbmpItem = NULL;
SetMenuItemInfo(m, SC_SIZE, false, &mi);
}
break;
case SC_CLOSE:
{
mi.cbSize = sizeof(MENUITEMINFO);
mi.fMask = MIIM_STATE|MIIM_BITMAP;
mi.fState = MFS_ENABLED;
mi.fType=MFT_BITMAP;
mi.hbmpItem = HBMMENU_MBAR_CLOSE;
SetMenuItemInfo(m, SC_CLOSE, false, &mi);
}
break;
};
}
TrackPopupMenu(m,TPM_LEFTALIGN | TPM_RIGHTBUTTON, X, Y, 0, Handle, NULL);
DestroyMenu(m);
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::DrawGrid1DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
TRect R = Rect;
int borderspace = 2;
R.Left+=borderspace;
R.Right-=borderspace;
R.Top+=borderspace;
R.Bottom-=borderspace;
RECT r=R;
r.left=R.Left-borderspace;
r.top=R.Top-borderspace;
r.right=R.Right+borderspace;
r.bottom=R.Bottom+borderspace;
DrawEdge(DrawGrid1->Canvas->Handle,&r,BDR_SUNKENOUTER,BF_RECT);
R.Left--;
R.Top--;
R.Right++;
R.Bottom++;
BYTE c_r=0,c_g=0,c_b=0;
c_r=palette[calcoffset(ACol,ARow,2)][0];
c_g=palette[calcoffset(ACol,ARow,2)][1];
c_b=palette[calcoffset(ACol,ARow,2)][2];
DrawGrid1->Canvas->Brush->Color = TColor(RGB(c_r,c_g,c_b));
DrawGrid1->Canvas->FillRect(R);  
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DrawGrid2DrawCell(TObject *Sender, int ACol,
      int ARow, TRect &Rect, TGridDrawState State)
{
TRect R = Rect;
BYTE c_r=0,c_g=0,c_b=0;
c_r=palette[paletteoffset(ACol,ARow,32)][0];
c_g=palette[paletteoffset(ACol,ARow,32)][1];
c_b=palette[paletteoffset(ACol,ARow,32)][2];
DrawGrid2->Canvas->Brush->Color = TColor(RGB(c_r,c_g,c_b));
DrawGrid2->Canvas->FillRect(R);
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DrawGrid1MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
BYTE c_r=0,c_g=0,c_b=0;
int ACol=0,ARow=0;
DrawGrid1->MouseToCell(X,Y,ACol,ARow);
//ACol = DrawGrid1->Col;
//ARow = DrawGrid1->Row;
c_r=palette[calcoffset(ACol,ARow,2)][0];
c_g=palette[calcoffset(ACol,ARow,2)][1];
c_b=palette[calcoffset(ACol,ARow,2)][2];
COLORREF color = RGB(c_r,c_g,c_b);
if (Button == mbLeft)
{
  FGShape->Brush->Color = TColor(color);
} else
if (Button == mbMiddle)
{

} else
if (Button == mbRight)
{
  BGShape->Brush->Color = TColor(color);
}

if (FGShape->Brush->Color == TColor(clBlack))
 FGShape->Pen->Color = TColor(clWhite);
else
 FGShape->Pen->Color = TColor(clBlack);

if (BGShape->Brush->Color == TColor(clBlack))
 BGShape->Pen->Color = TColor(clWhite);
else
 BGShape->Pen->Color = TColor(clBlack);  
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::BGShapeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if (PaletteTable->PageIndex == 0)
{
BGColorDialog->Color = BGShape->Brush->Color;
if (BGColorDialog->Execute())
{
BGShape->Brush->Color = BGColorDialog->Color;

if (BGShape->Brush->Color == TColor(clBlack))
 BGShape->Pen->Color = TColor(clWhite);
else
 BGShape->Pen->Color = TColor(clBlack);
}
}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FGShapeMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
if (PaletteTable->PageIndex == 0)
{
FGColorDialog->Color = FGShape->Brush->Color;
if (FGColorDialog->Execute())
{
FGShape->Brush->Color = FGColorDialog->Color;

if (FGShape->Brush->Color == TColor(clBlack))
 FGShape->Pen->Color = TColor(clWhite);
else
 FGShape->Pen->Color = TColor(clBlack);
}
}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::Panel2Resize(TObject *Sender)
{
ControlBar3->Top = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ControlBar3DockDrop(TObject *Sender,
      TDragDockObject *Source, int X, int Y)
{
ControlBar3->Top = 0;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ControlBar3DockOver(TObject *Sender,
      TDragDockObject *Source, int X, int Y, TDragState State,
      bool &Accept)
{
ControlBar3->Top = 0;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetBGColor(TColor value)
{
    if(BGShape->Brush->Color != value) {
        BGShape->Brush->Color = value;
        if (BGShape->Brush->Color == TColor(clBlack))
         BGShape->Pen->Color = TColor(clWhite);
        else
         BGShape->Pen->Color = TColor(clBlack);
    }
}
//---------------------------------------------------------------------------
TColor __fastcall TMainForm::GetBGColor()
{
    return BGShape->Brush->Color;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetFGColor(TColor value)
{
    if(FGShape->Brush->Color != value) {
        FGShape->Brush->Color = value;
        if (FGShape->Brush->Color == TColor(clBlack))
         FGShape->Pen->Color = TColor(clWhite);
        else
         FGShape->Pen->Color = TColor(clBlack);
    }
}
//---------------------------------------------------------------------------
TColor __fastcall TMainForm::GetFGColor()
{
    return FGShape->Brush->Color;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetDrawingTool(TDrawingTool value)
{
    switch(value)
    {
           case dtZoom:
           {
              ZoomBtn->Down = true;
           }
           break;
           case dtFloodFill:
           {
              FillBtn->Down = true;
           }
           break;
           case dtColorPicker:
           {
              ColorPickerBtn->Down = true;
           }
           break;
           case dtErase:
           {
              EraseBtn->Down = true;
           }
           break;
           case dtText:
           {
              TextBtn->Down = true;
           }
           break;
           case dtPencil:
           {
              PenBtn->Down = true;
           }
           break;
           case dtLine:
           {
              LineBtn->Down = true;
           }
           break;
           case dtRect:
           {
              RectBtn->Down = true;
           }
           break;
           case dtFillRect:
           {
              FRectBtn->Down = true;
           }
           break;
           case dtEllipse:
           {
              EllipseBtn->Down = true;
           }
           break;
           case dtFillEllipse:
           {
              FEllipseBtn->Down = true;
           }
           break;
    };
    if(FDrawingTool != value) {
        FDrawingTool = value;
    }
}
//---------------------------------------------------------------------------
TDrawingTool __fastcall TMainForm::GetDrawingTool()
{
    if (ZoomBtn->Down) FDrawingTool = dtZoom;
    if (FillBtn->Down) FDrawingTool = dtFloodFill;
    if (ColorPickerBtn->Down) FDrawingTool = dtColorPicker;
    if (EraseBtn->Down) FDrawingTool = dtErase;
    if (TextBtn->Down) FDrawingTool = dtText;
    if (PenBtn->Down) FDrawingTool = dtPencil;
    if (LineBtn->Down) FDrawingTool = dtLine;
    if (RectBtn->Down) FDrawingTool = dtRect;
    if (FRectBtn->Down) FDrawingTool = dtFillRect;
    if (EllipseBtn->Down) FDrawingTool = dtEllipse;
    if (FEllipseBtn->Down) FDrawingTool = dtFillEllipse;
    return FDrawingTool;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormCreate(TObject *Sender)
{
DrawingTool = dtPencil;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::FormShow(TObject *Sender)
{
    if (MainForm->MDIChildCount>0)
    {
      MainForm->ToolBar1->Menu = NULL;
      MainForm->Texture1->Visible = true;  
      MainForm->Image1->Visible = true;
      MainForm->NW->Visible = true;
      MainForm->ToolBar1->Menu = MainMenu1;
    }
    else
    {
      MainForm->ToolBar1->Menu = NULL;
      MainForm->NW->Visible = false;
      MainForm->Texture1->Visible = false;  
      MainForm->Image1->Visible = false;
      MainForm->ToolBar1->Menu = MainMenu1;
    }
}
//---------------------------------------------------------------------------


void __fastcall TMainForm::FileSaveAs1Execute(TObject *Sender)
{
 if (MDIChildCount > 0)
 {
  TProgressForm * progressform = MainForm->BeginProgressWait("Please Wait, saving textures...");
  TMDIChild *Child = GetCurrentMDIChild();
  if (Child)
  {
    SaveDialog->FileName = Child->Caption;
    if (SaveDialog->Execute())
    {
       if (savetexture(SaveDialog->FileName.c_str()))
       {
          Child->Caption = ExtractFileName(SaveDialog->FileName);  
          Child->Modified = false;   
          Child->IsNew = false;
       }
    }
  }
  MainForm->EndProgressWait(progressform);
 }
}
//---------------------------------------------------------------------------
#include "editunit.h"
//---------------------------------------------------------------------------
void __fastcall TMainForm::FileSave1Update(TObject *Sender)
{
 bool m=false,e=false;
 if (MDIChildCount > 0)
 {
   if (ActiveMDIChild->ClassType() == __classid(TMDIChild))
   {
     TMDIChild* Child = dynamic_cast<TMDIChild *>(MainForm->ActiveMDIChild);
     if (Child->Modified) m = true;
     if (Child->Empty) e = true;
   }
   if (ActiveMDIChild->ClassType() == __classid(TEditForm))
   {
     TEditForm* Child = dynamic_cast<TEditForm *>(MainForm->ActiveMDIChild);
     if (Child->Modified) m = true;
     if (Child->Empty) e = true;
   }
   if (m || e)
   {
     FileSave1->Enabled = true;
   }
   else
   {    
     FileSave1->Enabled = false;
   }
 }     
 else FileSave1->Enabled = false;
}
//---------------------------------------------------------------------------
TMDIChild * __fastcall TMainForm::GetCurrentMDIChild(void)
{
 if (MDIChildCount > 0)
 {
   if (ActiveMDIChild)
   {
      if (ActiveMDIChild->ClassType() == __classid(TMDIChild))
      {
        TMDIChild* Child = dynamic_cast<TMDIChild *>(ActiveMDIChild);
        return Child;
      }
      else if (ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
        TEditForm* EditFrm = dynamic_cast<TEditForm *>(ActiveMDIChild);
        for (int i=0;i<MainForm->MDIChildCount;i++)
        {
            if (MainForm->MDIChildren[i]->ClassType() == __classid(TMDIChild))
            {                    
                TMDIChild* Child = dynamic_cast<TMDIChild *>(MainForm->MDIChildren[i]);
                int texture = Child->Tag;
                if (texture == EditFrm->texture) return Child;
            }
        }
      }
   }
 }
 return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::DrawGrid2SelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
if (clp_mm) CanSelect = false;
else
{
CanSelect = true;
BYTE c_r=0,c_g=0,c_b=0;
c_r=palette[paletteoffset(ACol,ARow,32)][0];
c_g=palette[paletteoffset(ACol,ARow,32)][1];
c_b=palette[paletteoffset(ACol,ARow,32)][2];
COLORREF color = RGB(c_r,c_g,c_b);
if (clp_mb == mbLeft)
{
  FGShape->Brush->Color = TColor(color);
} else
if (clp_mb == mbMiddle)
{

} else
if (clp_mb == mbRight)
{
  BGShape->Brush->Color = TColor(color);
}

if (FGShape->Brush->Color == TColor(clBlack))
 FGShape->Pen->Color = TColor(clWhite);
else
 FGShape->Pen->Color = TColor(clBlack);

if (BGShape->Brush->Color == TColor(clBlack))
 BGShape->Pen->Color = TColor(clWhite);
else
 BGShape->Pen->Color = TColor(clBlack);
}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DrawGrid2MouseMove(TObject *Sender,
      TShiftState Shift, int X, int Y)
{
clp_mm = true;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DrawGrid2MouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
clp_mm = false;
clp_mb = Button;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::DrawGrid2MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
clp_mm = false;
clp_mb = Button;
if (clp_mb == mbRight)
{
bool tmpsl=true;
int ACol=0,ARow=0;
DrawGrid2->MouseToCell(X,Y,ACol,ARow);
DrawGrid2SelectCell(Sender,ACol,ARow,tmpsl);
}
}
//---------------------------------------------------------------------------
BYTE FindColorInPalette(char cPalette[256][3], BYTE r, BYTE g, BYTE b)
{
  for (int i=0;i<256;i++)
  {
      BYTE c_r = (BYTE)cPalette[i][0];
      BYTE c_g = (BYTE)cPalette[i][1];
      BYTE c_b = (BYTE)cPalette[i][2];
      if ((c_r == r)&&(c_g == g)&&(c_b == b)) return i;
  }
  return 0;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetPaletteDefault(void)
{
   for (int i=0;i<256;i++)
   {
      palette[i][0] = Def_RGB[i*3+0];
      palette[i][1] = Def_RGB[i*3+1];
      palette[i][2] = Def_RGB[i*3+2];
   }
   DrawGrid1->Repaint();
   DrawGrid2->Repaint();
   if (PaletteTable->PageIndex == 1)
   {
     FGColor = FixColor8(FGColor);
     BGColor = FixColor8(BGColor);
   }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::SetPalette(char cpalette[256][3])
{
   for (int i=0;i<256;i++)
   {
      palette[i][0] = (BYTE)cpalette[i][0];
      palette[i][1] = (BYTE)cpalette[i][1];
      palette[i][2] = (BYTE)cpalette[i][2];
   }
   DrawGrid1->Repaint();
   DrawGrid2->Repaint();
   if (PaletteTable->PageIndex == 1)
   {
     FGColor = FixColor8(FGColor);
     BGColor = FixColor8(BGColor);
   }
}
//---------------------------------------------------------------------------
TColor __fastcall TMainForm::FixColor8(TColor color)
{
   BYTE c_r = GetRValue(color);
   BYTE c_g = GetGValue(color);
   BYTE c_b = GetBValue(color);
   char cpalette[256][3];
   for (int i=0;i<256;i++)
   {
      cpalette[i][0] = (BYTE)palette[i][0];
      cpalette[i][1] = (BYTE)palette[i][1];
      cpalette[i][2] = (BYTE)palette[i][2];
   }
   BYTE c = FindColorInPalette(cpalette,c_r,c_g,c_b);
   BYTE r = palette[c][0];
   BYTE g = palette[c][1];
   BYTE b = palette[c][2];
   return TColor(RGB(r,g,b));
}
//---------------------------------------------------------------------------
TProgressForm * __fastcall TMainForm::BeginProgressWait(AnsiString message)
{
    Screen->Cursor = crAppStart;
    Application->ProcessMessages();
    TProgressForm *progressform = new TProgressForm(Application);
    Application->ProcessMessages();
    progressform->WaitLabel->Caption = message; 
    progressform->AlphaBlendValue = 255;
    Application->ProcessMessages();     
    progressform->Show();
    Application->ProcessMessages();
    progressform->BringToFront();
    Application->ProcessMessages();
    return progressform;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EndProgressWait(TProgressForm * progressform)
{
    if (progressform)
    {
      Application->ProcessMessages();
      progressform->AlphaBlendValue = 0;
      Application->ProcessMessages();
      progressform->Close();
      delete progressform;
    }
    Screen->Cursor = crDefault;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::FileSaveAs1Update(TObject *Sender)
{
 if (MDIChildCount > 0)
 {
   FileSaveAs1->Enabled = true;
 }
 else FileSaveAs1->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ApplicationEvents1Idle(TObject *Sender,
      bool &Done)
{
    FormResize(Sender);
    if (ActiveMDIChild)
    {
      if (ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
         Edit1->Visible = true;
      }
      else Edit1->Visible = false;
      
      if (ActiveMDIChild->ClassType() == __classid(TMDIChild))
      {
      //   curtexture = ActiveMDIChild->Tag;
      }
      if (ActiveMDIChild->WindowState == wsMaximized)
      {
       Panel1->Visible = true;
       if (ToolBar1->Indent != 21)
       {
         ToolBar1->Indent = 21;
         ToolBar1->Repaint();
         ToolBar2->Repaint();
         mdi_child_icon_on = true;
       }
      }
      else Panel1->Visible = false;
    } else Panel1->Visible = false;
    if (ToolBar1->Indent != 5)
    {
           if (ActiveMDIChild)
           {
              if (ActiveMDIChild->WindowState != wsMaximized)
              {
                ToolBar1->Indent = 5;
                ToolBar1->Repaint();
                //ToolBar2->Repaint();
              } else
              {
                if (ToolBar1->Indent != 21)
                {
                  ToolBar1->Indent = 21;
                  ToolBar1->Repaint();
                  ToolBar2->Repaint();
                  mdi_child_icon_on = true;
                }
              }
           }
           else
           {
             ToolBar1->Indent = 5;
             ToolBar1->Repaint();    
             //ToolBar2->Repaint();
           }
    }
    else Edit1->Visible = false;

    if (!ActiveMDIChild) return;
    // curtexture = ActiveMDIChild->Tag;
    TMDIChild *Child = GetCurrentMDIChild();
    if (Child)
    {
       curtexture = Child->Tag;
    }
    switch (textures[curtexture].format)
    {
    case FORMAT_TEXTURE_TILES_RGB:
    {
      TextureTilesRGB->Checked = true;
      TextureTilesRGBA->Checked = false;
      TextureTilesCompressedRGB->Checked = false;
      TextureTilesCompressedRGBA->Checked = false;
    }
    break;
    case FORMAT_TEXTURE_TILES_RGBA:
    {
      TextureTilesRGB->Checked = false;
      TextureTilesRGBA->Checked = true;
      TextureTilesCompressedRGB->Checked = false;
      TextureTilesCompressedRGBA->Checked = false;
    }
    break;
    case FORMAT_TEXTURE_COMPRESSED_TILES_RGB:
    {
      TextureTilesRGB->Checked = false;
      TextureTilesRGBA->Checked = false;
      TextureTilesCompressedRGB->Checked = true;
      TextureTilesCompressedRGBA->Checked = false;
    }
    break;
    case FORMAT_TEXTURE_COMPRESSED_TILES_RGBA:
    {
      TextureTilesRGB->Checked = false;
      TextureTilesRGBA->Checked = false;
      TextureTilesCompressedRGB->Checked = false;
      TextureTilesCompressedRGBA->Checked = true;
    }
    break;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EditMenuUpdate(void)
{
  if (MainForm->MDIChildCount>0)
  {
      MainForm->ToolBar1->Menu = NULL;
      if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
         MainForm->Edit1->Visible = true;
         MainForm->Adjust1->Visible = true;
      }
      else
      {
         MainForm->Edit1->Visible = false;
         MainForm->Adjust1->Visible = false;
      }
      MainForm->ToolBar1->Menu = MainForm->MainMenu1;
  }
  else
  {
     MainForm->ToolBar1->Menu = NULL;
     MainForm->Edit1->Visible = false;
     MainForm->Adjust1->Visible = false;
     MainForm->ToolBar1->Menu = MainForm->MainMenu1;
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EditUpdate(TObject *Sender)
{
  TAction* A = dynamic_cast<TAction *>(Sender);
  if (MainForm->MDIChildCount>0)
  {
      if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
        A->Enabled = true;
      } else A->Enabled = false;
  } else A->Enabled = false;
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EditPaste1Execute(TObject *Sender)
{
  if (MainForm->MDIChildCount>0)
  {
      if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
         TEditForm* Child = dynamic_cast<TEditForm *>(MainForm->ActiveMDIChild);
         if (Clipboard()->HasFormat(CF_BITMAP))
         {
            Graphics::TBitmap *TempBitmap = new Graphics::TBitmap();
            try
            {
                  TempBitmap->Assign(Clipboard());
                  Child->Image->Bitmap->Canvas->Draw(0,0,TempBitmap);
                  delete TempBitmap;
            }
            catch(...)
            {
                  delete TempBitmap;
            }
         }
      }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EditCut1Execute(TObject *Sender)
{
  if (MainForm->MDIChildCount>0)
  {
      if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
         TEditForm* Child = dynamic_cast<TEditForm *>(MainForm->ActiveMDIChild);
         EditCopy1Execute(Sender);
         TColor tmpcolor = Child->Image->Bitmap->Canvas->Brush->Color;
         Child->Image->Bitmap->Canvas->Brush->Color = MainForm->BGColor;
         Child->Image->Bitmap->Canvas->CopyMode = cmWhiteness;
         Types::TRect ARect = Rect(0, 0, Child->Image->Bitmap->Width, Child->Image->Bitmap->Height);
         Child->Image->Bitmap->Canvas->CopyRect(ARect, Child->Image->Bitmap->Canvas, ARect);
         Child->Image->Bitmap->Canvas->CopyMode = cmSrcCopy;
         Child->Image->Bitmap->FillRectTS(ARect,Color32(Child->Image->Bitmap->Canvas->Brush->Color));
         Child->Image->Bitmap->Canvas->Brush->Color = tmpcolor;
      }
  }

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EditCopy1Execute(TObject *Sender)
{
  if (MainForm->MDIChildCount>0)
  {
      if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
         TEditForm* Child = dynamic_cast<TEditForm *>(MainForm->ActiveMDIChild);
         Clipboard()->Assign(Child->Image->Bitmap);
      }
  }

}
//---------------------------------------------------------------------------


void __fastcall TMainForm::Brightness1Execute(TObject *Sender)
{
  if (MainForm->MDIChildCount>0)
  {
      if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
      {
         TEditForm* Child = dynamic_cast<TEditForm *>(MainForm->ActiveMDIChild);
         Graphics::TBitmap *preview = new Graphics::TBitmap();
         preview->Assign(Child->Image->Bitmap);
         BDlg->Bitmap->Assign(preview);
         delete preview;
         if (BDlg->ShowModal() == mrOk)
         {
            int value = BDlg->ScrollBar1->Position;
            for (int y=0;y<Child->Image->Bitmap->Height;y++)
            {
              for (int x=0;x<Child->Image->Bitmap->Width;x++)
              {
                unsigned int c = Child->Image->Bitmap->Pixel[x][y];
                unsigned int color = 0;
                unsigned char r,g,b,a;
                Gr32::Color32ToRGBA(c,r,g,b,a);
                r = britable[value][r];
                g = britable[value][g];
                b = britable[value][b];
                color = Color32(r,g,b,a);
                Child->Image->Bitmap->Pixel[x][y] = color;
              }
            }
            Child->Image->Bitmap->Changed();
            Child->UpdateImage(Sender);
         }
      }
  }
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::AddExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
 Child->Button1Click(Sender);

 if (MainForm->MDIChildCount>0)
 {
     if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
     {
         Child->Button3Click(Sender);
     }
 }
}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::NewTileExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
 Child->Button2Click(Sender);

 if (MainForm->MDIChildCount>0)
 {
     if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
     {
         Child->Button3Click(Sender);
     }
 }

}
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::TextureInfoExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
 int texture = Child->Tag;
 curtexture = texture;
 char t_name[256];
 char t_description[256];
 char t_comments[256];
 char t_company[64];
 char t_copyright[64];  
 TextureInfoDlg->T_Name->Text = textures[texture].name;
 TextureInfoDlg->T_Description->Text = textures[texture].description;
 TextureInfoDlg->T_Comments->Text = textures[texture].comments;
 TextureInfoDlg->T_Company->Text = textures[texture].company;
 TextureInfoDlg->T_Copyright->Text = textures[texture].copyright;
 if (TextureInfoDlg->ShowModal() == mrOk)
 {
   strcpy(t_name,TextureInfoDlg->T_Name->Text.c_str());
   strcpy(t_description,TextureInfoDlg->T_Description->Text.c_str());
   strcpy(t_comments,TextureInfoDlg->T_Comments->Text.c_str());
   strcpy(t_company,TextureInfoDlg->T_Company->Text.c_str());
   strcpy(t_copyright,TextureInfoDlg->T_Copyright->Text.c_str());
   strcpy(textures[texture].name,t_name);
   strcpy(textures[texture].description,t_description);
   strcpy(textures[texture].comments,t_comments);
   strcpy(textures[texture].company,t_company);
   strcpy(textures[texture].copyright,t_copyright);
   Child->Modified = true;
 }
}
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ExportExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
 if (MainForm->MDIChildCount>0)
 {
     if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
     {
        TEditForm* Edit_Form = dynamic_cast<TEditForm *>(MainForm->ActiveMDIChild);
        int texture = Edit_Form->texture;
        int image = Edit_Form->image;
        curtexture = texture;
        Child->ListView1->Selected = Child->ListView1->Items->Item[image];
        Child->ListView1->ItemFocused = Child->ListView1->Items->Item[image];
     }
 }
 Child->Button4Click(Sender);

}

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::EditExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
 Child->Button3Click(Sender);
}

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImportExecute(TObject *Sender)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
  
}

}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImageItemUpdate(TObject *Sender)
{
TAction* A = dynamic_cast<TAction *>(Sender);
if (MDIChildCount > 0)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
  if (Child->ListView1->Selected) A->Enabled = true;
  else A->Enabled = false;
} else A->Enabled = false;
}
else A->Enabled = false;
}
//---------------------------------------------------------------------------

void __fastcall TMainForm::ImportUpdate(TObject *Sender)
{
TAction* A = dynamic_cast<TAction *>(Sender);
if (MDIChildCount > 0)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
  if (Child->ListView1->Selected)
  {
      if (MainForm->MDIChildCount>0)
      {
          if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
          {
            A->Enabled = true;
          } else A->Enabled = false;
      } else A->Enabled = false;
  }
  else A->Enabled = false;
} else A->Enabled = false;
}
else A->Enabled = false;

}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ImageEditUpdate(TObject *Sender)
{
TAction* A = dynamic_cast<TAction *>(Sender);
if (MDIChildCount > 0)
{
TMDIChild *Child = GetCurrentMDIChild();
if (Child)
{
  if (Child->ListView1->Selected)
  {
      if (MainForm->MDIChildCount>0)
      {
          if (MainForm->ActiveMDIChild->ClassType() == __classid(TEditForm))
          {
              ImageEditMenuVisible(A, false);
          } else ImageEditMenuVisible(A, true);
      } else ImageEditMenuVisible(A, false);
  }
  else
  {
    ImageEditMenuVisible(A, true);
    A->Enabled = false;
  }
} else ImageEditMenuVisible(A, false);
}
else ImageEditMenuVisible(A, false);
}
//---------------------------------------------------------------------------
void __fastcall TMainForm::ImageEditMenuVisible(TAction* mAction, bool mVisible)
{
    bool mEnabled = mVisible;
    NE->Visible = mVisible;
    Edit3->Visible = mVisible;
    mAction->Enabled = mEnabled;
}
//---------------------------------------------------------------------------

