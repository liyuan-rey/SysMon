#if !defined(AFX_SELECTOR_H__7EEB699D_D43B_4181_9F52_0DCA99879C67__INCLUDED_)
#define AFX_SELECTOR_H__7EEB699D_D43B_4181_9F52_0DCA99879C67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Selector.h : header file
//

extern CString GetLocalMachineName();

typedef struct _LIST_ITEM {
	CString    strDisplayName;
	CString    strServiceName;
} LIST_ITEM, *PLIST_ITEM;

/////////////////////////////////////////////////////////////////////////////
// CSelector dialog

class CSelector : public CDialog
{
// Construction
public:
	BOOL Init(CListCtrl* lpList);
	CSelector(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CSelector)
	enum { IDD = IDD_DIALOG_SVRSELECT };
	CListCtrl	m_SelectorList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelector)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL FillList();

	// Generated message map functions
	//{{AFX_MSG(CSelector)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CListCtrl* m_pListForQuery;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTOR_H__7EEB699D_D43B_4181_9F52_0DCA99879C67__INCLUDED_)
