#if !defined(AFX_PROPRULE_H__DBE797F0_21CE_46A5_9711_A302111DB921__INCLUDED_)
#define AFX_PROPRULE_H__DBE797F0_21CE_46A5_9711_A302111DB921__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropRule.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropRule dialog

// 规则编辑属性页的类
class CPropRule : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropRule)

// Construction
public:
	CPropRule();
	~CPropRule();

// Dialog Data
	//{{AFX_DATA(CPropRule)
	enum { IDD = IDD_PROPPAGE_RULE };
	CButton	m_applyNow;
	CButton	m_addMon;
	CButton	m_decMon;
	CListCtrl	m_ruleList;
	//}}AFX_DATA


// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropRule)
	public:
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	BOOL WriteToLog(CString strMsg, LONG lErrMsg);
	BOOL WriteToLog(CString strMsg);
	BOOL ReBoot();
	BOOL CheckResource();
	BOOL CheckSvrStatus();
	BOOL SetCtrlStatus();
	BOOL RestartSevices();
	// Generated message map functions
	//{{AFX_MSG(CPropRule)
	afx_msg void OnButtonAddmon();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonDecmon();
	afx_msg void OnApplyNow();
	afx_msg void OnCheckResource();
	afx_msg void OnCheckSendmsg();
	afx_msg void OnClickRulelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRclickRulelist(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnChangeEditPercent();
	afx_msg void OnChangeEditMsgto();
	afx_msg void OnCheckStartmon();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	BOOL bIsMon;
	UINT IDT_MONTIMER;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPRULE_H__DBE797F0_21CE_46A5_9711_A302111DB921__INCLUDED_)
