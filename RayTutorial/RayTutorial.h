
// RayTutorial.h : main header file for the RayTutorial application
//
#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"       // main symbols


// CRayTutorialApp:
// See RayTutorial.cpp for the implementation of this class
//

class CRayTutorialApp : public CWinAppEx
{
public:
	CRayTutorialApp();


// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

public:
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CRayTutorialApp theApp;
