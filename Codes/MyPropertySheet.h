#if !defined(AFX_MYPROPERTYSHEET_H__52AFA986_C14A_4C45_9324_45EA8EB13271__INCLUDED_)
#define AFX_MYPROPERTYSHEET_H__52AFA986_C14A_4C45_9324_45EA8EB13271__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyPropertySheet.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

// 增加了退出时删除无模式属性单包含的属性页
class CMyPropertySheet : public CPropertySheet
{
	DECLARE_DYNAMIC(CMyPropertySheet)

// Construction
public:
	CMyPropertySheet(UINT nIDCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);
	CMyPropertySheet(LPCTSTR pszCaption, CWnd* pParentWnd = NULL, UINT iSelectPage = 0);

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyPropertySheet)
	protected:
	virtual void PostNcDestroy();
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyPropertySheet();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyPropertySheet)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYPROPERTYSHEET_H__52AFA986_C14A_4C45_9324_45EA8EB13271__INCLUDED_)
