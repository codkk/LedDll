// LedDll.h : LedDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "opencv2\opencv.hpp"
#include <vector>
using namespace std;
using namespace cv;
#define MAX_NUM 20
#define RES_ERROR -1  //�������� 
#define RES_OK 0	  //��������
#define RES_CAM_CANNOT_OPEN 1 //�����ʧ��
#define RES_CAM_GET_FAIL 2 //�����ȡͼ��ʧ��
// CLedDllApp
// �йش���ʵ�ֵ���Ϣ������� LedDll.cpp
//

class CLedDllApp : public CWinApp
{
public:
	CLedDllApp();

// ��д
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

typedef struct ColorPara {
	CString Name;  //��ɫ����
	int ColorType;   //��ɫ���
	int returnValue; //����ֵ
	int valMin; // ��Сֵ
	int valMax; //���ֵ
	int AreaThr; //����ж���ֵ
	int Thr;    //�ָ��㷨��ֵ
	int ClosingSize; //���������
	ColorPara() {
		ColorType = -1;
		returnValue = 0;
		valMin = 0;
		valMax = 255;
		AreaThr = 10;
		Thr = 40;
		ClosingSize = 10;
		Name = _T("Unknow");
		//memset(Name, '\0', 1024);
	}
}ColorPara;

typedef struct TaskPara {
	//�������
	int Left;
	int Right;
	int Top;
	int Bottom;
	//������
	double ExpTime;
	//��ɫ���
	int ColorType;
	CString Name;
	TaskPara() {
		Left = 0;
		Right = 0;
		Top = 0;
		Bottom = 0;
		ExpTime = 5000.0;
		ColorType = -1;
		Name = "";
	}
}TaskPara;


vector<ColorPara> g_vecPara;
vector<TaskPara> g_vecTask;
CPoint g_winOrg;
MVcamera g_Cam;
cv::Mat g_Image;
//char res[MAX_NUM];  //����� 0��ʧ�ܣ� �ɹ�
