// LoadDcmDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DicomTool.h"
#include "LoadDcmDlg.h"
#include "afxdialogex.h"


// CLoadDcmDlg 对话框

IMPLEMENT_DYNAMIC(CLoadDcmDlg, CBCGPDialog)

CLoadDcmDlg::CLoadDcmDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(IDD_DIALOG_LoadDicom, pParent)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

CLoadDcmDlg::~CLoadDcmDlg()
{
}

void CLoadDcmDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_Image, m_rRawDib);
}


BEGIN_MESSAGE_MAP(CLoadDcmDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CLoadDcmDlg 消息处理程序
