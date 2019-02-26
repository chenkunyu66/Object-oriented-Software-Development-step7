
// Workshop.h : main header file for the Workshop application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CWorkshopApp:
/// See Workshop.cpp for the implementation of this class
class CWorkshopApp : public CWinApp
{
public:
	CWorkshopApp();


// Overrides
public:
	virtual BOOL InitInstance(); ///<initins
	virtual int ExitInstance();  ///<inits

// Implementation
public:
	afx_msg void OnAppAbout();  ///<OnAppAbout
	DECLARE_MESSAGE_MAP()


private:
    Gdiplus::GdiplusStartupInput gdiplusStartupInput; ///<see
    ULONG_PTR gdiplusToken; ///<wefe
};

extern CWorkshopApp theApp;
