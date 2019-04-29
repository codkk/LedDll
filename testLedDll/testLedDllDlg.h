
// testLedDllDlg.h : 头文件
//

#pragma once

//typedef bool(*Judge)(Mat Image, int Type);
typedef int (*Run)(char* pPath, char* pRes);
typedef int(*RunDirect)(char* pPath, char* pRes);
typedef int(*RunDirect2)(char* pRootPath, char* pPath, char* pRes);
typedef int(*RunDirect3)(char* pRootPath, char* pPath);
typedef bool(*initDll)();
typedef bool(*UinitDll)();
typedef int(*grabOneImage)();
typedef int(*grabOneImageDrect)(); //连接 -》 拍照 -》 断开连接
typedef int(*grabOneImageDrect2)(char* pRootPath); //连接 -》 拍照 -》 断开连接 传入绝对路径

typedef int(*dectectNum)();
typedef int(*showHist)();
// CtestLedDllDlg 对话框
class CtestLedDllDlg : public CDialogEx
{
// 构造
public:
	CtestLedDllDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CtestLedDllDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TESTLEDDLL_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
	grabOneImageDrect2 pGrabOneImageDrect2;
	RunDirect pRunDirect;
	RunDirect2 pRunDirect2;
	RunDirect3 pRunDirect3;
	dectectNum pDectectNum;
	showHist pShowHist;
	HMODULE dllHandle;   //图像处理库的句柄
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	CString m_streditRoot;
	CString m_streditIni;
	afx_msg void OnBnClickedButton7();
};
