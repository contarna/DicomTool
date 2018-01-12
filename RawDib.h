#if !defined(AFX_RAWDIB_H__B7C1A9F4_0DBB_43CC_9567_55E1C252C412__INCLUDED_)
#define AFX_RAWDIB_H__B7C1A9F4_0DBB_43CC_9567_55E1C252C412__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// RawDib.h : header file
//

#include "common.h"
#include "math.h"
/////////////////////////////////////////////////////////////////////////////
// CRawDib window
DECLARE_HANDLE(HDIB);

class CRawDib : public CStatic
{
// Construction
public:
	CRawDib();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRawDib)
	//}}AFX_VIRTUAL

// Implementation
public:	
	HDIB		m_hBMP;
	HDIB		m_hDibHead;
	BYTE*		m_pRawDataMatrix;	//图像数据指针，要分配内存
	WORD*		m_pRawData;			//图像数据指针，不分配内存，指向RawImageData
	CRect		m_crSrc;
	CRect		m_crDest;
	HPALETTE	m_hLogPal;
	CDC*		m_pDC;
	int			m_nWWid;
	int			m_nWPos;
	WORD		g_GammaLUT[65536];

	// Operations
public:
	void		Init();
	void		DrawImage();
	void		LoadRawImage(LPCTSTR lpstrRawImagePath);

	WORD*		BuildTable(float Gamma);
	HDIB		CreatHDibHead();
	HPALETTE	CreatHLogPal();
	void		dspBMP(HDC hDC,CRect crSrc,CRect crDest,HDIB hDIB,HDIB hDibHead,HPALETTE hLogPal);
	int			GetBMP(HGLOBAL hBMP,BYTE* pDR,int WindowWidth,int WindowPosition);

	// Deconstruction
public:	
	void AdjustWidAndPos(unsigned short* lpDIBBits,int nHeight, int nWidth, int* WwidWpos);
	virtual ~CRawDib();

	// Generated message map functions
protected:
	//{{AFX_MSG(CRawDib)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RAWDIB_H__B7C1A9F4_0DBB_43CC_9567_55E1C252C412__INCLUDED_)
