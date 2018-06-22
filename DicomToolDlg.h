
// DicomToolDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "afxeditbrowsectrl.h"
#include "DicomNet.h"

// CDicomToolDlg �Ի���
class CDicomToolDlg : public CBCGPDialog
{
// ����
public:
	CDicomToolDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DICOMTOOL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnTcnSelchangeTabLoaddicom(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButton1();
	CIPAddressCtrl m_hostIP;
	CString m_port;
	CString m_RemoteAEtitle;
	CMFCEditBrowseCtrl m_FileCtrl;
	afx_msg void OnBnClickedBtndisconnect();

private:
	DicomNet* p_Connect;

};
