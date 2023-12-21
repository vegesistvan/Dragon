#pragma once
#include "listboxex.h"
#include "listctrlex.h"
#include "colorstatic.h"


// CSelectFamily dialog

class CSelectFamily : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectFamily)

public:
	CSelectFamily(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectFamily();

// Dialog Data
	enum { IDD = IDD_SELECTFAMILY };
	CString m_familyNameSelected;
	CString	m_familyNumberSelected;

	CString m_familyName;
	int	m_familyNumber;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	int m_orderix;
	void keress( int start );

	DECLARE_MESSAGE_MAP()
public:
	

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
//	afx_msg void OnChangeSearch();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	CListCtrlEx m_ListCtrl;
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedKeress();
	CColorStatic colorKeress;
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
