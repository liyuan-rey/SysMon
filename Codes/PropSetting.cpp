// PropSetting.cpp : implementation file
//

#include "stdafx.h"
#include "SysMon.h"
#include "PropSetting.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropSetting property page

IMPLEMENT_DYNCREATE(CPropSetting, CPropertyPage)

CPropSetting::CPropSetting() : CPropertyPage(CPropSetting::IDD)
{
	//{{AFX_DATA_INIT(CPropSetting)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CPropSetting::~CPropSetting()
{
}

void CPropSetting::DoDataExchange(CDataExchange* pDX)
{
	CPropertyPage::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropSetting)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropSetting, CPropertyPage)
	//{{AFX_MSG_MAP(CPropSetting)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropSetting message handlers
