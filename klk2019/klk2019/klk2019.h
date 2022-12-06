
// klk2019.h : main header file for the klk2019 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cklk2019App:
// See klk2019.cpp for the implementation of this class
//

class Cklk2019App : public CWinApp
{
public:
	Cklk2019App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cklk2019App theApp;
