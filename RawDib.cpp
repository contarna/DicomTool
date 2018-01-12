// RawDib.cpp : implementation file
//

#include "stdafx.h"

#include "RawDib.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CRawDib

CRawDib::CRawDib()
{
	m_nWWid = 65535;
	m_nWPos = 32768;

	m_pRawDataMatrix = new BYTE[ImageMatrixSize]; //3072*3072*2
	memset(m_pRawDataMatrix,0,ImageMatrixSize);

	m_pRawData = NULL;
	BuildTable(2.2F);
}

CRawDib::~CRawDib()
{
	delete []m_pRawDataMatrix;
	m_pRawDataMatrix = NULL;
}


BEGIN_MESSAGE_MAP(CRawDib, CStatic)
	//{{AFX_MSG_MAP(CRawDib)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRawDib message handlers

//类外调用函数======================================
void CRawDib::Init()
{
	m_hLogPal=CreatHLogPal();
	m_hDibHead=CreatHDibHead();
	m_hBMP=(HDIB)::GlobalAlloc(GHND,IMAGESIZE);				//3072*3072

	m_crSrc.SetRect(0,0,ImageWidth-1,ImageHeight-1);		//3072-1,3072-1
	GetClientRect(m_crDest);
	m_pDC=GetDC();
}

void CRawDib::LoadRawImage(LPCTSTR lpstrRawImagePath)
{
	ASSERT(lpstrRawImagePath);

	CFile fimage;
	if (fimage.Open(lpstrRawImagePath,CFile::modeRead)==0)
	{
		AfxMessageBox("无法打开文件!");
		return;
	}

	ULONGLONG RawLong=fimage.GetLength();
	if (RawLong<ImageMatrixSize)						//3072*3072*2
	{
		CString str;
		str.Format("%d--%d",RawLong,ImageMatrixSize);
		AfxMessageBox("图像长度不正确!");
		fimage.Close();
		return;
	}

	fimage.Seek(RawLong-ImageMatrixSize,CFile::begin);	//3072*3072*2
	fimage.Read(m_pRawDataMatrix,ImageMatrixSize);		//3072*3072*2
	fimage.Close();
	m_pRawData=(WORD *)m_pRawDataMatrix;				//将m_pRawDat指向m_pRawDataMatrix
	DrawImage();
}

void CRawDib::DrawImage()
{
	GetBMP(m_hBMP,m_pRawDataMatrix,m_nWWid,m_nWPos);
	dspBMP(m_pDC->m_hDC,m_crSrc,m_crDest,m_hBMP,m_hDibHead,m_hLogPal);
}
//类外调用函数======================================




//类内调用函数======================================
WORD* CRawDib::BuildTable(float Gamma)
{
	float fPrecompensation=1/Gamma;
	int i;
	float f;
	for (i=0;i<65536;i++)
	{
		f=(i+0.5F)/65536;
		f=(float)pow(f,fPrecompensation);
		g_GammaLUT[i]=(unsigned short)(f*65536-0.5F);
	}
	return g_GammaLUT;
}

HDIB CRawDib::CreatHDibHead()
{
	HDIB				g_hDibHead;
	int					i;
	WORD				wDibHead;
	LPSTR				lpbi;
	BITMAPINFOHEADER*	lpDIB;
	RGBQUAD*			lpRGB;
	wDibHead    =    sizeof(BITMAPINFOHEADER);
	wDibHead    +=   256*sizeof(RGBQUAD);
	g_hDibHead  =    (HDIB)GlobalAlloc(GHND,wDibHead);
	if (NULL==g_hDibHead)  
		return FALSE;
	lpbi        =   (LPSTR)GlobalLock(g_hDibHead);
	lpDIB       =   (BITMAPINFOHEADER*)lpbi;
	lpDIB->biSize    =   sizeof(BITMAPINFOHEADER);
	lpDIB->biWidth   =   ImageWidth;	//3072
	lpDIB->biHeight  =   ImageHeight;	//3072
	lpDIB->biPlanes  =   1;
	lpDIB->biBitCount=   8;
	lpDIB->biCompression	=  BI_RGB;
	lpDIB->biSizeImage		=  0;
	lpDIB->biXPelsPerMeter	=  0;
	lpDIB->biYPelsPerMeter	=  0;
	lpDIB->biClrUsed		=  0;
	lpDIB->biClrImportant	=  0;
	lpRGB=(RGBQUAD*)(lpbi+sizeof(BITMAPINFOHEADER));
	for (i=0;i<256;i++)
	{
		lpRGB[i].rgbBlue=(BYTE)i;
		lpRGB[i].rgbGreen=(BYTE)i;
		lpRGB[i].rgbRed=(BYTE)i;
		lpRGB[i].rgbReserved=0;
	}
	::GlobalUnlock((HGLOBAL)g_hDibHead);
	return g_hDibHead;
}

HPALETTE CRawDib::CreatHLogPal()
{
	int			i;
	LOGPALETTE*	pPal;
	HPALETTE	g_HLogPal;
	pPal = (LOGPALETTE*)LocalAlloc(LPTR,sizeof(LOGPALETTE)+256*sizeof(PALETTEENTRY));
	pPal->palNumEntries=256;
	pPal->palVersion=0x300;
	for (i=0;i<256;i++)			//256 = 2^8
	{
		pPal->palPalEntry[i].peBlue=(BYTE)i;
		pPal->palPalEntry[i].peGreen=(BYTE)i;
		pPal->palPalEntry[i].peRed=(BYTE)i;
		pPal->palPalEntry[i].peFlags=(BYTE)0;
	}
	g_HLogPal=(HPALETTE)::GlobalAlloc(GHND,sizeof(LOGPALETTE)+256*sizeof(PALETTEENTRY));
	g_HLogPal=CreatePalette(pPal);
	LocalFree((HANDLE)pPal);
	return g_HLogPal;
}

void CRawDib::dspBMP(HDC hDC,CRect crSrc,CRect crDest,HDIB hDIB,HDIB hDibHead,HPALETTE hLogPal)
{
	BITMAPINFO* lpbmi;
	HPALETTE    hOldPal;
	CRect       Rect;
	BITMAPINFOHEADER* lpDIB;
	BYTE*       pBmp=(BYTE*)GlobalLock((HGLOBAL)hDIB);
	lpbmi=(BITMAPINFO*)GlobalLock((HGLOBAL)hDibHead);
	lpDIB=(BITMAPINFOHEADER*)lpbmi;
	lpDIB->biWidth=ImageWidth;
	lpDIB->biHeight=ImageHeight;
	hOldPal=::SelectPalette(hDC,hLogPal,FALSE);
	::RealizePalette (hDC);
	::SetStretchBltMode(hDC,COLORONCOLOR);
	::StretchDIBits(hDC,crDest.left,crDest.top,crDest.right-crDest.left+1,crDest.bottom-crDest.top+1,crSrc.left,crSrc.top,crSrc.right-crSrc.left+1,crSrc.bottom-crSrc.top+1,pBmp,lpbmi,DIB_RGB_COLORS,SRCCOPY);
	::SelectPalette(hDC,hOldPal,FALSE);
	GlobalUnlock((HGLOBAL)hDibHead);
	GlobalUnlock((HGLOBAL)hDIB);
}

BOOL CRawDib::GetBMP(HGLOBAL hBMP,BYTE* pRawData,int WindowWidth,int WindowPosition)
{
	if (pRawData==NULL)
	{
		return FALSE;
	}
	BYTE *pBMP = (BYTE*)::GlobalLock((HGLOBAL)hBMP);
	WORD* pWDR=(WORD*)pRawData;
	int low=WindowPosition-WindowWidth/2;
	if (low<0)
	{
		low=0;
	}
	int	high=WindowPosition+WindowWidth/2;
	if (high>65535)
	{
		high=65535;
	}
	int iWidth=WindowWidth+1;
	int piexel;
	for (int i=0;i<ImageWidth*ImageHeight;i++)
	{
		piexel= pWDR[i];
		if (piexel<low)
		{
			piexel=0;
		}
		else if (piexel>high)
		{
			piexel=255;
		}
		else
			piexel=((piexel-low)<<8)/iWidth;
		
		pBMP[i]=piexel;
	}
	::GlobalUnlock((HGLOBAL)hBMP);
	return TRUE;
}

void CRawDib::AdjustWidAndPos(unsigned short *lpDIBBits, int nHeight, int nWidth, int *WwidWpos)
{
	// 指向源图像的指针
	unsigned short*	lpSrc;
	unsigned short*	m_llpDIBBits=lpDIBBits;
	
	// 循环变量
	int	i,j;
	
	// 灰度映射表
	//	unsigned short	bMap[65536];
	
	// 各个灰度值的计数
	unsigned long	m_lCount[65536];
	unsigned long	minsum=0;
	unsigned long	maxsum=0;
	
	unsigned long autocenter;
	unsigned long autowindow;
	//****************************
	// 计算原始累计直方图
	
	for (i = 0; i < 65536; i ++)
	{
		// 清零
		m_lCount[i] = 0;
	}
	
	// 计算各个灰度值的计数
	for (i = 0; i < nHeight; i ++)
	{
		for (j = 0; j < nWidth; j ++)
		{
			lpSrc = (unsigned short *)m_llpDIBBits + nWidth * i + j;
			
			// 计数加1
			m_lCount[*(lpSrc)]++;
		}
	}
	unsigned long transepoint=(unsigned long)((nHeight*nWidth)/65535);
	unsigned long counter=6*transepoint;
	for (i = 0; i < 65536 && counter>0; i ++)
	{
		if (m_lCount[i]>transepoint)
		{
			minsum += i;
			counter--;
		}
	}
	
	counter=6*transepoint;
	for (i = 65535; (unsigned long)i >=0 && counter>0; i --)
	{
		if (m_lCount[i]>transepoint)
		{
			maxsum += i;
			counter--;
		}
	}
	counter=6*transepoint;
	
	autowindow = (maxsum-minsum)/counter;
	
	autocenter = (minsum+maxsum)/(counter*2);
	unsigned long GreySumLeft=0;
	unsigned long GreySumRight=0;
	for (i = minsum/counter; (unsigned long)i < autocenter; i++)
	{
		GreySumLeft += m_lCount[i];
	}
	for (i = autocenter; (unsigned long)i < maxsum/counter; i++)
	{
		GreySumRight += m_lCount[i];
	}
	if (GreySumRight>GreySumLeft)
	{
		autocenter = (minsum+maxsum)/(counter*2) + (maxsum-minsum)/(counter*8);
	}
	else
	{
		autocenter = (minsum+maxsum)/(counter*2) - (maxsum-minsum)/(counter*8);
	}
	WwidWpos[0]=autowindow;
	WwidWpos[1]=autocenter;

}
//类内调用函数======================================