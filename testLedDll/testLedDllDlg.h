
// testLedDllDlg.h : ͷ�ļ�
//

#pragma once

//typedef bool(*Judge)(Mat Image, int Type);
typedef int (*Run)(char* pPath, char* pRes);
typedef bool(*initDll)();
typedef bool(*UinitDll)();
typedef int(*grabOneImage)();
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
	HMODULE dllHandle;   //ͼ�����ľ��
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
};
