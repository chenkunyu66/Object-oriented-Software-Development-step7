/**
* \file WorkshopLib.h
*
* \author Charles B. Owen
*
* The library App class.
* \cond
*/

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CWorkshopLibApp
// See WorkshopLib.cpp for the implementation of this class
//

class CWorkshopLibApp : public CWinApp
{
public:
	CWorkshopLibApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

/// \endcond
