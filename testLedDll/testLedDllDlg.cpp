
// testLedDllDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "testLedDll.h"
#include "testLedDllDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestLedDllDlg 对话框



CtestLedDllDlg::CtestLedDllDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_TESTLEDDLL_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//pJudge = NULL;
	pInitFun = NULL;
	pRun = NULL;
}

CtestLedDllDlg::~CtestLedDllDlg()
{
	FreeLibrary(dllHandle);
}

void CtestLedDllDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CtestLedDllDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDOK, &CtestLedDllDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDC_BUTTON1, &CtestLedDllDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CtestLedDllDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON4, &CtestLedDllDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON3, &CtestLedDllDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CtestLedDllDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CtestLedDllDlg::OnBnClickedButton6)
END_MESSAGE_MAP()


// CtestLedDllDlg 消息处理程序

BOOL CtestLedDllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	dllHandle = LoadLibrary(_T("LedDll.dll"));
	if (dllHandle == NULL)
	{
		AfxMessageBox(_T("LOAD FAIL"));
		return TRUE;
	}

	//加载dll初始化函数
	pRun = (Run)::GetProcAddress(dllHandle, "Run");
	if (pRun == NULL) {
		AfxMessageBox(_T("function  load failed!\n"));
		return false;
	}
	pRunDirect = (RunDirect)::GetProcAddress(dllHandle, "RunDirect");
	if (pRunDirect == NULL) {
		AfxMessageBox(_T("function  load failed!\n"));
		return false;
	}
	pRunDirect2 = (RunDirect2)::GetProcAddress(dllHandle, "RunDirect2");
	if (pRunDirect2 == NULL) {
		AfxMessageBox(_T("function  load failed!\n"));
		return false;
	}
	pInitFun = (initDll)::GetProcAddress(dllHandle, "InitDll");
	if (pInitFun == NULL) {
		AfxMessageBox(_T("function  load failed!\n"));
		return false;
	}
	pUinitFun = (UinitDll)::GetProcAddress(dllHandle, "UinitDll");
	if (pUinitFun == NULL) {
		AfxMessageBox(_T("function  load failed!\n"));
		return false;
	}
	pGrabOneImage = (grabOneImage)::GetProcAddress(dllHandle, "GrabOneImage");
	if (pGrabOneImage == NULL) {
		AfxMessageBox(_T("function  load failed!\n"));
		return false;
	}
	pGrabOneImageDrect = (grabOneImage)::GetProcAddress(dllHandle, "GrabOneImageDirect");
	if (pGrabOneImageDrect == NULL) {
		AfxMessageBox(_T("function  load failed!\n"));
		return false;
	}

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CtestLedDllDlg::OnPaint()
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
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CtestLedDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestLedDllDlg::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CtestLedDllDlg::OnBnClickedButton1()
{
	char res[20];
	memset(res, 0, 20);
	pRun("Task//Task.ini", res);
}


void CtestLedDllDlg::OnBnClickedButton2()
{
	if (pInitFun() == 0)
	{
		AfxMessageBox("DONE");
	}
	else
	{
		AfxMessageBox("Failed");
	}
}


void CtestLedDllDlg::OnBnClickedButton4()
{
	pGrabOneImage();
}


void CtestLedDllDlg::OnBnClickedButton3()
{
	pUinitFun();
}


void CtestLedDllDlg::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	if (pGrabOneImageDrect() == 0)
	{
		AfxMessageBox("DONE");
	}
	else
	{
		AfxMessageBox("FAIL");
	}
}


void CtestLedDllDlg::OnBnClickedButton6()
{
	char res[20];
	memset(res, 0, 20);
	std::string strroot = "E:\\LedDll\\testLedDll\\";
	char p[1024] = { 0 };
	memset(p, '\0', 1024);
	strcpy_s(p, strroot.c_str());
	pRunDirect2(p,"E:\\LedDll\\testLedDll\\Task\\Task.ini", res);
}
