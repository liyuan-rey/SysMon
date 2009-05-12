// PropSysinfo.cpp : implementation file
//

#include "stdafx.h"
#include "sysmon.h"
#include "PropSysinfo.h"

#include "winbase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSysinfo property page

IMPLEMENT_DYNCREATE(CPropSysinfo, CPropertyPage)

CPropSysinfo::CPropSysinfo() : CPropertyPage(CPropSysinfo::IDD)
{
	//{{AFX_DATA_INIT(CPropSysinfo)
	//}}AFX_DATA_INIT
}

CPropSysinfo::~CPropSysinfo()
{
}

void CPropSysinfo::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropSysinfo)
	DDX_Control(pDX, IDC_MEMUSERLEFT, m_memUserLeft);
	DDX_Control(pDX, IDC_MEMUSER, m_memUser);
	DDX_Control(pDX, IDC_MEMPHYSICALLEFT, m_memPhysicalLeft);
	DDX_Control(pDX, IDC_MEMPHYSICAL, m_memPhysical);
	DDX_Control(pDX, IDC_MEMPERCENT, m_memPercent);
	DDX_Control(pDX, IDC_MEMPAGELEFT, m_memPageLeft);
	DDX_Control(pDX, IDC_MEMPAGE, m_memPage);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropSysinfo, CPropertyPage)
	//{{AFX_MSG_MAP(CPropSysinfo)
	ON_WM_TIMER()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSysinfo message handlers

BOOL CPropSysinfo::OnInitDialog() 
{
	CPropertyPage::OnInitDialog();
	
	// TODO: Add extra initialization here
	RefreshInfo();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropSysinfo::RefreshInfo()
{
	LPMEMORYSTATUS pMemStatus = new MEMORYSTATUS;

	pMemStatus->dwLength = sizeof(MEMORYSTATUS);
	::GlobalMemoryStatus(pMemStatus);

	CString strTemp;

	strTemp.Format("%ld%%", pMemStatus->dwMemoryLoad);
	m_memPercent.SetWindowText(_T(strTemp));
	strTemp.Format("%ld", pMemStatus->dwTotalPhys / 1024);
	m_memPhysical.SetWindowText(_T(strTemp));
	strTemp.Format("%ld", pMemStatus->dwAvailPhys / 1024);
	m_memPhysicalLeft.SetWindowText(_T(strTemp));
	strTemp.Format("%ld", pMemStatus->dwTotalPageFile / 1024);
	m_memPage.SetWindowText(_T(strTemp));
	strTemp.Format("%ld", pMemStatus->dwAvailPageFile / 1024);
	m_memPageLeft.SetWindowText(_T(strTemp));
	strTemp.Format("%ld", pMemStatus->dwTotalVirtual / 1024);
	m_memUser.SetWindowText(_T(strTemp));
	strTemp.Format("%ld", pMemStatus->dwAvailVirtual / 1024);
	m_memUserLeft.SetWindowText(_T(strTemp));

	delete pMemStatus;
	pMemStatus = NULL;
}

void CPropSysinfo::OnTimer(UINT nIDEvent) 
{
	// TODO: Add your message handler code here and/or call default
	RefreshInfo();

	CPropertyPage::OnTimer(nIDEvent);
}

BOOL CPropSysinfo::OnKillActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	if (m_MyTimer)
		KillTimer(m_MyTimer);

	return CPropertyPage::OnKillActive();
}

BOOL CPropSysinfo::OnSetActive() 
{
	// TODO: Add your specialized code here and/or call the base class
	m_MyTimer = SetTimer(8048, 1000, NULL);
	if (!m_MyTimer)
		AfxMessageBox(_T("Create Timer Fail!"));

	return CPropertyPage::OnSetActive();
}
