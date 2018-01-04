#if !defined(AFX_PROPSETTING_H__12E752CD_1A2B_45F0_A045_04BC8292CC67__INCLUDED_)
#define AFX_PROPSETTING_H__12E752CD_1A2B_45F0_A045_04BC8292CC67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// PropSetting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CPropSetting dialog

// 综合设置属性页的类
class CPropSetting : public CPropertyPage
{
	DECLARE_DYNCREATE(CPropSetting)

	// Construction
  public:
	CPropSetting();
	~CPropSetting();

	// Dialog Data
	//{{AFX_DATA(CPropSetting)
	enum
	{
		IDD = IDD_PROPPAGE_SETTING
	};
	// NOTE - ClassWizard will add data members here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_DATA

	// Overrides
	// ClassWizard generate virtual function overrides
	//{{AFX_VIRTUAL(CPropSetting)
  protected:
	virtual void DoDataExchange(CDataExchange *pDX); // DDX/DDV support
													 //}}AFX_VIRTUAL

	// Implementation
  protected:
	// Generated message map functions
	//{{AFX_MSG(CPropSetting)
	// NOTE: the ClassWizard will add member functions here
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_PROPSETTING_H__12E752CD_1A2B_45F0_A045_04BC8292CC67__INCLUDED_)
