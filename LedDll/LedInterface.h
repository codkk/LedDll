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
//����ֵ�� ��ʼ�����true or false
extern "C" _declspec(dllexport) bool InitDll();

//������ ����ʼ��dll���Ͽ���������ر�׼������
//����ֵ�� ����ʼ�����true or false
extern "C" _declspec(dllexport) bool UinitDll();

extern "C" _declspec(dllexport) int GrabOneImage();