// Selector.cpp : implementation file
//

#include "stdafx.h"
#include "sysmon.h"
#include "Selector.h"

#include "Windows.h"
#include "Winreg.h"
#include "Winsvc.h"
#include "Winerror.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void AutoSizeColumns(CListCtrl *m_List, int col = -1);

/////////////////////////////////////////////////////////////////////////////
// CSelector dialog

CSelector::CSelector(CWnd *pParent /*=NULL*/)
	: CDialog(CSelector::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSelector)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

void CSelector::DoDataExchange(CDataExchange *pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSelector)
	DDX_Control(pDX, IDC_LIST_SELECT, m_SelectorList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CSelector, CDialog)
//{{AFX_MSG_MAP(CSelector)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSelector message handlers

void CSelector::OnOK()
{
	// TODO: Add extra validation here
	// 验证指针是否有效
	VERIFY(m_pListForQuery);

	// 遍历选择的项目
	POSITION pos = m_SelectorList.GetFirstSelectedItemPosition();
	// 没有选择,返回
	if (pos == NULL)
	{
		CDialog::OnCancel();
		return;
	}
	else
	{
		BOOL bSuccess = TRUE;
		m_pListForQuery->SetRedraw(false);
		while (pos)
		{
			int nItem = m_SelectorList.GetNextSelectedItem(pos);
			CString strServiceName, strDisplayName;

			strServiceName = m_SelectorList.GetItemText(nItem, 1);
			strDisplayName = m_SelectorList.GetItemText(nItem, 0);

			int nCount = m_pListForQuery->GetItemCount();
			// 判断是否重复
			BOOL bDouble = FALSE;
			for (int i = 0; i < nCount; i++)
			{
				CString strCompare = m_pListForQuery->GetItemText(i, 1);
				if (strCompare == strServiceName)
				{
					bDouble = TRUE;
					break;
				}
			}
			if (bDouble)
				continue;

			// 写入列表
			int iInsPos = m_pListForQuery->InsertItem(nCount, "");
			if (m_pListForQuery->SetItemText(iInsPos, 0, strDisplayName) == 0)
				bSuccess = FALSE;
			if (m_pListForQuery->SetItemText(iInsPos, 1, strServiceName) == 0)
				bSuccess = FALSE;
		}
		m_pListForQuery->SetRedraw(true);

		if (!bSuccess)
			AfxMessageBox(_T("操作过程中遇到错误!"));
	}

	CDialog::OnOK();
}

BOOL CSelector::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO: Add extra initialization here
	// 初始化 CListCtrl
	m_SelectorList.InsertColumn(0, _T("显示名称"), LVCFMT_LEFT, 70);
	m_SelectorList.InsertColumn(1, _T("服务名称"), LVCFMT_LEFT, 70);

	m_SelectorList.SetExtendedStyle(m_SelectorList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE);

	FillList();
	AutoSizeColumns(&m_SelectorList);
	m_SelectorList.SetFocus();

	return FALSE; // return TRUE unless you set the focus to a control
				  // EXCEPTION: OCX Property Pages should return FALSE
}

BOOL CSelector::FillList()
{
	HKEY hRegServicesKey = 0;
	LONG lRet = 0L;

	lRet = RegOpenKey(HKEY_LOCAL_MACHINE, "SYSTEM\\CurrentControlSet\\Services",
					  &hRegServicesKey);

	if (lRet == ERROR_SUCCESS)
	{
		DWORD dwSubKeys = 0, dwMaxSubKeyNameLen = 0;
		lRet = RegQueryInfoKey(hRegServicesKey, NULL, NULL, NULL, &dwSubKeys, &dwMaxSubKeyNameLen,
							   NULL, NULL, NULL, NULL, NULL, NULL);

		if (lRet != ERROR_SUCCESS)
		{
			CString errStr;
			errStr.Format("查询服务信息时出现错误,错误号 #%ld .\n任务失败!", lRet);
			AfxMessageBox(_T(errStr));

			return FALSE;
		}

		SC_HANDLE hSCManager = OpenSCManager(GetLocalMachineName(), NULL, GENERIC_READ);

		if (hSCManager == NULL)
		{
			CString errStr;
			errStr.Format("打开服务管理句柄时出现错误.\n任务失败!");
			AfxMessageBox(_T(errStr));

			return FALSE;
		}

		VERIFY(dwMaxSubKeyNameLen <= MAX_PATH + 1);
		m_SelectorList.SetRedraw(false);
		for (DWORD dwIndex = 0; dwIndex < dwSubKeys; dwIndex++)
		{
			//	取得服务名称
			char pszServiceName[MAX_PATH + 1] = "";
			DWORD dwServiceNameLen = MAX_PATH + 1;
			lRet = RegEnumKey(hRegServicesKey, dwIndex, pszServiceName, dwServiceNameLen);

			if (lRet == ERROR_NO_MORE_ITEMS)
				break;

			if (lRet != ERROR_SUCCESS)
				continue;

			// 检查是否为服务
			SC_HANDLE hService = OpenService(hSCManager, pszServiceName, GENERIC_READ);

			if (hService == NULL)
				continue;

			DWORD cbBytesNeeded;
			LPQUERY_SERVICE_CONFIG pqsc;
			// pass a false size to QueryServiceConfig
			QueryServiceConfig(hService, NULL, 0, &cbBytesNeeded);
			pqsc = (LPQUERY_SERVICE_CONFIG)malloc(cbBytesNeeded);
			if (QueryServiceConfig(hService, pqsc, cbBytesNeeded, &cbBytesNeeded) == 0)
			{
				free(pqsc);
				CloseServiceHandle(hService);
				hService = 0;

				continue;
			}

			if (!(pqsc->dwServiceType & SERVICE_WIN32))
			{
				free(pqsc);
				CloseServiceHandle(hService);
				hService = 0;

				continue;
			}

			// 插入列表
			int nCount = m_SelectorList.GetItemCount();
			int iInsPos = m_SelectorList.InsertItem(nCount, "");
			m_SelectorList.SetItemText(iInsPos, 0, _T(pqsc->lpDisplayName));
			m_SelectorList.SetItemText(iInsPos, 1, _T(pszServiceName));

			free(pqsc);
			CloseServiceHandle(hService);
			hService = 0;
		}
		m_SelectorList.SetRedraw(true);

		if (CloseServiceHandle(hSCManager) == 0)
			AfxMessageBox(_T("操作即将完成时出现意外错误!"));
	}
	else
	{
		CString errStr;
		errStr.Format("打开注册表时出现错误,错误号 #%ld .\n任务失败!", lRet);
		AfxMessageBox(_T(errStr));

		return FALSE;
	}

	if (RegCloseKey(hRegServicesKey) != ERROR_SUCCESS)
		AfxMessageBox(_T("操作即将完成时出现意外错误!"));

	return TRUE;
}

BOOL CSelector::Init(CListCtrl *lpList)
{
	if (lpList)
	{
		m_pListForQuery = lpList;
		return TRUE;
	}

	return FALSE;
}
