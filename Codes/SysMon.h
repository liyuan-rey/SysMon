// SysMon.h : main header file for the SYSMON application
//

#if !defined(AFX_SYSMON_H__2D466286_1C70_11D5_B897_0000B4921403__INCLUDED_)
#define AFX_SYSMON_H__2D466286_1C70_11D5_B897_0000B4921403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#define MINCOLWIDTH 10

/////////////////////////////////////////////////////////////////////////////
// CSysMonApp:
// See SysMon.cpp for the implementation of this class
//

class CSysMonApp : public CWinApp
{
public:
	CSysMonApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysMonApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CSysMonApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSMON_H__2D466286_1C70_11D5_B897_0000B4921403__INCLUDED_)
