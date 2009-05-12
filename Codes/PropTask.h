#if !defined(AFX_PROPTASK_H__F008C511_3937_4A9A_A3EB_DD703D9E547A__INCLUDED_)
#define AFX_PROPTASK_H__F008C511_3937_4A9A_A3EB_DD703D9E547A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropTask.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropTask dialog

class CPropTask : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropTask)

// Construction
public:
	CPropTask();
	~CPropTask();

// Dialog Data
	//{{AFX_DATA(CPropTask)
	enum { IDD = IDD_PROPPAGE_TASK };
		// NOTE - ClassWizard will add data members here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropTask)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	// Generated message map functions
	//{{AFX_MSG(CPropTask)
		// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPTASK_H__F008C511_3937_4A9A_A3EB_DD703D9E547A__INCLUDED_)
