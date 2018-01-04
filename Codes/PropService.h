#if !defined(AFX_PROPSERVICE_H__C1485EDB_E475_493B_982F_1FF2B071B5AF__INCLUDED_)
#define AFX_PROPSERVICE_H__C1485EDB_E475_493B_982F_1FF2B071B5AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropService.h : header file
//

extern CString GetLocalMachineName();

/////////////////////////////////////////////////////////////////////////////
// CPropService dialog

// 服务列表属性页的类
class CPropService : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropService)

	// Construction
  public:
	void SetType(int nServices, int nDrivers);
	void SetState(int nActive, int nInactive, int nAll);
	CString m_strMachineName;
	CPropService();
	~CPropService();

	friend class CSysMonDlg;

	// Dialog Data
	//{{AFX_DATA(CPropService)
	enum
	{
		IDD = IDD_PROPPAGE_SERVICE
	};
	CListCtrl m_svrList;
	//}}AFX_DATA

	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropService)
  protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
													 //}}AFX_VIRTUAL

	// Implementation
  protected:
	DWORD GetSvrStatus(CString strServiceName);
	CString m_strCurSvrName;
	DWORD SvrControl(DWORD dwCtrlCode);
	DWORD m_dwStatus;
	DWORD m_dwType;
	BOOL WriteToLog(CString strMsg, LONG lErrMsg);
	BOOL GetSvrList();
	//	BOOL GetSvrList(PSVR_LIST pSvrList, DWORD dwNumTasks);
	// Generated message map functions
	//{{AFX_MSG(CPropService)
	virtual BOOL OnInitDialog();
	afx_msg void OnTypeDrivers();
	afx_msg void OnTypeServices();
	afx_msg void OnViewAll();
	afx_msg void OnViewStoped();
	afx_msg void OnViewRunning();
	afx_msg void OnCreateSvr();
	afx_msg void OnViewLog();
	afx_msg void OnContextMenu(CWnd *pWnd, CPoint point);
	afx_msg void OnSvrLoad();
	afx_msg void OnSvrStop();
	afx_msg void OnSvrResume();
	afx_msg void OnSvrPause();
	afx_msg void OnSvrRefresh();
	afx_msg void OnUpdateSvrLoad(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSvrPause(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSvrResume(CCmdUI *pCmdUI);
	afx_msg void OnUpdateSvrStop(CCmdUI *pCmdUI);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

  private:
	BOOL InsertInList(
		LPCTSTR lpSvrName, LPCTSTR lpDisplayName,
		DWORD dwServiceType, LPCTSTR lpStartType,
		LPCTSTR lpCurrentState, LPTSTR lpLoadOrderGroup,
		LPTSTR lpServiceStartName, LPTSTR lpDependencies,
		LPTSTR lpBinaryPathName /*, CString strDescription */);

	CString GetStatusString(DWORD dwServiceStatus);
	CString GetStartupString(DWORD dwStartupType);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSERVICE_H__C1485EDB_E475_493B_982F_1FF2B071B5AF__INCLUDED_)
