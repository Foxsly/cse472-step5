// LibGrafx.h : main header file for the LibGrafx DLL
//
// This work is Copyright (C) 2002-2012 Michigan State University
// This work is licensed under Microsoft Public License (Ms-PL)
//
// Please include author attribution when using this code.
//
// Author:  Charles B. Owen
//

#pragma once
//! \cond ignore

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols


// CLibGrafxApp
// See LibGrafx.cpp for the implementation of this class
//

class CLibGrafxApp : public CWinApp
{
public:
	CLibGrafxApp();

// Overrides
public:
	virtual BOOL InitInstance();

	DECLARE_MESSAGE_MAP()
};

//! \endcond

