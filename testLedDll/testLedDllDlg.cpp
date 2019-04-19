
// testLedDllDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "testLedDll.h"
#include "testLedDllDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CtestLedDllDlg �Ի���



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


// CtestLedDllDlg ��Ϣ�������

BOOL CtestLedDllDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	dllHandle = LoadLibrary(_T("LedDll.dll"));
	if (dllHandle == NULL)
	{
		AfxMessageBox(_T("LOAD FAIL"));
		return TRUE;
	}

	//����dll��ʼ������
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

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CtestLedDllDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CtestLedDllDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CtestLedDllDlg::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
