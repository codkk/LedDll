#pragma once
//dll接口

//错误码
#define ERR_SUCCESS 0      //正常
#define ERR_CAM -1			//相机问题
#define ERR_COLOR -2			//颜色识别算法错误
#define ERR_TASK -3			//加载任务失败
#define ERR_OTHER -4			//其他

//函数： 执行功能
//参数pPath: 任务文件路径
//参数pRes:  结果标志位： 长度20， 哪一位为false表示该任务的某一项ng。
//返回值： 错误码
extern "C" _declspec(dllexport) int Run(char* pPath, char* pRes);

//函数： 初始化dll（连接相机，加载标准参数）
//返回值： 错误码
extern "C" _declspec(dllexport) int InitDll();

//函数： 反初始化dll（断开相机，加载标准参数）
//返回值： 错误码
extern "C" _declspec(dllexport) int UinitDll();

//函数： 获取一张图像，前提是相机初始化成功后
//返回值： 错误码
extern "C" _declspec(dllexport) int GrabOneImage();

//函数： 直接获取一张图像，（包含了InitDll, GrabOneImage, UinitDll）
//返回值： 错误码
extern "C" _declspec(dllexport) int GrabOneImageDirect();

//函数： 直接获取一张图像，（包含了InitDll, GrabOneImage, UinitDll）
//参数:  dll所在根目录，以斜杠“\”结尾
//返回值： 错误码
extern "C" _declspec(dllexport) int GrabOneImageDirect2(char* rootpath);

//函数： 直接获取一张图像，（包含了InitDll, RunDirect, UinitDll）
//返回值： 错误码
extern "C" _declspec(dllexport) int RunDirect(char* pPath, char* pRes);

//函数： 直接获取一张图像，（包含了InitDll, RunDirect, UinitDll）
//参数:  rootpath：dll所在根目录，以斜杠“\”结尾
//		pPath:  任务文件*ini,所在目录，完整绝对路径。
//      pRes:   传入的一维数组指针，20个元素，请先初始化为全1，如果测试PASS则会在响应元素位置置1， 反之置0.
//返回值： 错误码
extern "C" _declspec(dllexport) int RunDirect2(char* rootpath, char* pPath, char* pRes);

//函数： 直接获取一张图像，（包含了InitDll, RunDirect, UinitDll）
//参数:  rootpath：dll所在根目录，以斜杠“\”结尾
//		pPath:  任务文件*ini,所在目录，完整绝对路径。
//返回值： 错误码以及返回值32位每一位代表结果，0代表false,1代表trueOK
extern "C" _declspec(dllexport) int RunDirect3(char* rootpath, char* pPath);

//函数： 模板匹配，识别数码管
//
extern "C" _declspec(dllexport) int DetectNum();

//函数 ：显示色调的直方图
extern "C" _declspec(dllexport) int ShowHist();