// MyPropertySheet.cpp : implementation file
//

#include "stdafx.h"
#include "sysmon.h"
#include "MyPropertySheet.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet

IMPLEMENT_DYNAMIC(CMyPropertySheet, CPropertySheet)

CMyPropertySheet(UINT nIDCaption, CWnd *pParentWnd, UINT iSelectPage)
	: CPropertySheet(nIDCaption, pParentWnd, iSelectPage)
{
}

CMyPropertySheet::CMyPropertySheet(LPCTSTR pszCaption, CWnd *pParentWnd, UINT iSelectPage)
	: CPropertySheet(pszCaption, pParentWnd, iSelectPage)
{
}

CMyPropertySheet::~CMyPropertySheet()
{
}

BEGIN_MESSAGE_MAP(CMyPropertySheet, CPropertySheet)
//{{AFX_MSG_MAP(CMyPropertySheet)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyPropertySheet message handlers

void CMyPropertySheet::PostNcDestroy()
{
	// TODO: Add your specialized code here and/or call the base class
	// 退出时删除属性页
	for (int i = 0; i < GetPageCount(); i++)
	{
		CPropertyPage *pPage = GetPage(i);
		ASSERT(pPage);
		if (pPage)
			delete pPage;
	}

	CPropertySheet::PostNcDestroy();
}
