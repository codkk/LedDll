/*
purpose:			�������������װ��ʹ��ͳһ�Ļ���Camera�ӿ�
date:				2017-12-25
author:				�⺺Ȩ
last modification:	2017-12-25	�⺺Ȩ	�½�
					2017-12-26  �⺺Ȩ  �����˽ӿ�
					2018-07-06  �⺺ȫ  �޸�getImage��ʽ������˫����ģʽ
*/
#pragma once
//#include "LightCtrl.h"
#include "CameraApi.h"//����MindVision���SDK����Ҫ���ļ�
//#include "CommDll\MyComm.h" //�������ֹ�Դ������SDK
#ifdef _WIN64
#pragma comment(lib, "..\\MVCAMSDK_X64.lib")
#else
#pragma comment(lib, "MVCAMSDK.lib")
#endif

#define MAX_CAM_NUM 4 //�����������

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
	void InitCam();							//��ʼ��SDK
	void IniDisp(int idx);
	void SetAeState(int idx,BOOL bAeState); //�����Զ��ع⣬true�Զ��ع�
	void SetExposureTime(int idx, double fExposureTime); //�����ع� ΢��
	double GetExposureTime(int idx);					 //����ع�
	//bool SetLight(int idx, int nLight);
	//int  GetLight(int idx);
	//bool SetLight(int idx, int nLight);					 //���ù�Դ���� 0~255
	CameraSdkStatus PauseShow(int idx);					 //��ͣ�ɼ�
	CameraSdkStatus ContinueShow(int idx);				 //�����ɼ�
	CameraSdkStatus SetTriggerMode(int idx, int mode);	 //���òɼ�ģʽ��0�������ɼ���1�������ɼ���2��Ӳ�����ɼ�;  ����ֵ 0���ɹ���
	void SetCross(int idx, bool isVisible);
	bool LoadIni(int idx, char* );
	bool SaveIni(int idx, char* );
	bool LoadIni(int idx, CString);
	bool SaveIni(int idx, CString);
	bool Show();
public:
	BOOL    m_bExit[MAX_CAM_NUM];//����֪ͨͼ��ץȡ�߳̽���
	int		m_hCamera[MAX_CAM_NUM];
	BOOL    m_bFresh[MAX_CAM_NUM];			   //ͼ���Ƿ�û������������swap����ͼ��Ϊfalse��
	BYTE*	m_pFrameBuffer[MAX_CAM_NUM];       //ÿһ�βɼ������ڴ�ָ�룬
	BYTE*   m_pFrameBuffer_cur[MAX_CAM_NUM];   //����Ҫʹ��ͼ��ָ����ͼ����ʱ,���ô�ָ�롣 ˫����ģʽ
	tSdkFrameHead		m_sFrInfo[MAX_CAM_NUM];//���ڱ��浱ǰͼ��֡��֡ͷ��Ϣ
	tSdkFrameHead		m_sFrInfo_cur[MAX_CAM_NUM];//���ڱ��浱ǰͼ��֡��֡ͷ��Ϣ

	UINT    m_threadID[MAX_CAM_NUM];//ͼ��ץȡ�̵߳�ID
	HANDLE  m_hDispThread[MAX_CAM_NUM];//ͼ��ץȡ�̵߳ľ��
	HWND	m_hWndDisp[MAX_CAM_NUM];//��ʾ�տؼ��ľ��
	HWND	m_hWndMainDlg;			//�����ڵľ��
	
	tSdkCameraDevInfo	sCameraList[MAX_CAM_NUM];
	INT					iCameraNums;			//����ĸ���
	CameraSdkStatus		status;
	CRect				rect;
	tSdkCameraCapbility sCameraInfo;
	//CLightCtrl			m_lightctrl;

	double	m_fExposureTime[MAX_CAM_NUM];
	double  m_fLight[MAX_CAM_NUM];  //��Դ����
private:
	void swap(int idx);
};

