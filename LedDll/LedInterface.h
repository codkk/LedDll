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
//返回值： 初始化结果true or false
extern "C" _declspec(dllexport) bool InitDll();

//函数： 反初始化dll（断开相机，加载标准参数）
//返回值： 反初始化结果true or false
extern "C" _declspec(dllexport) bool UinitDll();

extern "C" _declspec(dllexport) int GrabOneImage();