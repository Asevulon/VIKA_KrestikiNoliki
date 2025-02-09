
// KrestikiNoliki.h: главный файл заголовка для приложения PROJECT_NAME
//

#pragma once

#ifndef __AFXWIN_H__
	#error "включить pch.h до включения этого файла в PCH"
#endif

#include "resource.h"		// основные символы


// CKrestikiNolikiApp:
// Сведения о реализации этого класса: KrestikiNoliki.cpp
//

class CKrestikiNolikiApp : public CWinApp
{
public:
	CKrestikiNolikiApp();

// Переопределение
public:
	virtual BOOL InitInstance();

// Реализация

	DECLARE_MESSAGE_MAP()
};

extern CKrestikiNolikiApp theApp;
