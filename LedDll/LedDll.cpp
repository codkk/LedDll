// LedDll.cpp : ���� DLL �ĳ�ʼ�����̡�
//

#include "stdafx.h"
#include "LedDll.h"
#include "LedInterface.h"
#include "opencv2\opencv.hpp"
#include <vector>
#include "Class\Ini.h"
#include "EasyLog.h"
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

//Window
#define WIN_SRC_IMG "Src"
#define WIN_BIN_IMG "Bin"
#define WIN_HIS_IMG "Hst"
#define WIN_ROI_IMG "Roi"

/////////////////////////////


//
//TODO:  ����� DLL ����� MFC DLL �Ƕ�̬���ӵģ�
//		��Ӵ� DLL �������κε���
//		MFC �ĺ������뽫 AFX_MANAGE_STATE ����ӵ�
//		�ú�������ǰ�档
//
//		����: 
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// �˴�Ϊ��ͨ������
//		}
//
//		�˺������κ� MFC ����
//		������ÿ��������ʮ����Ҫ��  ����ζ��
//		��������Ϊ�����еĵ�һ�����
//		���֣������������ж������������
//		������Ϊ���ǵĹ��캯���������� MFC
//		DLL ���á�
//
//		�й�������ϸ��Ϣ��
//		����� MFC ����˵�� 33 �� 58��
//

// CLedDllApp

BEGIN_MESSAGE_MAP(CLedDllApp, CWinApp)
END_MESSAGE_MAP()


// CLedDllApp ����

CLedDllApp::CLedDllApp()
{
	// TODO:  �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CLedDllApp ����

CLedDllApp theApp;

typedef int(*CheckMe)();
typedef int(*RegistMe)();
HMODULE dllHandle;
CheckMe pCheckme = NULL;
RegistMe pRegistme = NULL;
// CLedDllApp ��ʼ��

BOOL CLedDllApp::InitInstance()
{
	CWinApp::InitInstance();

	return TRUE;
}
///////////////////////////////////////////
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
//���
typedef struct ResultPara {
	bool bOk;
	double Area;
	ResultPara() {
		bOk = true;
		Area = 0.0;
	}
}ResultPara;
////////////////////////////////////
//ȫ�ֱ���
//��ʾ���
Mat g_ImageSrc;	//ԭͼ
Mat g_ImageHue; //ɫ��0-180
MVcamera m_Cam;	//���1
//MyLogger * m_myLoger = MyLogger::getInstance();
std::string g_strRootPath;

//�㷨���


//�������
BOOL g_bDebug = FALSE;
vector<ColorPara> g_vecPara;
vector<TaskPara> g_vecTask;
vector<ResultPara> g_vecResult;
////////////////////////////////////
//���ر�׼����
bool LoadColorParas(const char* path)
{
	EasyLog::Inst()->Log("LoadColorParas:");
	EasyLog::Inst()->Log(path);
	CIni file;
	bool ret = file.Read(path);
	if (!ret)
	{
		EasyLog::Inst()->Log("LoadColorParas: ret false");
		return false;
	}

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
	EasyLog::Inst()->Log("LoadColorParas: ret true");
	return true;
}

//���������б�
bool LoadTaskParas(char* path)
{
	EasyLog::Inst()->Log("LoadTaskParas��");
	EasyLog::Inst()->Log(path);
	CIni file;
	bool ret = file.Read(path);
	if (!ret)
	{
		EasyLog::Inst()->Log("LoadTaskParas��ret false");
		return false;
	}


	//
	g_vecTask.clear();

	int num = 0;
	file.GetValue(NODE_TASK_SETTING, SUB_TASK_NUM, num);
	for (int i = 0; i < num; i++)
	{
		EasyLog::Inst()->Log("����һ��");
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
	EasyLog::Inst()->Log("LoadTaskParas��ret true");
	return true;
}


//������ ���㲢����ֱ��ͼ
bool DrawHist(Mat& src, Mat& dst)
{
	//�������
	MatND dstHist;
	int dims = 1;
	float hranges[] = { 0, 255 };
	const float *ranges[] = { hranges };
	int size = 256;
	int channels = 0;

	//����ֱ��ͼ
	calcHist(&src, 1, &channels, Mat(), dstHist, dims, &size, ranges);
	int scale = 1;

	//��С���ֵ
	double minValue = 0;
	double maxValue = 0;
	minMaxLoc(dstHist, &minValue, &maxValue, 0, 0);

	//����ֱ��ͼ
	int hpt = saturate_cast<int> (0.9*size);
	for (int i = 0; i < 256; i++)
	{
		float binValue = dstHist.at<float>(i);
		int realValue = saturate_cast<int> (binValue * hpt / maxValue);
		rectangle(dst, Point(i*scale, size - 1), Point((i + 1)*scale - 1, size - realValue), Scalar(0,255,0));
		
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



//����: �ж�ͼ��ĳ���������ɫ
//
//
int  JudgeColor(Mat& ImgSrc, TaskPara& Task, ResultPara& result)
{
	EasyLog::Inst()->Log("JudgeColor:start");
	//0. ��ͼ
	cv::Rect roi = cv::Rect(Task.Left, Task.Top, Task.Right-Task.Left, Task.Bottom-Task.Top);
	Mat roiImg = ImgSrc(roi);
	cv::imshow(WIN_ROI_IMG, roiImg);
	imwrite("roi.jpg", roiImg);
	//���
	//Mat roiImg = Mat()

	//imshow("roi", roiImg);
	//waitKey(0);

	//1. ת��ɫ�ʿռ�,���ɫ��ͼ0��180
	Mat dstImage;
	cvtColor(roiImg, dstImage, COLOR_BGR2HLS);
	
	//vector<Mat> channels;
	Mat channels[3];
	channels[0] = Mat::zeros(dstImage.size(), CV_8UC1);
	channels[1] = Mat::zeros(dstImage.size(), CV_8UC1);
	channels[2] = Mat::zeros(dstImage.size(), CV_8UC1);
	Mat imageHue;
	cv::split(dstImage, channels);
	imageHue = channels[0];

	//1.1 ת��ΪRGB
	Mat channelsBGR[3];
	channelsBGR[0] = Mat::zeros(dstImage.size(), CV_8UC1);
	channelsBGR[1] = Mat::zeros(dstImage.size(), CV_8UC1);
	channelsBGR[2] = Mat::zeros(dstImage.size(), CV_8UC1);
	Mat imageR,imageB, imageG;
	cv::split(dstImage, channelsBGR);
	imageB = channelsBGR[0];
	imageG = channelsBGR[1];
	imageR = channelsBGR[2];
	//imshow("R", imageR);
	//imshow("B", imageB);
	//imshow("G", imageG);
	//waitKey();

	//2. ɫ��ֱ��ͼ(�ɲ���)
	Mat dst(256, 256, CV_8UC3, Scalar(0));
	DrawHist(imageHue,dst);
	cv::imshow(WIN_HIS_IMG, dst);
	//waitKey();
	
	//3. ��ֵ�����������
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
	if (!flag)
	{
		EasyLog::Inst()->Log("JudgeColor:ret ERR_COLOR �����������ɫ���Ͳ�����");
		return ERR_COLOR;
	}
	ColorPara stdPara = g_vecPara.at(idx);
	min = stdPara.valMin;
	max = stdPara.valMax;

	//����Ǻ�ɫ
	if (colorTy == 1)
	{
		imageHue = imageR.clone();
	}

	//���xianֵС����С��ֵ������180~40��ָ���� 180��255�� 0��40�����ζ�ֵ����
	Mat binImg;
	Mat closeImg;
	if (max < min)
	{
		Mat Image1;
		Mat Image2;
		threshold(imageHue, Image1, min, 255, THRESH_BINARY);
		//cv::imshow(WIN_BIN_IMG, Image1);
		//waitKey(0);
		threshold(imageHue, Image2, max, 255, THRESH_BINARY_INV);
		//cv::imshow(WIN_BIN_IMG, Image2);
		//waitKey(0);
		bitwise_or(Image1, Image2, binImg);
	}
	else//���磺40~180 ָ���� ��40��180��֮��Ĳ���
	{
		Mat Image1;
		Mat Image2;
		threshold(imageHue, Image1, min, 255, THRESH_BINARY);
		//cv::imshow(WIN_BIN_IMG, Image1);
		//waitKey(0);
		threshold(imageHue, Image2, max, 255, THRESH_BINARY_INV);
		//cv::imshow(WIN_BIN_IMG, Image2);
		//waitKey(0);
		bitwise_and(Image1, Image2, binImg);
	}
	//��ʴ
	Mat element = getStructuringElement(MORPH_RECT, Size(stdPara.ClosingSize, stdPara.ClosingSize));
	cv::erode(binImg, binImg, element);
	cv::imshow(WIN_BIN_IMG, binImg);
	//waitKey(0);

	//4. �������
	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	RNG rng;
	double area = 0.0;
	cv::findContours(binImg, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
	/// Draw contours
	Mat drawing = Mat::zeros(binImg.size(), CV_8UC3);
	for (int i = 0; i< contours.size(); i++)
	{
		//Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));		
		//drawContours(roiImg, contours, i, Scalar(255,255,0), 2, 8, hierarchy, 0, Point());
		area +=(contourArea(contours[i]));
	}
	result.Area = area;
	//waitKey(0);
	if (area >= stdPara.AreaThr)
	{
		result.bOk = true;
		EasyLog::Inst()->Log("JudgeColor:ret OK");
		return 1;
	}
	else
	{
		result.bOk = false;
		EasyLog::Inst()->Log("JudgeColor:ret NG");
		return 0;
	}

	EasyLog::Inst()->Log("JudgeColor:ret ERR_COLOR �����ܵ�����");
	return ERR_COLOR;
}




int InitDll()
{
	//std::string str = "InitDll:Start";
	//char c[] = "InitDll:Start";
	////LOG4CPLUS_DEBUG(m_myLoger->logger, c);
	//
	CString str;
	str.Format(_T("%syance0.dll"), g_strRootPath);
	dllHandle = LoadLibrary(str);
	if (dllHandle == NULL)
	{
		AfxMessageBox("LOAD dll FAIL");
		return ERR_OTHER;
	}
	//����dll��ʼ������
	pCheckme = (CheckMe)::GetProcAddress(dllHandle, "check2");
	if (pCheckme == NULL) {
		AfxMessageBox("function  load failed!\n");
		FreeLibrary(dllHandle);
		return ERR_OTHER;
	}
	pRegistme = (RegistMe)::GetProcAddress(dllHandle, "registerMe2");
	if (pRegistme == NULL) {
		AfxMessageBox("function  load failed!\n");
		FreeLibrary(dllHandle);
		return ERR_OTHER;
	}
	int re = pCheckme();
	if (re != 0)
	{
		if (-1 == re)
		{
			if (pRegistme())
			{
				AfxMessageBox("ע��fail");
				return ERR_OTHER;
			}
			else
			{

			}
		}
		else
		{
			AfxMessageBox("UnRegist!\n");
			return ERR_OTHER;
		}
	}


	EasyLog::Inst()->Log("InitDll:start");
	bool ret = true;
	m_Cam.InitCam();
	EasyLog::Inst()->Log("InitDll:�������...");
	if (!m_Cam.Connect())
	{
		AfxMessageBox(_T("������Ӳ��ɹ�"));
		//return FALSE;
		EasyLog::Inst()->Log("InitDll:������Ӳ��ɹ�");
		ret = false;
		//LOG4CPLUS_INFO(m_myLoger->logger, "InitDll:Ret:������Ӳ��ɹ�");
	}
	else
	{
		//���ز����ļ�
		std::string str = g_strRootPath + "Para\\myconfig.Config";
		char p[1024] = { 0 };
		memset(p, '\0', 1024);
		strcpy(p, str.c_str());
		EasyLog::Inst()->Log("InitDll:�����������...");
		if (!m_Cam.LoadIni(0, p))
		{
			AfxMessageBox(_T("��������������ɹ�"));
			EasyLog::Inst()->Log("InitDll:��������������ɹ�");
			ret = false;
			//LOG4CPLUS_INFO(m_myLoger->logger, "InitDll:Ret:��������������ɹ�");
		}
	}
	if (!ret) return ERR_OTHER;
	std::string str2 = g_strRootPath + PATH_PARA;
	ret = LoadColorParas(str2.c_str());
	if (ret)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "InitDll:Ret:Sucess");
		EasyLog::Inst()->Log("InitDll:ret ERR_SUCCESS");
		return ERR_SUCCESS;
	}
	else
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "InitDll:Ret:������ɫ��׼�������ɹ�");
		EasyLog::Inst()->Log("InitDll:ret ERR_OTHER");
		return ERR_OTHER;
	}

}

int UinitDll()
{
	EasyLog::Inst()->Log("UinitDll:start");
	if (m_Cam.DisConnect())
	{
		EasyLog::Inst()->Log("UinitDll:ret ERR_SUCCESS");
		return ERR_SUCCESS;
	}
	else
	{
		EasyLog::Inst()->Log("UinitDll:ret ERR_CAM");
		return ERR_CAM;
	}

}

int Run(char* pPath, char* pRes)
{
	EasyLog::Inst()->Log("Run:start");
	//��������
	g_vecTask.clear();
	g_vecResult.clear();
	if (!LoadTaskParas(pPath))
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�");
		EasyLog::Inst()->Log("Run:ret ��������������ɹ�");
		return ERR_TASK;
	}
	if (0 == g_vecTask.size())
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�:�������Ϊ��");
		EasyLog::Inst()->Log("Run:ret ��������������ɹ�:�������Ϊ��");
		return ERR_TASK;
	}
	//����
	//��ȡͼ��
	tSdkFrame Image;
	CameraSdkStatus st = m_Cam.GetImage(0, Image);
	if (st != CAMERA_STATUS_SUCCESS)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:������մ���");
		EasyLog::Inst()->Log("Run:ret ������մ���");
		return ERR_CAM;
	}
	cv::Mat tmp2(
		cvSize(Image.head.iWidth, Image.head.iHeight),
		Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		Image.pBuffer
	);
	//���·�ת
	cv::flip(tmp2, tmp2, 0);
	g_ImageSrc = tmp2;
	//g_ImageSrc = imread("test2.jpg");
	imwrite("src.jpg", g_ImageSrc);

	//��������
	namedWindow(WIN_SRC_IMG, 0);
	resizeWindow(WIN_SRC_IMG, 640, 480);
	moveWindow(WIN_SRC_IMG, 0, 0);

	namedWindow(WIN_ROI_IMG, 0);
	resizeWindow(WIN_ROI_IMG, 320, 240);
	moveWindow(WIN_ROI_IMG, 640, 0);

	namedWindow(WIN_HIS_IMG, 0);
	resizeWindow(WIN_HIS_IMG, 320, 240);
	moveWindow(WIN_HIS_IMG, 640+320, 0);

	namedWindow(WIN_BIN_IMG, 0);
	resizeWindow(WIN_BIN_IMG, 320, 240);
	moveWindow(WIN_BIN_IMG, 640+320+320, 0);

	cv::imshow(WIN_SRC_IMG, g_ImageSrc);

	//��������ʶ����ɫ
	vector<bool> vctRes;
	bool bSuccess = true;
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������ʶ��һ��");
		EasyLog::Inst()->Log("Run: ��������ʶ��һ��");
		TaskPara TaskTmp = g_vecTask.at(i);
		ResultPara Result;
		int ret = JudgeColor(g_ImageSrc, TaskTmp, Result);
		bool bret = false;
		if (ret == 1)
		{
			pRes[i] = 1;
			EasyLog::Inst()->Log("Run: OK");
			bret = true;
		}
		else if (ret == 0)
		{
			pRes[i] = 0;
			EasyLog::Inst()->Log("Run: NG");
			bret = false;
		}
		else
		{
			cv::destroyAllWindows();
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��ɫʶ��������");
			EasyLog::Inst()->Log("Run: ret ��ɫʶ��������");
			return ERR_COLOR;
		}

		bSuccess &= bret;
		if(g_bDebug)
			cv::waitKey(1000);
		vctRes.push_back(bret);
		g_vecResult.push_back(Result);
	}

	//����ʧ�ܽ��ͼ��JPG
	if (!bSuccess)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:����ʶ��ʧ��ͼƬ");
		EasyLog::Inst()->Log("Run: ����ʶ��ʧ��ͼƬ");
		//ʱ������
		SYSTEMTIME curT;
		GetLocalTime(&curT);
		CString strPath;
		strPath.Format("%d-%d-%d_%d-%d-%d.jpg", curT.wYear, curT.wMonth, curT.wDay, curT.wHour, curT.wMinute, curT.wSecond);
		imwrite(strPath.GetBuffer(), g_ImageSrc);
		strPath.ReleaseBuffer();
	}
	//��ʾ���,ͼ�񣬱����ı�
	//���û����ı�����ز���
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������result.txt");
	FILE *pFile = fopen("result.txt", "w");
	FILE *pFile2 = fopen("result_area.txt", "w");
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		std::string text = "UnKnown";
		cv::Scalar color = cv::Scalar(255, 255, 0);
		if (i < g_vecResult.size())
		{
			if (g_vecResult[i].bOk)
			{
				text = "OK";
				color = cv::Scalar(0, 255, 0);
				fprintf(pFile, "OK\n");
				fprintf(pFile2, "OK,%.2lf\n", g_vecResult[i].Area);
			}
			else
			{
				text = "NG";
				color = cv::Scalar(0, 0, 255);
				fprintf(pFile, "NG\n");
				fprintf(pFile2, "NG,%.2lf\n", g_vecResult[i].Area);
			}
		}
		else
		{
			fprintf(pFile, "Unknown\n");
			fprintf(pFile2, "Unknown, 0\n");
		}
		
		int font_face = cv::FONT_HERSHEY_COMPLEX;
		double font_scale = 4;
		int thickness = 4;
		int baseline;
		cv::Point origin;
		origin.x = (g_vecTask[i].Left + g_vecTask[i].Right)/2;
		origin.y = (g_vecTask[i].Top + g_vecTask[i].Bottom)/2;
		cv::putText(g_ImageSrc, text, origin, font_face, font_scale, color, thickness, 8, 0);
	}
	fclose(pFile);
	fclose(pFile2);
	cv::imshow(WIN_SRC_IMG, g_ImageSrc);
	cv::waitKey(1000);

	//�رմ���
	cv::destroyAllWindows();
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:Success");
	EasyLog::Inst()->Log("Run: ret ERR_SUCCESS");
	return ERR_SUCCESS;
}

int GrabOneImage()
{
	//��ȡͼ��
	tSdkFrame Image;
	CameraSdkStatus st = m_Cam.GetImage(0, Image);
	if (st != CAMERA_STATUS_SUCCESS)
	{
		return ERR_CAM;
	}
	cv::Mat tmp2(
		cvSize(Image.head.iWidth, Image.head.iHeight),
		Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		Image.pBuffer
	);
	//���·�ת
	cv::flip(tmp2, tmp2, 0);
	//g_ImageSrc = tmp2;
	//g_ImageSrc = imread("test2.jpg");
	imwrite("GrabOneImg.jpg", tmp2);
	return ERR_SUCCESS;
}

int GrabOneImageDirect()
{
	EasyLog::Inst()->Log("GrabOneImageDirect:start");

	//��ʼ��
	int ret1 = InitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("GrabOneImageDirect:ret ERR");
		return ret1;
	}

	//��ȡͼ��
	//Sleep(1000);
	tSdkFrame Image;
	CameraSdkStatus st = m_Cam.GetImage(0, Image);
	if (st != CAMERA_STATUS_SUCCESS)
	{
		EasyLog::Inst()->Log("GrabOneImageDirect:ret �����ȡͼ�񲻳ɹ�");
		return ERR_CAM;
	}
	cv::Mat tmp2(
		cvSize(Image.head.iWidth, Image.head.iHeight),
		Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		Image.pBuffer
	);
	//���·�ת
	cv::flip(tmp2, tmp2, 0);
	//g_ImageSrc = tmp2;
	//g_ImageSrc = imread("test2.jpg");
	imwrite("GrabOneImg.jpg", tmp2);
	//Sleep(1000);
	//����ʼ��
	ret1 = UinitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("GrabOneImageDirect:ret ERR");
		return ret1;
	}


	EasyLog::Inst()->Log("GrabOneImageDirect:ret ERR_SUCCESS");
	return ERR_SUCCESS;
}

int GrabOneImageDirect2(char * rootpath)
{
	EasyLog::Inst()->Log("GrabOneImageDirect:start");
	g_strRootPath = rootpath;
	//��ʼ��
	int ret1 = InitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("GrabOneImageDirect:ret ERR");
		return ret1;
	}

	//��ȡͼ��
	//Sleep(1000);
	tSdkFrame Image;
	CameraSdkStatus st = m_Cam.GetImage(0, Image);
	if (st != CAMERA_STATUS_SUCCESS)
	{
		EasyLog::Inst()->Log("GrabOneImageDirect:ret �����ȡͼ�񲻳ɹ�");
		return ERR_CAM;
	}
	cv::Mat tmp2(
		cvSize(Image.head.iWidth, Image.head.iHeight),
		Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		Image.pBuffer
	);
	//���·�ת
	cv::flip(tmp2, tmp2, 0);
	//g_ImageSrc = tmp2;
	//g_ImageSrc = imread("test2.jpg");
	std::string str = g_strRootPath + "GrabOneImg.jpg";
	imwrite(str.c_str(), tmp2);
	//Sleep(1000);
	//����ʼ��
	ret1 = UinitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("GrabOneImageDirect:ret ERR");
		return ret1;
	}


	EasyLog::Inst()->Log("GrabOneImageDirect:ret ERR_SUCCESS");
	return ERR_SUCCESS;
}

int RunDirect(char * pPath, char * pRes)
{
	EasyLog::Inst()->Log("RunDirect:start*");
	//��ʼ��
	int ret1 = InitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("RunDirect:ret ERR");
		return ret1;
	}


	//��������
	g_vecTask.clear();
	g_vecResult.clear();
	if (!LoadTaskParas(pPath))
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�");
		return ERR_TASK;
	}
	if (0 == g_vecTask.size())
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�:�������Ϊ��");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�:�������Ϊ��");
		return ERR_TASK;
	}
	//����
	//��ȡͼ��
	tSdkFrame Image;
	CameraSdkStatus st = m_Cam.GetImage(0, Image);
	if (st != CAMERA_STATUS_SUCCESS)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:������մ���");
		EasyLog::Inst()->Log("RunDirect:ret ������մ���");
		return ERR_CAM;
	}
	cv::Mat tmp2(
		cvSize(Image.head.iWidth, Image.head.iHeight),
		Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		Image.pBuffer
	);
	//���·�ת
	cv::flip(tmp2, tmp2, 0);
	g_ImageSrc = tmp2;
	//g_ImageSrc = imread("test2.jpg");
	cv::imwrite("src.jpg", g_ImageSrc);

	//��������
	cv::namedWindow(WIN_SRC_IMG, 0);
	cv::resizeWindow(WIN_SRC_IMG, 640, 480);
	cv::moveWindow(WIN_SRC_IMG, 0, 0);

	cv::namedWindow(WIN_ROI_IMG, 0);
	cv::resizeWindow(WIN_ROI_IMG, 320, 240);
	cv::moveWindow(WIN_ROI_IMG, 640, 0);

	cv::namedWindow(WIN_HIS_IMG, 0);
	cv::resizeWindow(WIN_HIS_IMG, 320, 240);
	cv::moveWindow(WIN_HIS_IMG, 640 + 320, 0);

	cv::namedWindow(WIN_BIN_IMG, 0);
	cv::resizeWindow(WIN_BIN_IMG, 320, 240);
	cv::moveWindow(WIN_BIN_IMG, 640 + 320 + 320, 0);

	cv::imshow(WIN_SRC_IMG, g_ImageSrc);

	//��������ʶ����ɫ
	vector<bool> vctRes;
	bool bSuccess = true;
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������ʶ��һ��");
		TaskPara TaskTmp = g_vecTask.at(i);
		ResultPara Result;
		int ret = JudgeColor(g_ImageSrc, TaskTmp, Result);
		bool bret = false;
		if (ret == 1)
		{
			pRes[i] = 1;
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:OK");
			EasyLog::Inst()->Log("RunDirect:OK");
			bret = true;
		}
		else if (ret == 0)
		{
			pRes[i] = 0;
			EasyLog::Inst()->Log("RunDirect:NG");
			bret = false;
		}
		else
		{
			cv::destroyAllWindows();
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��ɫʶ��������");
			EasyLog::Inst()->Log("RunDirect:ret ��ɫʶ��������");
			return ERR_COLOR;
		}

		bSuccess &= bret;
		if (g_bDebug)
			cv::waitKey(1000);
		vctRes.push_back(bret);
		g_vecResult.push_back(Result);
	}

	//����ʧ�ܽ��ͼ��JPG
	if (!bSuccess)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:����ʶ��ʧ��ͼƬ");
		//ʱ������
		SYSTEMTIME curT;
		GetLocalTime(&curT);
		CString strPath;
		strPath.Format("%d-%d-%d_%d-%d-%d.jpg", curT.wYear, curT.wMonth, curT.wDay, curT.wHour, curT.wMinute, curT.wSecond);
		cv::imwrite(strPath.GetBuffer(), g_ImageSrc);
		strPath.ReleaseBuffer();
	}
	//��ʾ���,ͼ�񣬱����ı�
	//���û����ı�����ز���
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������result.txt");
	FILE *pFile = fopen("result.txt", "w");
	FILE *pFile2 = fopen("result_area.txt", "w");
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		std::string text = "UnKnown";
		cv::Scalar color = cv::Scalar(255, 255, 0);
		if (i < g_vecResult.size())
		{
			if (g_vecResult[i].bOk)
			{
				text = "OK";
				color = cv::Scalar(0, 255, 0);
				fprintf_s(pFile, "OK\n");
				fprintf_s(pFile2, "OK,%.2lf\n", g_vecResult[i].Area);
			}
			else
			{
				text = "NG";
				color = cv::Scalar(0, 0, 255);
				fprintf_s(pFile, "NG\n");
				fprintf_s(pFile2, "NG,%.2lf\n", g_vecResult[i].Area);
			}
		}
		else
		{
			fprintf_s(pFile, "Unknown\n");
			fprintf_s(pFile2, "Unknown, 0\n");
		}

		int font_face = cv::FONT_HERSHEY_COMPLEX;
		double font_scale = 4;
		int thickness = 4;
		int baseline;
		cv::Point origin;
		origin.x = (g_vecTask[i].Left + g_vecTask[i].Right) / 2;
		origin.y = (g_vecTask[i].Top + g_vecTask[i].Bottom) / 2;
		cv::putText(g_ImageSrc, text, origin, font_face, font_scale, color, thickness, 8, 0);
	}
	fclose(pFile);
	fclose(pFile2);
	cv::imshow(WIN_SRC_IMG, g_ImageSrc);
	cv::waitKey(1000);

	//�رմ���
	cv::destroyAllWindows();
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:Success");
	
	//����ʼ��
	ret1 = UinitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("RunDirect:ret ERR");
		return ret1;
	}


	EasyLog::Inst()->Log("RunDirect:ret ERR_SUCCESS");
	return ERR_SUCCESS;
}

int RunDirect2(char* rootpath, char * pPath, char * pRes)
{
	EasyLog::Inst()->Log("RunDirect:start*");

	g_strRootPath = rootpath;
	//��ʼ��
	int ret1 = InitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("RunDirect:ret ERR");
		return ret1;
	}


	//��������
	g_vecTask.clear();
	g_vecResult.clear();
	if (!LoadTaskParas(pPath))
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�");
		UinitDll();
		return ERR_TASK;
	}
	if (0 == g_vecTask.size())
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�:�������Ϊ��");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�:�������Ϊ��");
		UinitDll();
		return ERR_TASK;
	}
	//����
	//��ȡͼ��
	tSdkFrame Image;
	CameraSdkStatus st = m_Cam.GetImage(0, Image);
	if (st != CAMERA_STATUS_SUCCESS)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:������մ���");
		EasyLog::Inst()->Log("RunDirect:ret ������մ���");
		UinitDll();
		return ERR_CAM;
	}
	cv::Mat tmp2(
		cvSize(Image.head.iWidth, Image.head.iHeight),
		Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		Image.pBuffer
	);
	//���·�ת
	cv::flip(tmp2, tmp2, 0);
	g_ImageSrc = tmp2;
	//g_ImageSrc = imread("test2.jpg");
	cv::imwrite("src.jpg", g_ImageSrc);

	//��������
	cv::namedWindow(WIN_SRC_IMG, 0);
	cv::resizeWindow(WIN_SRC_IMG, 640, 480);
	cv::moveWindow(WIN_SRC_IMG, 0, 0);

	cv::namedWindow(WIN_ROI_IMG, 0);
	cv::resizeWindow(WIN_ROI_IMG, 320, 240);
	cv::moveWindow(WIN_ROI_IMG, 640, 0);

	cv::namedWindow(WIN_HIS_IMG, 0);
	cv::resizeWindow(WIN_HIS_IMG, 320, 240);
	cv::moveWindow(WIN_HIS_IMG, 640 + 320, 0);

	cv::namedWindow(WIN_BIN_IMG, 0);
	cv::resizeWindow(WIN_BIN_IMG, 320, 240);
	cv::moveWindow(WIN_BIN_IMG, 640 + 320 + 320, 0);

	cv::imshow(WIN_SRC_IMG, g_ImageSrc);

	//��������ʶ����ɫ
	vector<bool> vctRes;
	bool bSuccess = true;
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������ʶ��һ��");
		TaskPara TaskTmp = g_vecTask.at(i);
		ResultPara Result;
		int ret = JudgeColor(g_ImageSrc, TaskTmp, Result);
		bool bret = false;
		if (ret == 1)
		{
			pRes[i] = 1;
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:OK");
			EasyLog::Inst()->Log("RunDirect:OK");
			bret = true;
		}
		else if (ret == 0)
		{
			pRes[i] = 0;
			EasyLog::Inst()->Log("RunDirect:NG");
			bret = false;
		}
		else
		{
			cv::destroyAllWindows();
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��ɫʶ��������");
			EasyLog::Inst()->Log("RunDirect:ret ��ɫʶ��������");
			UinitDll();
			return ERR_COLOR;
		}

		bSuccess &= bret;
		if (g_bDebug)
			cv::waitKey(1000);
		vctRes.push_back(bret);
		g_vecResult.push_back(Result);
	}

	//����ʧ�ܽ��ͼ��JPG
	if (!bSuccess)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:����ʶ��ʧ��ͼƬ");
		//ʱ������
		SYSTEMTIME curT;
		GetLocalTime(&curT);
		CString strPath;
		strPath.Format("%d-%d-%d_%d-%d-%d.jpg", curT.wYear, curT.wMonth, curT.wDay, curT.wHour, curT.wMinute, curT.wSecond);
		
		std::string str;
		char p[1024];
		memset(p, '\0', 1024);
		sprintf_s(p, "Failed\\%d-%d-%d_%d-%d-%d.jpg", curT.wYear, curT.wMonth, curT.wDay, curT.wHour, curT.wMinute, curT.wSecond);
		str = g_strRootPath +p;
		cv::imwrite(str, g_ImageSrc);
	}
	//��ʾ���,ͼ�񣬱����ı�
	//���û����ı�����ز���
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������result.txt");
	std::string strResult = g_strRootPath + "result.txt";
	std::string strResultArea = g_strRootPath + "result_area.txt";
	FILE *pFile = fopen(strResult.c_str(), "w");
	FILE *pFile2 = fopen(strResultArea.c_str(), "w");
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		std::string text = "UnKnown";
		cv::Scalar color = cv::Scalar(255, 255, 0);
		if (i < g_vecResult.size())
		{
			if (g_vecResult[i].bOk)
			{
				text = "OK";
				color = cv::Scalar(0, 255, 0);
				fprintf_s(pFile, "OK\n");
				fprintf_s(pFile2, "OK,%.2lf\n", g_vecResult[i].Area);
			}
			else
			{
				text = "NG";
				color = cv::Scalar(0, 0, 255);
				fprintf_s(pFile, "NG\n");
				fprintf_s(pFile2, "NG,%.2lf\n", g_vecResult[i].Area);
			}
		}
		else
		{
			fprintf_s(pFile, "Unknown\n");
			fprintf_s(pFile2, "Unknown, 0\n");
		}

		int font_face = cv::FONT_HERSHEY_COMPLEX;
		double font_scale = 4;
		int thickness = 4;
		int baseline;
		cv::Point origin;
		origin.x = (g_vecTask[i].Left + g_vecTask[i].Right) / 2;
		origin.y = (g_vecTask[i].Top + g_vecTask[i].Bottom) / 2;
		cv::putText(g_ImageSrc, text, origin, font_face, font_scale, color, thickness, 8, 0);
	}
	fclose(pFile);
	fclose(pFile2);
	cv::imshow(WIN_SRC_IMG, g_ImageSrc);
	cv::waitKey(1000);

	//�رմ���
	cv::destroyAllWindows();
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:Success");

	//����ʼ��
	ret1 = UinitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("RunDirect:ret ERR");
		return ret1;
	}


	EasyLog::Inst()->Log("RunDirect:ret ERR_SUCCESS");
	return ERR_SUCCESS;
}

int RunDirect3(char * rootpath, char * pPath)
{
	int retN = 0;
	EasyLog::Inst()->Log("RunDirect:start*");

	g_strRootPath = rootpath;
	//��ʼ��
	int ret1 = InitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("RunDirect:ret ERR");
		return ret1;
	}


	//��������
	g_vecTask.clear();
	g_vecResult.clear();
	if (!LoadTaskParas(pPath))
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�");
		UinitDll();
		return ERR_TASK;
	}
	if (0 == g_vecTask.size())
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�:�������Ϊ��");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�:�������Ϊ��");
		UinitDll();
		return ERR_TASK;
	}
	//����
	//��ȡͼ��
	tSdkFrame Image;
	CameraSdkStatus st = m_Cam.GetImage(0, Image);
	if (st != CAMERA_STATUS_SUCCESS)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:������մ���");
		EasyLog::Inst()->Log("RunDirect:ret ������մ���");
		UinitDll();
		return ERR_CAM;
	}
	cv::Mat tmp2(
		cvSize(Image.head.iWidth, Image.head.iHeight),
		Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
		Image.pBuffer
	);
	//���·�ת
	cv::flip(tmp2, tmp2, 0);
	g_ImageSrc = tmp2;
	//g_ImageSrc = imread("test2.jpg");
	std::string str;
	str = g_strRootPath + "src.jpg";
	cv::imwrite(str, g_ImageSrc);

	//��������
	cv::namedWindow(WIN_SRC_IMG, 0);
	cv::resizeWindow(WIN_SRC_IMG, 640, 480);
	cv::moveWindow(WIN_SRC_IMG, 0, 0);

	cv::namedWindow(WIN_ROI_IMG, 0);
	cv::resizeWindow(WIN_ROI_IMG, 320, 240);
	cv::moveWindow(WIN_ROI_IMG, 640+10, 0);

	cv::namedWindow(WIN_HIS_IMG, 0);
	cv::resizeWindow(WIN_HIS_IMG, 320, 240);
	cv::moveWindow(WIN_HIS_IMG, 640 + 320+20, 0);

	cv::namedWindow(WIN_BIN_IMG, 0);
	cv::resizeWindow(WIN_BIN_IMG, 320, 240);
	cv::moveWindow(WIN_BIN_IMG, 640 + 10, 240+40);

	cv::imshow(WIN_SRC_IMG, g_ImageSrc);

	//��������ʶ����ɫ
	vector<bool> vctRes;
	bool bSuccess = true;
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������ʶ��һ��");
		TaskPara TaskTmp = g_vecTask.at(i);
		ResultPara Result;
		int ret = JudgeColor(g_ImageSrc, TaskTmp, Result);
		bool bret = false;
		if (ret == 1)
		{
			//pRes[i] = 1;
			retN |= (1 << i);
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:OK");
			EasyLog::Inst()->Log("RunDirect:OK");
			bret = true;
		}
		else if (ret == 0)
		{
			retN &= (~(1 << i));
			EasyLog::Inst()->Log("RunDirect:NG");
			bret = false;
		}
		else
		{
			cv::destroyAllWindows();
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��ɫʶ��������");
			EasyLog::Inst()->Log("RunDirect:ret ��ɫʶ��������");
			UinitDll();
			return ERR_COLOR;
		}

		bSuccess &= bret;
		if (g_bDebug)
			cv::waitKey(1000);
		vctRes.push_back(bret);
		g_vecResult.push_back(Result);
	}

	//����ʧ�ܽ��ͼ��JPG
	if (!bSuccess)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:����ʶ��ʧ��ͼƬ");
		//ʱ������
		SYSTEMTIME curT;
		GetLocalTime(&curT);
		CString strPath;
		strPath.Format("%d-%d-%d_%d-%d-%d.jpg", curT.wYear, curT.wMonth, curT.wDay, curT.wHour, curT.wMinute, curT.wSecond);

		std::string str;
		char p[1024];
		memset(p, '\0', 1024);
		sprintf_s(p, "Failed\\%d-%d-%d_%d-%d-%d.jpg", curT.wYear, curT.wMonth, curT.wDay, curT.wHour, curT.wMinute, curT.wSecond);
		str = g_strRootPath + p;
		cv::imwrite(str, g_ImageSrc);
	}
	//��ʾ���,ͼ�񣬱����ı�
	//���û����ı�����ز���
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������result.txt");
	std::string strResult = g_strRootPath + "result.txt";
	std::string strResultArea = g_strRootPath + "result_area.txt";
	FILE *pFile = fopen(strResult.c_str(), "w");
	FILE *pFile2 = fopen(strResultArea.c_str(), "w");
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		std::string text = "UnKnown";
		cv::Scalar color = cv::Scalar(255, 255, 0);
		if (i < g_vecResult.size())
		{
			if (g_vecResult[i].bOk)
			{
				text = "OK";
				color = cv::Scalar(0, 255, 0);
				fprintf_s(pFile, "OK\n");
				fprintf_s(pFile2, "OK,%.2lf\n", g_vecResult[i].Area);
			}
			else
			{
				text = "NG";
				color = cv::Scalar(0, 0, 255);
				fprintf_s(pFile, "NG\n");
				fprintf_s(pFile2, "NG,%.2lf\n", g_vecResult[i].Area);
			}
		}
		else
		{
			fprintf_s(pFile, "Unknown\n");
			fprintf_s(pFile2, "Unknown, 0\n");
		}

		int font_face = cv::FONT_HERSHEY_COMPLEX;
		double font_scale = 4;
		int thickness = 4;
		int baseline;
		cv::Point origin;
		origin.x = (g_vecTask[i].Left + g_vecTask[i].Right) / 2;
		origin.y = (g_vecTask[i].Top + g_vecTask[i].Bottom) / 2;
		cv::putText(g_ImageSrc, text, origin, font_face, font_scale, color, thickness, 8, 0);
		cv::Rect rct = cv::Rect(g_vecTask[i].Left,
			g_vecTask[i].Top,
			(g_vecTask[i].Right - g_vecTask[i].Left),
			(g_vecTask[i].Bottom - g_vecTask[i].Top));
		cv::rectangle(g_ImageSrc, rct, color, thickness);
	}
	fclose(pFile);
	fclose(pFile2);
	cv::imshow(WIN_SRC_IMG, g_ImageSrc);
	cv::waitKey(1000);

	//�رմ���
	cv::destroyAllWindows();
	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:Success");

	//����ʼ��
	ret1 = UinitDll();
	if (ret1 != ERR_SUCCESS)
	{
		EasyLog::Inst()->Log("RunDirect:ret ERR");
		return ret1;
	}


	EasyLog::Inst()->Log("RunDirect:ret ERR_SUCCESS");
	char p[1024];
	sprintf_s(p, "%d", retN);
	EasyLog::Inst()->Log(p);
	return retN;
}

int DetectNum()
{
	int nMethod = TM_CCORR_NORMED;
	Mat srcImg = imread("test.jpg", 1);
	Mat tmpImg = imread("tmp.jpg",1);
	namedWindow("TEST", CV_WINDOW_NORMAL);
	namedWindow("tmp", CV_WINDOW_NORMAL);

	//��ʼ�����
	int rows = srcImg.rows - tmpImg.rows + 1;
	int cols = srcImg.cols - tmpImg.cols + 1;
	Mat rstImg;
	rstImg.create(rows, cols, CV_32FC1);

	//����ƥ�䣬��׼��
	matchTemplate(srcImg, tmpImg, rstImg, nMethod);
	normalize(rstImg, rstImg, 0, 1, NORM_MINMAX, -1, Mat());

	//ͨ������minMaxLoc ��λ��ƥ���λ��
	double minValue; double maxValue;
	Point minLocation, maxLocation, matchLocation;
	minMaxLoc(rstImg, &minValue, &maxValue, &minLocation, &maxLocation, Mat());

	//
	if (nMethod== TM_SQDIFF || nMethod == TM_SQDIFF_NORMED)
	{
		matchLocation = minLocation;
	}
	else 
	{
		matchLocation = maxLocation;
	}

	//���ƾ���
	rectangle(srcImg, matchLocation,
		Point(matchLocation.x + tmpImg.cols, matchLocation.y + tmpImg.rows),
		Scalar(0, 0, 255), 2, 8, 0);

	rectangle(rstImg, matchLocation,
		Point(matchLocation.x + tmpImg.cols, matchLocation.y + tmpImg.rows),
		Scalar(0, 0, 255), 2, 8, 0);

	imshow("TEST", srcImg);
	imshow("tmp", rstImg);
	waitKey();
	return 0;
}

int ShowHist(/*char* rootpath, char* pPath*/)
{
	//g_strRootPath = rootpath;
	//��������
	bool ret = LoadColorParas("Para\\Setting.ini");
	if (ret)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "InitDll:Ret:Sucess");
		EasyLog::Inst()->Log("InitDll:ret ERR_SUCCESS");
		//return ERR_SUCCESS;
	}
	else
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "InitDll:Ret:������ɫ��׼�������ɹ�");
		EasyLog::Inst()->Log("InitDll:ret ERR_OTHER");
		return ERR_OTHER;
	}

	g_vecTask.clear();
	g_vecResult.clear();
	if (!LoadTaskParas("Task\\Task.ini"))
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�");
		UinitDll();
		return ERR_TASK;
	}
	if (0 == g_vecTask.size())
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��������������ɹ�:�������Ϊ��");
		EasyLog::Inst()->Log("RunDirect:ret ��������������ɹ�:�������Ϊ��");
		UinitDll();
		return ERR_TASK;
	}
	//����
	//��ȡͼ��
	//tSdkFrame Image;
	//CameraSdkStatus st = m_Cam.GetImage(0, Image);
	//if (st != CAMERA_STATUS_SUCCESS)
	//{
	//	//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:������մ���");
	//	EasyLog::Inst()->Log("RunDirect:ret ������մ���");
	//	UinitDll();
	//	return ERR_CAM;
	//}
	//cv::Mat tmp2(
	//	cvSize(Image.head.iWidth, Image.head.iHeight),
	//	Image.head.uiMediaType == CAMERA_MEDIA_TYPE_MONO8 ? CV_8UC1 : CV_8UC3,
	//	Image.pBuffer
	//);
	////���·�ת
	//cv::flip(tmp2, tmp2, 0);

	//g_ImageSrc = imread("test2.jpg");
	std::string str;
	str = /*g_strRootPath + */"src.jpg";
	g_ImageSrc = cv::imread(str);

	//��������
	cv::namedWindow(WIN_SRC_IMG, 0);
	cv::resizeWindow(WIN_SRC_IMG, 640, 480);
	cv::moveWindow(WIN_SRC_IMG, 0, 0);

	cv::namedWindow(WIN_ROI_IMG, 0);
	cv::resizeWindow(WIN_ROI_IMG, 320, 240);
	cv::moveWindow(WIN_ROI_IMG, 640 + 10, 0);

	cv::namedWindow(WIN_HIS_IMG, 0);
	cv::resizeWindow(WIN_HIS_IMG, 320, 240);
	cv::moveWindow(WIN_HIS_IMG, 640 + 320 + 20, 0);

	cv::namedWindow(WIN_BIN_IMG, 0);
	cv::resizeWindow(WIN_BIN_IMG, 320, 240);
	cv::moveWindow(WIN_BIN_IMG, 640 + 10, 240 + 40);

	cv::imshow(WIN_SRC_IMG, g_ImageSrc);

	//��������ʶ����ɫ
	vector<bool> vctRes;
	bool bSuccess = true;
	for (int i = 0; i < g_vecTask.size(); i++)
	{
		//LOG4CPLUS_INFO(m_myLoger->logger, "Run:��������ʶ��һ��");
		TaskPara TaskTmp = g_vecTask.at(i);
		ResultPara Result;
		int ret = JudgeColor(g_ImageSrc, TaskTmp, Result);
		bool bret = false;
		if (ret == 1)
		{
			//pRes[i] = 1;
			//retN |= (1 << i);
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:OK");
			EasyLog::Inst()->Log("RunDirect:OK");
			bret = true;
		}
		else if (ret == 0)
		{
			//retN &= (~(1 << i));
			EasyLog::Inst()->Log("RunDirect:NG");
			bret = false;
		}
		else
		{
			cv::destroyAllWindows();
			//LOG4CPLUS_INFO(m_myLoger->logger, "Run:Ret:��ɫʶ��������");
			EasyLog::Inst()->Log("RunDirect:ret ��ɫʶ��������");
			UinitDll();
			return ERR_COLOR;
		}

		bSuccess &= bret;
		if (g_bDebug)
			cv::waitKey(1000);
		vctRes.push_back(bret);
		g_vecResult.push_back(Result);
	}
	return 0;
}
