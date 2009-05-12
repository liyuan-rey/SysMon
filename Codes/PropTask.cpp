// PropTask.cpp : implementation file
//

#include "stdafx.h"
#include "sysmon.h"
#include "PropTask.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropTask property page

IMPLEMENT_DYNCREATE(CPropTask, CPropertyPage)

CPropTask::CPropTask() : CPropertyPage(CPropTask::IDD)
{
	//{{AFX_DATA_INIT(CPropTask)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropTask::~CPropTask()
{
}

void CPropTask::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropTask)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropTask, CPropertyPage)
	//{{AFX_MSG_MAP(CPropTask)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropTask message handlers
