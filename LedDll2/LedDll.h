// LedDll.h : LedDll DLL 的主头文件
//

#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"		// 主符号
#include "opencv2\opencv.hpp"
#include <vector>
using namespace std;
using namespace cv;
#define MAX_NUM 20
#define RES_ERROR -1  //发生错误 
#define RES_OK 0	  //正常返回
#define RES_CAM_CANNOT_OPEN 1 //相机打开失败
#define RES_CAM_GET_FAIL 2 //相机获取图像失败
// CLedDllApp
// 有关此类实现的信息，请参阅 LedDll.cpp
//

class CLedDllApp : public CWinApp
{
public:
	CLedDllApp();

// 重写
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

typedef struct ColorPara {
	CString Name;  //颜色名称
	int ColorType;   //颜色序号
	int returnValue; //返回值
	int valMin; // 最小值
	int valMax; //最大值
	int AreaThr; //面积判定阈值
	int Thr;    //分割算法阈值
	int ClosingSize; //闭运算参数
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
	//检测区域
	int Left;
	int Right;
	int Top;
	int Bottom;
	//相机相关
	double ExpTime;
	//颜色相关
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
//char res[MAX_NUM];  //结果： 0：失败， 成功
