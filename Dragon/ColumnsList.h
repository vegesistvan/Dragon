#pragma once
#include "listctrlex.h"


// CColumnsList dialog

class CColumnsList : public CDialogEx
{
	DECLARE_DYNAMIC(CColumnsList)

public:
	CColumnsList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColumnsList();
	int	m_selected;
// Dialog Data
	enum { IDD = IDD_COLUMNSLIST };
	int		m_numberOfColumns;
	CListCtrl* p_ListCtrl;
//	BOOL * selected;
	std::vector<BOOL>* selected;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString	str;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheckUnderline();
	BOOL m_underline;
};
