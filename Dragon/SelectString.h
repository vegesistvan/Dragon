#pragma once
#include "listctrlex.h"


// CSelectString dialog

class CSelectString : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectString)

public:
	CSelectString(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectString();

// Dialog Data
	enum { IDD = IDD_SELECTSTRING };
	CString m_selected;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	CString m_command;

	void fillTable();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnBnClickedOk();
};
