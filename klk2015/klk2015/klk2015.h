
// klk2015.h : main header file for the klk2015 application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'pch.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// Cklk2015App:
// See klk2015.cpp for the implementation of this class
//

class Cklk2015App : public CWinApp
{
public:
	Cklk2015App() noexcept;


// Overrides
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern Cklk2015App theApp;
