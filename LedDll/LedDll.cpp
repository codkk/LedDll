// LedDll.cpp : 定义 DLL 的初始化例程。
//

#include "stdafx.h"
#include "LedDll.h"
#include "LedInterface.h"
#include "opencv2\opencv.hpp"
#include <vector>
#include "Class\Ini.h"
using namespace cv;
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

/////////////////////////////
#define PATH_PARA "Para\\Setting.ini"
#define NODE_SETTING "Setting"
#define NODE_COLOR "Color"
#define SUB_NUM "Num"
#define SUB_DEBUG "Debug"
#define SUB_NAME "Name"
#define SUB_COLOR_TY "ColorType"
#define SUB_RET_VAL "ReturnValue"
#define SUB_VAL_MIN "valMin"
#define SUB_VAL_MAX "valMax"
#define SUB_AREA_THR "AreaThr"
#define SUB_THR "Thr"
#define SUB_CLOSING_SZ "ClosingSize"

//TASK
#define PATH_TASK "Task\\Task.ini"
#define NODE_TASK_SETTING "TaskSetting"
#define NODE_TASK_AREA    "Area"
#define SUB_TASK_NUM "Num"
#define SUB_TASK_NAME "Name"
#define SUB_TASK_COLOR "ColorType"
#define SUB_TASK_EXPTIME "ExpTime"
#define SUB_TASK_BOT "Bottom"
#define SUB_TASK_RHT "Right"
#define SUB_TASK_TOP "Top"
#define SUB_TASK_LFT "Left"

/////////////////////////////


//
//TODO:  如果此 DLL 相对于 MFC DLL 是动态链接的，
//		则从此 DLL 导出的任何调入
//		MFC 的函数必须将 AFX_MANAGE_STATE 宏添加到
//		该函数的最前面。
//
//		例如: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// 此处为普通函数体
//		}
//
//		此宏先于任何 MFC 调用
//		出现在每个函数中十分重要。  这意味着
//		它必须作为函数中的第一个语句
//		出现，甚至先于所有对象变量声明，
//		这是因为它们的构造函数可能生成 MFC
//		DLL 调用。
//
//		有关其他详细信息，
//		请参阅 MFC 技术说明 33 和 58。
//

// CLedDllApp

BEGIN_MESSAGE_MAP(CLedDllApp, CWinApp)
END_MESSAGE_MAP()


// CLedDllApp 构造

CLedDllApp::CLedDllApp()
{
	// TODO:  在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CLedDllApp 对象

CLedDllApp theApp;


// CLedDllApp 初始化

BOOL CLedDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
///////////////////////////////////////////
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


////////////////////////////////////
//全局变量
//显示相关
Mat g_ImageSrc;	//原图
Mat g_ImageHue; //色调0-180


//算法相关


//流程相关
BOOL g_bDebug = FALSE;
vector<ColorPara> g_vecPara;
vector<TaskPara> g_vecTask;

////////////////////////////////////
//加载标准参数
bool LoadColorParas(char* path)
{
	CIni file;
	bool ret = file.Read(path);
	if (!ret) return false;

	//
	g_vecPara.clear();
	file.GetValue(NODE_SETTING, SUB_DEBUG, g_bDebug);
	int num = 0;
	file.GetValue(NODE_SETTING, SUB_NUM, num);
	for (int i = 0; i < num; i++)
	{
		//
		ColorPara para;
		char node[1024] = { 0 };
		std::string str = NODE_COLOR;
		sprintf_s(node, "%s%d", str.c_str(), i+1);
		file.GetValue(node, SUB_NAME, para.Name);
		file.GetValue(node, SUB_COLOR_TY, para.ColorType);
		file.GetValue(node, SUB_RET_VAL, para.returnValue);
		file.GetValue(node, SUB_VAL_MIN, para.valMin);
		file.GetValue(node, SUB_VAL_MAX, para.valMax);
		file.GetValue(node, SUB_AREA_THR, para.AreaThr);
		file.GetValue(node, SUB_THR, para.Thr);
		file.GetValue(node, SUB_CLOSING_SZ, para.ClosingSize);
		g_vecPara.push_back(para);
	}

	return true;
}

//加载任务列表
bool LoadTaskParas(char* path)
{
	CIni file;
	bool ret = file.Read(path);
	if (!ret) return false;

	//
	g_vecTask.clear();

	int num = 0;
	file.GetValue(NODE_TASK_SETTING, SUB_TASK_NUM, num);
	for (int i = 0; i < num; i++)
	{
		//
		TaskPara task;
		char node[1024] = { 0 };
		std::string str = NODE_TASK_AREA;
		sprintf_s(node, "%s%d", str.c_str(), i + 1);
		file.GetValue(node, SUB_TASK_NAME, task.Name);
		file.GetValue(node, SUB_TASK_COLOR, task.ColorType);
		file.GetValue(node, SUB_TASK_EXPTIME, task.ExpTime);
		file.GetValue(node, SUB_TASK_BOT, task.Bottom);
		file.GetValue(node, SUB_TASK_RHT, task.Right);
		file.GetValue(node, SUB_TASK_TOP, task.Top);
		file.GetValue(node, SUB_TASK_LFT, task.Left);
		g_vecTask.push_back(task);
	}

	return true;
}


//函数： 计算并绘制直方图
bool DrawHist(Mat& src, Mat& dst)
{
	//定义变量
	MatND dstHist;
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };
	int size = 256;
	int channels = 0;

	//计算直方图
	calcHist(&src, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;

	//最小最大值
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);

	//绘制直方图
	int hpt = saturate_cast<int> (0.9*size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);
		int realValue = saturate_cast<int> (binValue * hpt / maxValue);
		rectangle(dst, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(255,255,255));
		
		if (i % 10 == 0)
		{
			if (i % 50 == 0)
			{
				rectangle(dst, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size-20), Scalar(0, 0, 255));
			}
			else
			{
				rectangle(dst, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size-10), Scalar(0, 0, 255));
			}
		}
	}
	return true;
}



//函数: 判断图像某以区域的颜色
//
//
bool JudgeColor(Mat& ImgSrc, TaskPara& Task)
{
	//0. 切图
	cv::Rect roi = cv::Rect(Task.Left, Task.Right, Task.Right-Task.Left, Task.Bottom-Task.Top);
	Mat roiImg = ImgSrc(roi);
	//imshow("roi", roiImg);
	//waitKey(0);

	//1. 转换色彩空间,获得色调图0到180
	Mat dstImage;
	cvtColor(roiImg, dstImage, COLOR_BGR2HLS);
	
	vector<Mat> channels;
	Mat imageHue;
	split(dstImage, channels);
	imageHue = channels.at(0);

	//2. 色调直方图(可不用)
	Mat dst(256, 256, CV_8UC3, Scalar(0));
	DrawHist(imageHue,dst);
	imshow("dst", dst);
	//waitKey();

	//3. 二值化并计算面积
	int colorTy = Task.ColorType;
	int min = 0;
	int max = 0;
	bool flag = false;
	int idx = 0;
	for (idx = 0; idx < g_vecPara.size(); idx++)
	{
		if (colorTy == g_vecPara[idx].ColorType)
		{
			flag = true;
			break;
		}
	}
	if (!flag) return false;

	ColorPara stdPara = g_vecPara.at(idx);
	min = stdPara.valMin;
	max = stdPara.valMax;

	//最大xian值小于最小限值，例如180~40，指的是 180到255， 0到40这两段二值化。
	Mat binImg;
	if (max < min)
	{
		Mat Image1;
		Mat Image2;
		threshold(imageHue, Image1, min, 255, THRESH_BINARY);
		threshold(imageHue, Image2, max, 255, THRESH_BINARY_INV);
		bitwise_or(Image1, Image2, binImg);
	}
	else//例如：40~180 指的是 【40，180】之间的部分
	{
		Mat Image1;
		Mat Image2;
		threshold(imageHue, Image1, min, 255, THRESH_BINARY);
		threshold(imageHue, Image2, max, 255, THRESH_BINARY_INV);
		bitwise_and(Image1, Image2, binImg);
	}

	imshow("binImg", binImg);
	//waitKey(0);

	//4. 计算面积
	Moments mu;
	mu = moments(binImg);
	double area = mu.m00;

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	RNG rng;
	area = 0.0;
	findContours(binImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// Draw contours
	Mat drawing = Mat::zeros(binImg.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));		
		drawContours(roiImg, contours, i, Scalar(255,255,0), 2, 8, hierarchy, 0, Point());
		area +=(contourArea(contours[i]));
	}
	imshow("roi", roiImg);
	imwrite("roi.jpg", roiImg);
	waitKey(0);
	if (area >= (double)stdPara.AreaThr)
		return true;
	else
		return false;
	//return true;
}




bool InitDll()
{
	LoadColorParas(PATH_PARA);
	return true;
}

bool UinitDll()
{
	return true;
}

int Run(char* pPath, char* pRes)
{
	//加载任务
	g_vecTask.clear();
	LoadTaskParas(PATH_TASK);
	//拍照
	g_ImageSrc = imread("test2.jpg");
	imshow("src", g_ImageSrc);
	//waitKey();
	//根据任务识别颜色
	vector<bool> vctRes;
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		TaskPara TaskTmp = g_vecTask.at(i);
		bool ret = JudgeColor(g_ImageSrc, TaskTmp);
		pRes[i] = ret;
		Sleep(1000);
		vctRes.push_back(ret);
	}
	//显示结果

	return 0;
}