#pragma once
//dll�ӿ�

//������
#define ERR_SUCCESS 0      //����
#define ERR_CAM -1			//�������
#define ERR_COLOR -2			//��ɫʶ���㷨����
#define ERR_TASK -3			//��������ʧ��
#define ERR_OTHER -4			//����

//������ ִ�й���
//����pPath: �����ļ�·��
//����pRes:  �����־λ�� ����20�� ��һλΪfalse��ʾ�������ĳһ��ng��
//����ֵ�� ������
extern "C" _declspec(dllexport) int Run(char* pPath, char* pRes);

//������ ��ʼ��dll��������������ر�׼������
//����ֵ�� ������
extern "C" _declspec(dllexport) int InitDll();

//������ ����ʼ��dll���Ͽ���������ر�׼������
//����ֵ�� ������
extern "C" _declspec(dllexport) int UinitDll();

//������ ��ȡһ��ͼ��ǰ���������ʼ���ɹ���
//����ֵ�� ������
extern "C" _declspec(dllexport) int GrabOneImage();

//������ ֱ�ӻ�ȡһ��ͼ�񣬣�������InitDll, GrabOneImage, UinitDll��
//����ֵ�� ������
extern "C" _declspec(dllexport) int GrabOneImageDirect();

//������ ֱ�ӻ�ȡһ��ͼ�񣬣�������InitDll, GrabOneImage, UinitDll��
//����:  dll���ڸ�Ŀ¼����б�ܡ�\����β
//����ֵ�� ������
extern "C" _declspec(dllexport) int GrabOneImageDirect2(char* rootpath);

//������ ֱ�ӻ�ȡһ��ͼ�񣬣�������InitDll, RunDirect, UinitDll��
//����ֵ�� ������
extern "C" _declspec(dllexport) int RunDirect(char* pPath, char* pRes);

//������ ֱ�ӻ�ȡһ��ͼ�񣬣�������InitDll, RunDirect, UinitDll��
//����:  rootpath��dll���ڸ�Ŀ¼����б�ܡ�\����β
//		pPath:  �����ļ�*ini,����Ŀ¼����������·����
//      pRes:   �����һά����ָ�룬20��Ԫ�أ����ȳ�ʼ��Ϊȫ1���������PASS�������ӦԪ��λ����1�� ��֮��0.
//����ֵ�� ������
extern "C" _declspec(dllexport) int RunDirect2(char* rootpath, char* pPath, char* pRes);

//������ ֱ�ӻ�ȡһ��ͼ�񣬣�������InitDll, RunDirect, UinitDll��
//����:  rootpath��dll���ڸ�Ŀ¼����б�ܡ�\����β
//		pPath:  �����ļ�*ini,����Ŀ¼����������·����
//����ֵ�� �������Լ�����ֵ32λÿһλ��������0����false,1����trueOK
extern "C" _declspec(dllexport) int RunDirect3(char* rootpath, char* pPath);

//������ ģ��ƥ�䣬ʶ�������
//
extern "C" _declspec(dllexport) int DetectNum();

//���� ����ʾɫ����ֱ��ͼ
extern "C" _declspec(dllexport) int ShowHist();