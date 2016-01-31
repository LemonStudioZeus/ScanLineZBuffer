// ScanLineZBuffer.h : main header file for the ScanLineZBuffer application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CScanLineZBufferApp:
// See ScanLineZBuffer.cpp for the implementation of this class
//

class CScanLineZBufferApp : public CWinApp
{
public:
	CScanLineZBufferApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CScanLineZBufferApp theApp;