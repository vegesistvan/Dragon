#pragma once

#if !defined(AFX_LISTBOXEX_H__37FAF93C_B0DC_4257_9ECA_FE4F05B482F1__INCLUDED_)
#define AFX_LISTBOXEX_H__37FAF93C_B0DC_4257_9ECA_FE4F05B482F1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ListBoxEx.h : header file
//
#define WM_LISTBOX_MENU WM_USER + 0x7FA4	
/////////////////////////////////////////////////////////////////////////////
// CListBoxEx window

class CListBoxEx : public CListBox
{
// Construction
public:
	CListBoxEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CListBoxEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CListBoxEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CListBoxEx)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTBOXEX_H__37FAF93C_B0DC_4257_9ECA_FE4F05B482F1__INCLUDED_)
