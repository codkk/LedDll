
// testLedDllDlg.h : ͷ�ļ�
//

#pragma once

//typedef bool(*Judge)(Mat Image, int Type);
typedef int (*Run)(char* pPath, char* pRes);
typedef int(*RunDirect)(char* pPath, char* pRes);
typedef int(*RunDirect2)(char* pRootPath, char* pPath, char* pRes);
typedef bool(*initDll)();
typedef bool(*UinitDll)();
typedef int(*grabOneImage)();
typedef int(*grabOneImageDrect)(); //���� -�� ���� -�� �Ͽ�����
// CtestLedDllDlg �Ի���
class CtestLedDllDlg : public CDialogEx
{
// ����
public:
	CtestLedDllDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CtestLedDllDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTLEDDLL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedButton1();
	//Judge pJudge;
	initDll pInitFun;
	Run pRun;
	grabOneImage pGrabOneImage;
	UinitDll pUinitFun;
	grabOneImageDrect pGrabOneImageDrect;
	RunDirect pRunDirect;
	RunDirect2 pRunDirect2;
	HMODULE dllHandle;   //ͼ�����ľ��
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
};
