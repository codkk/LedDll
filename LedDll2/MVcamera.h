/*
purpose:			迈德威视相机封装，使用统一的基类Camera接口
date:				2017-12-25
author:				吴汉权
last modification:	2017-12-25	吴汉权	新建
					2017-12-26  吴汉权  增加了接口
					2018-07-06  吴汉全  修改getImage方式，增加双缓冲模式
*/
#pragma once
//#include "LightCtrl.h"
#include "CameraApi.h"//引用MindVision相机SDK所需要的文件
//#include "CommDll\MyComm.h" //引用数字光源控制器SDK
#ifdef _WIN64
#pragma comment(lib, "..\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif

#define MAX_CAM_NUM 4 //相机的最多个数

class AFX_EXT_CLASS MVcamera //: public CLightCtrl
	//: public CMyComm//:
//	public Camera
{
public:
	MVcamera();
	~MVcamera();
	bool Connect();
	bool DisConnect();
	//bool ConnectLight(int idx);
	CameraSdkStatus GetImage(int idx, tSdkFrame &Image);
	void SetDispWnd(int idx, HWND hwnd);  //
	HWND GetDispWnd(int idx);
	void InitCam();							//初始化SDK
	void IniDisp(int idx);
	void SetAeState(int idx,BOOL bAeState); //设置自动曝光，true自动曝光
	void SetExposureTime(int idx, double fExposureTime); //设置曝光 微秒
	double GetExposureTime(int idx);					 //获得曝光
	//bool SetLight(int idx, int nLight);
	//int  GetLight(int idx);
	//bool SetLight(int idx, int nLight);					 //设置光源亮度 0~255
	CameraSdkStatus PauseShow(int idx);					 //暂停采集
	CameraSdkStatus ContinueShow(int idx);				 //继续采集
	CameraSdkStatus SetTriggerMode(int idx, int mode);	 //设置采集模式，0，连续采集，1，软触发采集，2，硬触发采集;  返回值 0：成功，
	void SetCross(int idx, bool isVisible);
	bool LoadIni(int idx, char* );
	bool SaveIni(int idx, char* );
	bool LoadIni(int idx, CString);
	bool SaveIni(int idx, CString);
	bool Show();
public:
	BOOL    m_bExit[MAX_CAM_NUM];//用来通知图像抓取线程结束
	int		m_hCamera[MAX_CAM_NUM];
	BOOL    m_bFresh[MAX_CAM_NUM];			   //图像是否还没被交换过。被swap过的图像为false。
	BYTE*	m_pFrameBuffer[MAX_CAM_NUM];       //每一次采集都放在此指针，
	BYTE*   m_pFrameBuffer_cur[MAX_CAM_NUM];   //当需要使用图像指针做图像处理时,将用此指针。 双缓冲模式
	tSdkFrameHead		m_sFrInfo[MAX_CAM_NUM];//用于保存当前图像帧的帧头信息
	tSdkFrameHead		m_sFrInfo_cur[MAX_CAM_NUM];//用于保存当前图像帧的帧头信息

	UINT    m_threadID[MAX_CAM_NUM];//图像抓取线程的ID
	HANDLE  m_hDispThread[MAX_CAM_NUM];//图像抓取线程的句柄
	HWND	m_hWndDisp[MAX_CAM_NUM];//显示空控件的句柄
	HWND	m_hWndMainDlg;			//主窗口的句柄
	
	tSdkCameraDevInfo	sCameraList[MAX_CAM_NUM];
	INT					iCameraNums;			//相机的个数
	CameraSdkStatus		status;
	CRect				rect;
	tSdkCameraCapbility sCameraInfo;
	//CLightCtrl			m_lightctrl;

	double	m_fExposureTime[MAX_CAM_NUM];
	double  m_fLight[MAX_CAM_NUM];  //光源亮度
private:
	void swap(int idx);
};

