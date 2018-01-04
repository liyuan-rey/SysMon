// SysMonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "SysMon.h"
#include "SysMonDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define VK_ESCAPE 0x1B

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
  public:
	CAboutDlg();

	// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum
	{
		IDD = IDD_ABOUTBOX
	};
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
  protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
													 //}}AFX_VIRTUAL

	// Implementation
  protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
//{{AFX_MSG_MAP(CAboutDlg)
// No message handlers
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysMonDlg dialog

CSysMonDlg::CSysMonDlg(CWnd *pParent /*=NULL*/)
	: CDialog(CSysMonDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSysMonDlg)
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CSysMonDlg::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSysMonDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSysMonDlg, CDialog)
//{{AFX_MSG_MAP(CSysMonDlg)
ON_WM_SYSCOMMAND()
ON_WM_PAINT()
ON_WM_QUERYDRAGICON()
ON_COMMAND(IDR_SYS_ABOUT, OnSysAbout)
ON_COMMAND(IDM_SYS_HELP, OnSysHelp)
ON_COMMAND(IDM_PROC_REFRESH, OnProcRefresh)
ON_COMMAND(IDM_PROC_TERMINATE, OnProcTerminate)
ON_COMMAND(IDM_PROC_SENDMSG, OnProcSendmsg)
ON_WM_CHAR()
ON_COMMAND(IDM_SYS_EXIT, OnSysExit)
ON_WM_CLOSE()
ON_WM_SIZE()
ON_COMMAND(IDM_SHELLICON_SHOWWINDOW, OnShelliconShowwindow)
ON_COMMAND(IDM_SHELLICON_EXIT, OnShelliconExit)
ON_COMMAND(IDM_SVR_LOAD, OnSvrLoad)
ON_COMMAND(IDM_SVR_PAUSE, OnSvrPause)
ON_COMMAND(IDM_SVR_REFRESH, OnSvrRefresh)
ON_COMMAND(IDM_SVR_RESUME, OnSvrResume)
ON_COMMAND(IDM_SVR_STOP, OnSvrStop)
ON_UPDATE_COMMAND_UI(IDM_SVR_LOAD, OnUpdateSvrLoad)
//}}AFX_MSG_MAP
ON_MESSAGE(WM_SYSMONNOTIFY, OnSysMonNotify)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSysMonDlg message handlers

BOOL CSysMonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu *pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);  // Set big icon
	SetIcon(m_hIcon, FALSE); // Set small icon

	// TODO: Add extra initialization here
	nIsHide = SW_SHOW;
	// 初始化属性单
	m_sheet.AddPage(&m_propSysinfo);
	m_sheet.AddPage(&m_propProcess);
	m_sheet.AddPage(&m_propService);
	m_sheet.AddPage(&m_propRule);
	m_sheet.AddPage(&m_propTask);
	m_sheet.AddPage(&m_propSetting);

	m_sheet.Create(this, WS_CHILD | WS_VISIBLE, 0);
	m_sheet.ModifyStyleEx(0, WS_EX_CONTROLPARENT);
	m_sheet.ModifyStyle(0, WS_TABSTOP);

	return TRUE; // return TRUE  unless you set the focus to a control
}

void CSysMonDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CSysMonDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM)dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CSysMonDlg::OnQueryDragIcon()
{
	return (HCURSOR)m_hIcon;
}

void CSysMonDlg::OnSysAbout()
{
	// TODO: Add your command handler code here
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}

void CSysMonDlg::OnSysHelp()
{
	// TODO: Add your command handler code here
	AfxMessageBox("Sorry, Help is not available for now!");
}

void CSysMonDlg::OnProcRefresh()
{
	// TODO: Add your command handler code here
	m_propProcess.OnProcRefresh();
}

void CSysMonDlg::OnProcTerminate()
{
	// TODO: Add your command handler code here
	m_propProcess.OnProcTerminate();
}

void CSysMonDlg::OnProcSendmsg()
{
	// TODO: Add your command handler code here
	m_propProcess.OnProcSendmsg();
}

void CSysMonDlg::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: Add your message handler code here and/or call default
	AfxMessageBox(nChar);
	// 添加任务栏图标
	if (nChar == VK_ESCAPE)
	{
		NOTIFYICONDATA nid;

		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = this->m_hWnd;
		nid.uID = IDR_MAINFRAME;
		nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		nid.uCallbackMessage = WM_SYSMONNOTIFY;
		nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		strcpy(nid.szTip, _T("系统监控器"));

		Shell_NotifyIcon(NIM_ADD, &nid);
	}

	// 隐藏窗体
	ShowWindow(SW_HIDE);

	CDialog::OnChar(nChar, nRepCnt, nFlags);
}

void CSysMonDlg::OnSysMonNotify(WPARAM wParam, LPARAM lParam)
{
	UINT uID;		//发出该消息的图标的ID
	UINT uMouseMsg; //鼠标动作
	POINT point;

	uID = (UINT)wParam;
	uMouseMsg = (UINT)lParam;
	if (uMouseMsg == WM_RBUTTONUP) // 如果是双击左键
	{
		if (uID == IDR_MAINFRAME) // 如果是我们的图标
		{
			GetCursorPos(&point); //取得鼠标位置

			// 取菜单资源
			CMenu mnuTop;
			mnuTop.LoadMenu(IDR_MENU_SHELLICON);

			// 取子菜单
			CMenu *pPopup = mnuTop.GetSubMenu(0);
			ASSERT_VALID(pPopup);

			// 设置默认菜单
			SetMenuDefaultItem(pPopup->m_hMenu, 0, TRUE);

			// 显示菜单
			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
								   point.x, point.y,
								   AfxGetMainWnd(), NULL);

			// 弹出式菜单的命令会被 MFC 的消息路由机制自动处理
		}
	}

	if (uMouseMsg == WM_LBUTTONDBLCLK) //如果是双击左键
	{
		if (uID == IDR_MAINFRAME)	//如果是我们的图标
			OnShelliconShowwindow(); // 显示窗体
	}

	return;
}

void CSysMonDlg::OnSysExit()
{
	// TODO: Add your command handler code here
	// 退出程序
	SendMessage(WM_CLOSE);
}

void CSysMonDlg::OnClose()
{
	// TODO: Add your message handler code here and/or call default
	if (IDOK == AfxMessageBox(_T("确定要退出系统监控程序吗?"),
							  MB_OKCANCEL))
	{
		CDialog::OnClose(); // CDialog::OnOK();
	}
}

void CSysMonDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here
	// 添加任务栏图标
	if (nType == SIZE_MINIMIZED)
	{
		NOTIFYICONDATA nid;

		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = this->m_hWnd;
		nid.uID = IDR_MAINFRAME;
		nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;
		nid.uCallbackMessage = WM_SYSMONNOTIFY;
		nid.hIcon = LoadIcon(AfxGetInstanceHandle(), MAKEINTRESOURCE(IDR_MAINFRAME));
		strcpy(nid.szTip, _T("系统监控器"));

		Shell_NotifyIcon(NIM_ADD, &nid);
	}

	// 隐藏窗体
	if (nIsHide == SW_SHOW)
	{
		ShowWindow(SW_SHOWMINIMIZED);
		ShowWindow(SW_HIDE);
		UpdateWindow();
		nIsHide = SW_HIDE;
	}
}

void CSysMonDlg::OnShelliconShowwindow()
{
	// TODO: Add your command handler code here
	// 显示窗体
	if (nIsHide == SW_HIDE)
	{
		ShowWindow(SW_RESTORE);
		SetForegroundWindow();
		nIsHide = SW_SHOW;
	}

	// 删除任务栏图标
	NOTIFYICONDATA nid;

	nid.cbSize = sizeof(NOTIFYICONDATA);
	nid.hWnd = this->m_hWnd;
	nid.uID = IDR_MAINFRAME; // 保证删除的是我们的图标
	Shell_NotifyIcon(NIM_DELETE, &nid);
}

void CSysMonDlg::OnShelliconExit()
{
	// TODO: Add your command handler code here
	OnShelliconShowwindow();
	SendMessage(WM_CLOSE);
}

void CSysMonDlg::OnSvrLoad()
{
	// TODO: Add your command handler code here
	m_propService.OnSvrLoad();
}

void CSysMonDlg::OnSvrPause()
{
	// TODO: Add your command handler code here
	m_propService.OnSvrPause();
}

void CSysMonDlg::OnSvrResume()
{
	// TODO: Add your command handler code here
	m_propService.OnSvrResume();
}

void CSysMonDlg::OnSvrStop()
{
	// TODO: Add your command handler code here
	m_propService.OnSvrStop();
}

void CSysMonDlg::OnSvrRefresh()
{
	// TODO: Add your command handler code here
	m_propService.OnSvrRefresh();
}

void CSysMonDlg::OnUpdateSvrLoad(CCmdUI *pCmdUI)
{
	// TODO: Add your command update UI handler code here
	AfxMessageBox("!");
}
