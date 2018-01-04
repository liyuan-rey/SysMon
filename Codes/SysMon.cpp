// SysMon.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "SysMon.h"
#include "SysMonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CString GetLocalMachineName()
{
	char szWkstaName[MAX_COMPUTERNAME_LENGTH + 1];
	DWORD dwWSize = sizeof(szWkstaName);
	GetComputerName(szWkstaName, &dwWSize);
	CString strLocalMachineName = (CString)(LPCTSTR)szWkstaName;

	return strLocalMachineName;
}

void AutoSizeColumns(CListCtrl *m_List, int col = -1)
{
	m_List->SetRedraw(false); // 禁止重绘
	int mincol = col < 0 ? 0 : col;
	int maxcol = col < 0 ? (m_List->GetHeaderCtrl())->GetItemCount() - 1 : col;
	for (col = mincol; col <= maxcol; col++)
	{
		m_List->SetColumnWidth(col, LVSCW_AUTOSIZE);
		int wc1 = m_List->GetColumnWidth(col);
		m_List->SetColumnWidth(col, LVSCW_AUTOSIZE_USEHEADER);
		int wc2 = m_List->GetColumnWidth(col);
		int wc = MAX(MINCOLWIDTH, MAX(wc1, wc2)); // MAX 模板, 定义在 stdAfx.h 中
		m_List->SetColumnWidth(col, wc);
	}
	m_List->SetRedraw(true); // 允许重绘
}

/////////////////////////////////////////////////////////////////////////////
// CSysMonApp

BEGIN_MESSAGE_MAP(CSysMonApp, CWinApp)
//{{AFX_MSG_MAP(CSysMonApp)
// NOTE - the ClassWizard will add and remove mapping macros here.
//    DO NOT EDIT what you see in these blocks of generated code!
//}}AFX_MSG
ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysMonApp construction

CSysMonApp::CSysMonApp()
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSysMonApp object

CSysMonApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSysMonApp initialization

BOOL CSysMonApp::InitInstance()
{
	AfxEnableControlContainer();

	// Standard initialization
	// If you are not using these features and wish to reduce the size
	//  of your final executable, you should remove from the following
	//  the specific initialization routines you do not need.

#ifdef _AFXDLL
	Enable3dControls(); // Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic(); // Call this when linking to MFC statically
#endif

	// 防止应用程序两次加载
	CWnd *pWndPre;
	//	pWndPre = CWnd::FindWindow(_T("CSysMonDlg"), _T("SysMon"));
	pWndPre = CWnd::FindWindow(_T("#32770"), _T("SysMon")); // #32770 为类 CSysMonDlg 的注册类名
	if (pWndPre != NULL)
	{
		/*		CHAR className[256];
		GetClassName(pWndPre->m_hWnd, className, 255);
		AfxMessageBox(_T(className));*/
		AfxMessageBox(_T("你已经运行了一个实例!"));
		return FALSE;
	}

	CSysMonDlg dlg;
	m_pMainWnd = &dlg;
	int nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with OK
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: Place code here to handle when the dialog is
		//  dismissed with Cancel
	}

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}
