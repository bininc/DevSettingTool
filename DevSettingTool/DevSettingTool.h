
// DevSettingTool.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CDevSettingToolApp: 
// �йش����ʵ�֣������ DevSettingTool.cpp
//

class CDevSettingToolApp : public CWinApp
{
public:
	CDevSettingToolApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CDevSettingToolApp theApp;