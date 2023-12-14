#pragma once

#if !defined(AFX_SELECTONE_H__A0CC4F22_3D9A_4C5B_A9A0_FAE823633734__INCLUDED_)
#define AFX_SELECTONE_H__A0CC4F22_3D9A_4C5B_A9A0_FAE823633734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
/////////////////////////////////////////////////////////////////////////////
// CSelectOne dialog

class CSelectOne : public CDialog
{
// Construction
public:
	CSelectOne(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectOne();
// Dialog Data

	enum { IDD = IDD_SELECTONE };

	CListCtrlEx	m_ListCtrl;
	CString	m_search;
	CString	m_caption;		
	CString	m_table;
	CString m_column;
	CString m_select_id;
	CString	m_selected;
	CString	m_id;
	CString	m_filter;
	INT_PTR	m_orderix;

	int		m_dbfile;
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSelectOne)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL


// Implementation
protected:
	int			m_focused;
	CString		str;
	CSqliteDBRecordSet*	m_recordset;

	CString	m_command;
	void	fillTable();
	BOOL	query( CString command );
		
	void	keress( int start );
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);	//!< set color of column
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);	//!< signal column number to be colored
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedButtonClear();
	afx_msg void OnClickedKeress();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SELECTONE_H__A0CC4F22_3D9A_4C5B_A9A0_FAE823633734__INCLUDED_)
