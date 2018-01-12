#pragma once
#include "RawDib.h"


// CLoadDcmDlg 对话框

class CLoadDcmDlg : public CBCGPDialog
{
	DECLARE_DYNAMIC(CLoadDcmDlg)

public:
	CLoadDcmDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CLoadDcmDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG_LoadDicom };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	CRawDib m_rRawDib;
};
