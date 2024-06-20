// Game2048.h : main header file for the Game2048 DLL
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CGame2048App
// See Game2048.cpp for the implementation of this class
//

class CGame2048App : public CWinApp
{
public:
	CGame2048App();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};
