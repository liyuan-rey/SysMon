// SysMonDlg.h : header file
//

#if !defined(AFX_SYSMONDLG_H__2D466288_1C70_11D5_B897_0000B4921403__INCLUDED_)
#define AFX_SYSMONDLG_H__2D466288_1C70_11D5_B897_0000B4921403__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "PropProcess.h"
#include "PropTask.h"
#include "PropRule.h"
#include "PropSetting.h"
#include "PropService.h"
#include "PropSysinfo.h"

#define WM_SYSMONNOTIFY         WM_USER + 8048

/////////////////////////////////////////////////////////////////////////////
// CSysMonDlg dialog

class CSysMonDlg : public CDialog
{
// Construction
public:
	CSysMonDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CSysMonDlg)
	enum { IDD = IDD_SYSMON_DIALOG };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSysMonDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	int nIsHide;
	CPropertySheet m_sheet;	// 属性单
	CPropSysinfo m_propSysinfo;	// 系统信息属性页
	CPropProcess m_propProcess;	// 进程管理属性页
	CPropService m_propService;	// 服务列表属性页
	CPropRule m_propRule;  // 规则设置属性页
	CPropTask m_propTask;  // 计划任务属性页
	CPropSetting m_propSetting;	// 综合设置属性页
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CSysMonDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSysAbout();
	afx_msg void OnSysHelp();
	afx_msg void OnProcRefresh();
	afx_msg void OnProcTerminate();
	afx_msg void OnProcSendmsg();
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSysExit();
	afx_msg void OnClose();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShelliconShowwindow();
	afx_msg void OnShelliconExit();
	afx_msg void OnSvrLoad();
	afx_msg void OnSvrPause();
	afx_msg void OnSvrRefresh();
	afx_msg void OnSvrResume();
	afx_msg void OnSvrStop();
	afx_msg void OnUpdateSvrLoad(CCmdUI* pCmdUI);
	//}}AFX_MSG
	afx_msg void OnSysMonNotify(WPARAM wParam,LPARAM lParam);
	DECLARE_MESSAGE_MAP()
private:
//	UINT IDT_MYTIMER;
//	DWORD SelectedProcID;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SYSMONDLG_H__2D466288_1C70_11D5_B897_0000B4921403__INCLUDED_)
