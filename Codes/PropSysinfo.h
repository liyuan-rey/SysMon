#if !defined(AFX_PROPSYSINFO_H__8C521C44_C3E2_4B23_8DC6_C2CC8C7AD496__INCLUDED_)
#define AFX_PROPSYSINFO_H__8C521C44_C3E2_4B23_8DC6_C2CC8C7AD496__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSysinfo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropSysinfo dialog

class CPropSysinfo : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropSysinfo)

// Construction
public:
	UINT m_MyTimer;
	CPropSysinfo();
	~CPropSysinfo();

// Dialog Data
	//{{AFX_DATA(CPropSysinfo)
	enum { IDD = IDD_PROPPAGE_SYSINFO };
	CStatic	m_memUserLeft;
	CStatic	m_memUser;
	CStatic	m_memPhysicalLeft;
	CStatic	m_memPhysical;
	CStatic	m_memPercent;
	CStatic	m_memPageLeft;
	CStatic	m_memPage;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropSysinfo)
	public:
	virtual BOOL OnKillActive();
	virtual BOOL OnSetActive();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	void RefreshInfo();
	// Generated message map functions
	//{{AFX_MSG(CPropSysinfo)
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSYSINFO_H__8C521C44_C3E2_4B23_8DC6_C2CC8C7AD496__INCLUDED_)
