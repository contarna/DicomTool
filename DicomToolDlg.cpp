
// DicomToolDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "DicomTool.h"
#include "DicomToolDlg.h"
#include "afxdialogex.h"
//#include "DcmTK.h"
#include "DicomNet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CBCGPDialog
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CBCGPDialog(IDD_ABOUTBOX)
{
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CBCGPDialog)
END_MESSAGE_MAP()


// CDicomToolDlg 对话框



CDicomToolDlg::CDicomToolDlg(CWnd* pParent /*=NULL*/)
	: CBCGPDialog(IDD_DICOMTOOL_DIALOG, pParent)
	, m_port(_T(""))
	, m_RemoteAEtitle(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	EnableVisualManagerStyle(TRUE, TRUE);
}

void CDicomToolDlg::DoDataExchange(CDataExchange* pDX)
{
	CBCGPDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_hostIP);
	DDX_Text(pDX, IDC_EDIT1, m_port);
	DDX_Text(pDX, IDC_EDIT2, m_RemoteAEtitle);
	DDX_Control(pDX, IDC_MFCEDITBROWSE1, m_FileCtrl);
}

BEGIN_MESSAGE_MAP(CDicomToolDlg, CBCGPDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CDicomToolDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CDicomToolDlg::OnBnClickedButton1)
END_MESSAGE_MAP()


// CDicomToolDlg 消息处理程序

BOOL CDicomToolDlg::OnInitDialog()
{
	CBCGPDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CDicomToolDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CBCGPDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CDicomToolDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CBCGPDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CDicomToolDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CDicomToolDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	DcmFileFormat fileformat;
	OFCondition oc = fileformat.loadFile("H:\\324\\324-1-1-001.dcm");
	if (oc.good()) {
		OFString patientName;
		if (fileformat.getDataset()->findAndGetOFString(DCM_PatientName, patientName).good())
		{
			CString str;
			str.Format("Patient Name:%s", patientName.data());
			::AfxMessageBox(str);
		}
	}

	//CBCGPDialog::OnOK();
}


void CDicomToolDlg::OnTcnSelchangeTabLoaddicom(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
}

	DicomNet* net = new DicomNet();
void CDicomToolDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);

	DWORD dwIP;
	IN_ADDR ia;
	CString strIP;
	m_hostIP.GetAddress(dwIP);
	ia.S_un.S_addr = htonl(dwIP);

	net->connectCfg.IP = inet_ntoa(ia);
	net->connectCfg.Port = atoi(m_port);
	net->connectCfg.PeerAE = m_RemoteAEtitle;
	net->connectCfg.OurAE = "ANY";
	net->Connect();
}
