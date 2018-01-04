// PropProcess.cpp : implementation file
//

#include "stdafx.h"
#include "SysMon.h"
#include "PropProcess.h"

#include "windows.h"
#include "Winbase.h"
#include "winperf.h"  // for Windows NT
#include "tlhelp32.h" // for Windows 95

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern void AutoSizeColumns(CListCtrl *m_List, int col = -1);

/////////////////////////////////////////////////////////////////////////////
// CPropProcess property page

IMPLEMENT_DYNCREATE(CPropProcess, CPropertyPage)

CPropProcess::CPropProcess() : CPropertyPage(CPropProcess::IDD)
{
    //{{AFX_DATA_INIT(CPropProcess)
    // NOTE: the ClassWizard will add member initialization here
    //}}AFX_DATA_INIT
}

CPropProcess::~CPropProcess()
{
}

void CPropProcess::DoDataExchange(CDataExchange *pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //{{AFX_DATA_MAP(CPropProcess)
    DDX_Control(pDX, IDC_LIST_PROCESS, m_procList);
    //}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CPropProcess, CPropertyPage)
//{{AFX_MSG_MAP(CPropProcess)
ON_WM_CONTEXTMENU()
ON_COMMAND(IDM_PROC_TERMINATE, OnProcTerminate)
ON_COMMAND(IDM_PROC_REFRESH, OnProcRefresh)
ON_COMMAND(IDM_PROC_SENDMSG, OnProcSendmsg)
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropProcess message handlers

BOOL CPropProcess::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO: Add extra initialization here
    ForceKill = TRUE;
    curTaskPos = -1;

    // 初始化 CListCtrl
    m_procList.InsertColumn(0, _T("序号"), LVCFMT_LEFT, 70);
    m_procList.InsertColumn(1, _T("进程名称"), LVCFMT_LEFT, 70);
    m_procList.InsertColumn(2, _T("Process ID"), LVCFMT_LEFT, 70);
    m_procList.InsertColumn(3, _T("Wnd Handle"), LVCFMT_LEFT, 70);
    m_procList.InsertColumn(4, _T("窗口标题"), LVCFMT_LEFT, 70);

    m_procList.InsertColumn(5, _T("优先级"), LVCFMT_LEFT, 50);
    m_procList.InsertColumn(6, _T("线程数"), LVCFMT_LEFT, 50);
    m_procList.InsertColumn(7, _T("路径"), LVCFMT_LEFT, 150);

    m_procList.SetExtendedStyle(m_procList.GetExtendedStyle() | LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP | LVS_EX_ONECLICKACTIVATE);

    m_procList.SetFocus(); // 所以函数返回 FALSE

    RefreshProcList();

    return FALSE; // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CPropProcess::OnContextMenu(CWnd *pWnd, CPoint point)
{
    // TODO: Add your message handler code here
    // 取得列表区域
    CListCtrl *pList = (CListCtrl *)GetDlgItem(IDC_LIST_PROCESS);
    VERIFY(pList != NULL);

    CRect rect, rectHeader;
    pList->GetWindowRect(&rect);
    CHeaderCtrl *pHeader = pList->GetHeaderCtrl();
    pHeader->GetWindowRect(&rectHeader);

    rectHeader.SetRect(rect.left, rect.top,
                       rect.left + rect.Width(), rect.top + rectHeader.Height() + 2);

    rect.SubtractRect(&rect, &rectHeader);
    if (!rect.PtInRect(point))
        return;
    /*
	// 验证键盘操作的弹出式菜单
	ScreenToClient(&rect);
	if( point.x == -1 && point.y == -1 )
	{
		CRect rect;
		GetClientRect(&rect);
		point = rect.TopLeft();
		point.Offset( 5, 5 );
		ClientToScreen( &point );
	}
*/
    // 取菜单资源
    CMenu mnuTop;
    mnuTop.LoadMenu(IDR_MENU_PROCESS);

    // 取子菜单
    CMenu *pPopup = mnuTop.GetSubMenu(0);
    ASSERT_VALID(pPopup);

    // 在这里 MFC 自动用 UPDATE_COMMAND_UI 机制检测菜单状态

    // 显示菜单
    pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_LEFTBUTTON,
                           point.x, point.y,
                           AfxGetMainWnd(), NULL);

    // 弹出式菜单的命令会被 MFC 的消息路由机制自动处理
}

void CPropProcess::OnProcTerminate()
{
    // TODO: Add your command handler code here
    POSITION pos = m_procList.GetFirstSelectedItemPosition();
    if (pos == NULL)
        AfxMessageBox(_T("请先选择要终止的进程!"));
    else
    {
        while (pos)
        {
            int nItem = m_procList.GetNextSelectedItem(pos);
            CString pid = m_procList.GetItemText(nItem, 2);
            curTaskPos = atoi(m_procList.GetItemText(nItem, 0)) - 1;
            int ret = Terminator(atoi(pid));
            if (ret)
            {
                m_procList.DeleteItem(nItem);
                curTaskPos = -1;
                // OnProcRefresh(); // 在速度比较慢的机器上，可能出现任务还没结束时就刷新了列表
            }
        }
    }
}

void CPropProcess::OnProcRefresh()
{
    // TODO: Add your command handler code here
    m_procList.DeleteAllItems();
    RefreshProcList();
}

///////////////////////////////////////////////////////
// Process Access Common Function

// these constants will be used below
#define INITIAL_SIZE 51200
#define EXTEND_SIZE 25600
#define REGKEY_PERF "software\\microsoft\\windows nt\\currentversion\\perflib"
#define REGSUBKEY_COUNTERS "Counters"
#define PROCESS_COUNTER "process"
#define PROCESSID_COUNTER "id process"
#define UNKNOWN_TASK _T("未知") //"unknown"

// 全局函数
BOOL CALLBACK EnumWindowsProc(HWND hwnd, DWORD lParam)
/*++
描述:
    窗口枚举的 Callback function

参数:
    hwnd -- 窗口句柄
    lParam -- ** 没有使用 **

返回值:
    TRUE -- 继续列举
--*/
{
    DWORD pid = 0;
    DWORD i;
    CHAR buf[TITLE_SIZE];
    PPROC_LIST_ENUM te = (PPROC_LIST_ENUM)lParam;
    PPROC_LIST tlist = te->tlist;
    DWORD numTasks = te->numtasks;

    // 取得窗口的进程 ID
    if (!GetWindowThreadProcessId(hwnd, &pid))
        return TRUE;

    // 在任务列表中查询当前窗口
    for (i = 0; i < numTasks; i++)
    {
        if (tlist[i].dwProcessId == pid)
        {
            // 查找顶层窗口标题
            HWND parentHWND = GetParent(hwnd);
            while (parentHWND != NULL)
            {
                hwnd = parentHWND;
                parentHWND = GetParent(parentHWND);
            }

            tlist[i].hwnd = hwnd;

            // 找到时获取窗口标题
            if (::GetWindowText(tlist[i].hwnd, buf, sizeof(buf)))
            {
                //保存窗口标题
                strcpy(tlist[i].WindowTitle, buf);
            }
            break;
        }
    }

    // 继续列举
    return TRUE;
}

BOOL CPropProcess::EnableDebugPriv95(VOID)
/*++
描述:
    改变进程优先级. Win95 系列不必改变

参数:

返回值:
    TRUE -- 成功
    FALSE -- 失败

Comments: 
    总返回 TRUE
--*/
{
    return TRUE;
}

BOOL CPropProcess::EnableDebugPrivNT(VOID)
/*++
描述:
    改变进程优先级以便完成终止进程操作

参数:

返回值:
    TRUE  -- 成功
    FALSE -- 失败
--*/
{
    HANDLE hToken;
    LUID DebugValue;
    TOKEN_PRIVILEGES tkp;

    // 开放进程的存取标示
    if (!OpenProcessToken(GetCurrentProcess(),
                          TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY,
                          &hToken))
    {
        AfxMessageBox("OpenProcessToken failed with %d\n", GetLastError());
        return FALSE;
    }

    // 获取 SE_DEBUG_NAME 特权
    if (!LookupPrivilegeValue((LPSTR)NULL,
                              SE_DEBUG_NAME,
                              &DebugValue))
    {
        AfxMessageBox("LookupPrivilegeValue failed with %d\n", GetLastError());
        return FALSE;
    }

    tkp.PrivilegeCount = 1;
    tkp.Privileges[0].Luid = DebugValue;
    tkp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;

    AdjustTokenPrivileges(hToken,
                          FALSE,
                          &tkp,
                          sizeof(TOKEN_PRIVILEGES),
                          (PTOKEN_PRIVILEGES)NULL,
                          (PDWORD)NULL);

    // AdjustTokenPrivileges 的返回值不能被测试
    if (GetLastError() != ERROR_SUCCESS)
    {
        AfxMessageBox("AdjustTokenPrivileges failed with %d\n", GetLastError());
        return FALSE;
    }

    return TRUE;
}

VOID CPropProcess::GetWindowTitles(PPROC_LIST_ENUM te)
{
    // 枚举所有的窗口
    EnumWindows((WNDENUMPROC)EnumWindowsProc, (LPARAM)te);
}

BOOL CPropProcess::MatchPattern(PUCHAR String, PUCHAR Pattern)
{
    UCHAR c, p, l;

    for (;;)
    {
        switch (p = *Pattern++)
        {
        case 0:                            // end of pattern
            return *String ? FALSE : TRUE; // if end of string TRUE

        case '*':
            while (*String)
            { // match zero or more char
                if (MatchPattern(String++, Pattern))
                    return TRUE;
            }
            return MatchPattern(String, Pattern);

        case '?':
            if (*String++ == 0) // match any one char
                return FALSE;   // not end of string
            break;

        case '[':
            if ((c = *String++) == 0) // match char set
                return FALSE;         // syntax

            c = toupper(c);
            l = 0;
            while (p = *Pattern++)
            {
                if (p == ']')     // if end of char set, then
                    return FALSE; // no match found

                if (p == '-')
                {                 // check a range of chars?
                    p = *Pattern; // get high limit of range
                    if (p == 0 || p == ']')
                        return FALSE; // syntax

                    if (c >= l && c <= p)
                        break; // if in range, move on
                }

                l = p;
                if (c == p) // if char matches this element
                    break;  // move on
            }

            while (p && p != ']') // got a match in char set
                p = *Pattern++;   // skip to end of set

            break;

        default:
            c = *String++;
            if (toupper(c) != p) // check for exact char
                return FALSE;    // not a match

            break;
        }
    }
}

BOOL CPropProcess::GetTaskList(PPROC_LIST pTaskList, DWORD dwNumTasks)
{
    DWORD dwSysID;
    dwSysID = GetSysID();

    switch (dwSysID)
    {
    case VER_PLATFORM_WIN32_NT:
        return GetTaskListNT(pTaskList, dwNumTasks);

    case VER_PLATFORM_WIN32_WINDOWS:
        return GetTaskList95(pTaskList, dwNumTasks);

    default:
        AfxMessageBox(_T("本程序只能运行在 Windows(NT, 95 或更高) 操作系统中!"));
        return FALSE;
    }
}

BOOL CPropProcess::EnableDebugPriv(VOID)
{
    DWORD dwSysID;
    dwSysID = GetSysID();

    switch (dwSysID)
    {
    case VER_PLATFORM_WIN32_NT:
        return EnableDebugPrivNT();

    case VER_PLATFORM_WIN32_WINDOWS:
        return EnableDebugPriv95();

    default:
        AfxMessageBox(_T("本程序只能运行在 Windows(NT, 95 或更高) 操作系统中!"));
        return FALSE;
    }
}

BOOL CPropProcess::Terminator(DWORD pidToKill)
{
    if (pidToKill == 0)
    {
        AfxMessageBox(_T("缺少 Process ID !"));
        return FALSE;
    }

    // 获得操作其他进程的权力
    EnableDebugPriv();

    ASSERT(curTaskList[curTaskPos].dwProcessId == pidToKill);

    return KillProcess(&curTaskList[curTaskPos], ForceKill);

    /*// 添加防止结束自身的代码
	CHAR              pname[MAX_PATH] = "";
    PROC_LIST_ENUM    te;
    DWORD             ThisPid;
    DWORD             i;
    DWORD             numTasks;
    int               rval = 0;
    char              tname[PROCESS_SIZE];
    LPSTR             p;

	// 枚举所有窗口并试图得得到窗口标题
    te.tlist = curTaskList;
    te.numtasks = numTasks;
    GetWindowTitles( &te );

    ThisPid = GetCurrentProcessId();

    for (i=0; i<numTasks; i++)
	{
        // 防止用户结束 KILL.EXE 及其父窗口
        if (ThisPid == curTaskList[i].dwProcessId) {
            continue;
        }
        if (MatchPattern( (unsigned char *)curTaskList[i].WindowTitle, (unsigned char *)"*KILL*" )) {
            continue;
        }

        tname[0] = 0;
        strcpy( tname, curTaskList[i].ProcessName );
        p = strchr( tname, '.' );
        if (p) {
            p[0] = '\0';
        }
        if (MatchPattern( (unsigned char *)tname, (unsigned char *)pname )) {
            curTaskList[i].flags = TRUE;
        } else if (MatchPattern( (unsigned char *)curTaskList[i].ProcessName, (unsigned char *)pname )) {
            curTaskList[i].flags = TRUE;
        } else if (MatchPattern( (unsigned char *)curTaskList[i].WindowTitle, (unsigned char *)pname )) {
            curTaskList[i].flags = TRUE;
        }
    }

    for (i=0; i<numTasks; i++) {
        if (curTaskList[i].flags) {
            if (KillProcess( &curTaskList[i], ForceKill ))
			{
				CString msg;
				msg.Format("进程 #%d [%s] 已被终止!", curTaskList[i].dwProcessId, curTaskList[i].ProcessName);
                AfxMessageBox(_T(msg));
            }
			else
			{
				CString msg;
				msg.Format("进程 #%d [%s] 不能终止!", curTaskList[i].dwProcessId, curTaskList[i].ProcessName);
                AfxMessageBox(_T(msg));

                rval = FALSE;
            }
        }
    }

	return TRUE;
//*/
}

BOOL CPropProcess::KillProcess(PPROC_LIST pTask, BOOL fForce)
{
    HANDLE hProcess;

    if (fForce || !pTask->hwnd)
    {
        hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pTask->dwProcessId);
        if (hProcess)
        {
            hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pTask->dwProcessId);
            if (hProcess == NULL)
            {
                return FALSE;
            }

            if (!TerminateProcess(hProcess, 1)) // 终止进程并返回退出码 1
            {
                CloseHandle(hProcess);
                return FALSE;
            }

            CloseHandle(hProcess);
            return TRUE;
        }
    }

    // 发送 WM_CLOSE 消息来结束进程
    ::PostMessage(pTask->hwnd, WM_CLOSE, 0, 0);
    //	::PostMessage( pTask->hwnd, WM_ENDSESSION, 0, 0 );
    return TRUE;
}

DWORD CPropProcess::GetTaskListNT(PPROC_LIST pTask, DWORD dwNumTasks)
{
    DWORD rc;
    HKEY hKeyNames;
    DWORD dwType;
    DWORD dwSize;
    LPBYTE buf = NULL;
    CHAR szSubKey[1024];
    LANGID lid;
    LPSTR p;
    LPSTR p2;
    PPERF_DATA_BLOCK pPerf;
    PPERF_OBJECT_TYPE pObj;
    PPERF_INSTANCE_DEFINITION pInst;
    PPERF_COUNTER_BLOCK pCounter;
    PPERF_COUNTER_DEFINITION pCounterDef;
    DWORD i;
    DWORD dwProcessIdTitle;
    DWORD dwProcessIdCounter;
    CHAR szProcessName[MAX_PATH];
    DWORD dwLimit = dwNumTasks - 1;

    //
    // Look for the list of counters.  Always use the neutral
    // English version, regardless of the local language.  We
    // are looking for some particular keys, and we are always
    // going to do our looking in English.  We are not going
    // to show the user the counter names, so there is no need
    // to go find the corresponding name in the local language.
    //
    lid = MAKELANGID(LANG_ENGLISH, SUBLANG_NEUTRAL);
    sprintf(szSubKey, "%s\\%03x", REGKEY_PERF, lid);
    rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE,
                      szSubKey,
                      0,
                      KEY_READ,
                      &hKeyNames);
    if (rc != ERROR_SUCCESS)
    {
        goto exit;
    }

    //
    // get the buffer size for the counter names
    //
    rc = RegQueryValueEx(hKeyNames,
                         REGSUBKEY_COUNTERS,
                         NULL,
                         &dwType,
                         NULL,
                         &dwSize);

    if (rc != ERROR_SUCCESS)
    {
        goto exit;
    }

    //
    // allocate the counter names buffer
    //
    buf = (LPBYTE)malloc(dwSize);
    if (buf == NULL)
    {
        goto exit;
    }
    memset(buf, 0, dwSize);

    //
    // read the counter names from the registry
    //
    rc = RegQueryValueEx(hKeyNames,
                         REGSUBKEY_COUNTERS,
                         NULL,
                         &dwType,
                         buf,
                         &dwSize);

    if (rc != ERROR_SUCCESS)
    {
        goto exit;
    }

    //
    // now loop thru the counter names looking for the following counters:
    //
    //      1.  "Process"           process name
    //      2.  "ID Process"        process id
    //
    // the buffer contains multiple null terminated strings and then
    // finally null terminated at the end.  the strings are in pairs of
    // counter number and counter name.
    //

    p = (char *)buf;
    while (*p)
    {
        if (p > (char *)buf)
        {
            for (p2 = p - 2; isdigit(*p2); p2--)
                ;
        }
        if (stricmp(p, PROCESS_COUNTER) == 0)
        {
            //
            // look backwards for the counter number
            //
            for (p2 = p - 2; isdigit(*p2); p2--)
                ;
            strcpy(szSubKey, p2 + 1);
        }
        else if (stricmp(p, PROCESSID_COUNTER) == 0)
        {
            //
            // look backwards for the counter number
            //
            for (p2 = p - 2; isdigit(*p2); p2--)
                ;
            dwProcessIdTitle = atol(p2 + 1);
        }
        //
        // next string
        //
        p += (strlen(p) + 1);
    }

    //
    // free the counter names buffer
    //
    free(buf);

    //
    // allocate the initial buffer for the performance data
    //
    dwSize = INITIAL_SIZE;
    buf = (unsigned char *)malloc(dwSize);
    if (buf == NULL)
    {
        goto exit;
    }
    memset(buf, 0, dwSize);

    while (TRUE)
    {

        rc = RegQueryValueEx(HKEY_PERFORMANCE_DATA,
                             szSubKey,
                             NULL,
                             &dwType,
                             buf,
                             &dwSize);

        pPerf = (PPERF_DATA_BLOCK)buf;

        //
        // check for success and valid perf data block signature
        //
        if ((rc == ERROR_SUCCESS) &&
            (dwSize > 0) &&
            (pPerf)->Signature[0] == (WCHAR)'P' &&
            (pPerf)->Signature[1] == (WCHAR)'E' &&
            (pPerf)->Signature[2] == (WCHAR)'R' &&
            (pPerf)->Signature[3] == (WCHAR)'F')
        {
            break;
        }

        //
        // if buffer is not big enough, reallocate and try again
        //
        if (rc == ERROR_MORE_DATA)
        {
            dwSize += EXTEND_SIZE;
            buf = (unsigned char *)realloc(buf, dwSize);
            memset(buf, 0, dwSize);
        }
        else
        {
            goto exit;
        }
    }

    //
    // set the perf_object_type pointer
    //
    pObj = (PPERF_OBJECT_TYPE)((DWORD)pPerf + pPerf->HeaderLength);

    //
    // loop thru the performance counter definition records looking
    // for the process id counter and then save its offset
    //
    pCounterDef = (PPERF_COUNTER_DEFINITION)((DWORD)pObj + pObj->HeaderLength);
    for (i = 0; i < (DWORD)pObj->NumCounters; i++)
    {
        if (pCounterDef->CounterNameTitleIndex == dwProcessIdTitle)
        {
            dwProcessIdCounter = pCounterDef->CounterOffset;
            break;
        }
        pCounterDef++;
    }

    dwNumTasks = min(dwLimit, (DWORD)pObj->NumInstances);

    pInst = (PPERF_INSTANCE_DEFINITION)((DWORD)pObj + pObj->DefinitionLength);

    //
    // loop thru the performance instance data extracting each process name
    // and process id
    //
    for (i = 0; i < dwNumTasks; i++)
    {
        //
        // pointer to the process name
        //
        p = (LPSTR)((DWORD)pInst + pInst->NameOffset);

        //
        // convert it to ascii
        //
        rc = WideCharToMultiByte(CP_ACP,
                                 0,
                                 (LPCWSTR)p,
                                 -1,
                                 szProcessName,
                                 sizeof(szProcessName),
                                 NULL,
                                 NULL);

        if (!rc)
        {
            //
            // if we cant convert the string then use a default value
            //
            strcpy(pTask->ProcessName, UNKNOWN_TASK);
        }

        if (strlen(szProcessName) + 4 <= sizeof(pTask->ProcessName))
        {
            strcpy(pTask->ProcessName, szProcessName);
            strcat(pTask->ProcessName, ".exe");
        }

        //
        // get the process id
        //
        pCounter = (PPERF_COUNTER_BLOCK)((DWORD)pInst + pInst->ByteLength);
        pTask->flags = 0;
        pTask->dwProcessId = *((LPDWORD)((DWORD)pCounter + dwProcessIdCounter));
        if (pTask->dwProcessId == 0)
        {
            pTask->dwProcessId = (DWORD)-2;
        }

        //
        // next process
        //
        pTask++;
        pInst = (PPERF_INSTANCE_DEFINITION)((DWORD)pCounter + pCounter->ByteLength);
    }

exit:
    if (buf)
    {
        free(buf);
    }

    RegCloseKey(hKeyNames);
    RegCloseKey(HKEY_PERFORMANCE_DATA);

    return dwNumTasks;
}

DWORD CPropProcess::GetTaskList95(PPROC_LIST pTaskList, DWORD dwNumTasks)
/*++
描述:
    当操作系统为 win95 或更高时, 通过 API -- Toolhelp32 -- 来得到任务列表

参数:
    dwNumTasks -- pTaskList 数组能够装载的最大 Task 数

返回值:
    返回被放在数组里的 Task 数量
--*/
{
    HANDLE hProcessSnap = NULL;
    PROCESSENTRY32 procEntry32 = {0};
    DWORD dwTaskCount = 0;

    // 保证至少需要列举一个 Task
    if (dwNumTasks == 0)
        return 0;

    // 获得当前系统进程快照
    hProcessSnap = ::CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (hProcessSnap == (HANDLE)-1)
        return 0;

    // 获取快照中每一进程的信息
    dwTaskCount = 0;
    procEntry32.dwSize = sizeof(PROCESSENTRY32); // 使用前必须设置
    if (::Process32First(hProcessSnap, &procEntry32))
    {
        do
        {
            LPSTR pCurChar; // 为双字节系统准备

            // 保存文件名和路径
            for (pCurChar = (procEntry32.szExeFile + lstrlen(procEntry32.szExeFile));
                 *pCurChar != '\\' && pCurChar != procEntry32.szExeFile;
                 --pCurChar)

                lstrcpy(pTaskList->ProcessName, pCurChar);
            pTaskList->flags = 0;
            pTaskList->dwProcessId = procEntry32.th32ProcessID;

            ++dwTaskCount;
            ++pTaskList; // 下一个 Task
        } while (dwTaskCount < dwNumTasks && ::Process32Next(hProcessSnap, &procEntry32));
    }
    else
        dwTaskCount = 0;

    // 释放 snapshot 对象
    CloseHandle(hProcessSnap);

    return dwTaskCount;
}

BOOL CPropProcess::RefreshProcList()
{
    DWORD numTasks;
    DWORD i;
    PROC_LIST_ENUM te;

    curTaskPos = -1;

    // 取得操作其他进程的权力
    EnableDebugPriv();

    memset(curTaskList, 0, sizeof(curTaskList));

    // 取得系统当前的进程列表
    numTasks = GetTaskList(curTaskList, MAX_TASKS);

    // 枚举所有窗口并试图为每个进程获得窗口标题
    te.tlist = curTaskList;
    te.numtasks = numTasks;
    GetWindowTitles(&te);

    // 刷新进程列表
    m_procList.SetRedraw(false);
    for (i = 0; i < numTasks; i++)
    {
        CString pid, wind, num;
        num.Format("%03d", i + 1);
        pid.Format("%d", curTaskList[i].dwProcessId);
        wind.Format("%d", curTaskList[i].hwnd);
        m_procList.InsertItem(i, num);
        m_procList.SetItemText(i, 1, curTaskList[i].ProcessName);
        m_procList.SetItemText(i, 2, pid);
        m_procList.SetItemText(i, 3, wind);

        if (curTaskList[i].hwnd)
        {
            m_procList.SetItemText(i, 4, curTaskList[i].WindowTitle);
        }
    }
    m_procList.SetRedraw(false);

    AutoSizeColumns(&m_procList);
    return TRUE;
}

DWORD CPropProcess::GetSysID()
{
    OSVERSIONINFO sysInfo = {0};
    sysInfo.dwOSVersionInfoSize = sizeof(sysInfo);
    GetVersionEx(&sysInfo);
    return sysInfo.dwPlatformId;
}

void CPropProcess::OnProcSendmsg()
{
    // TODO: Add your command handler code here
    ForceKill = FALSE;
    OnProcTerminate();
    ForceKill = TRUE;
}
