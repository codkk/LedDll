// LedDll.h : LedDll DLL ����ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


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
