
// testLedDll.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CtestLedDllApp: 
// �йش����ʵ�֣������ testLedDll.cpp
//

class CtestLedDllApp : public CWinApp
{
public:
	CtestLedDllApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CtestLedDllApp theApp;