#pragma once
#include "listctrlex.h"



class CSelectMother : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectMother)

public:
	CSelectMother(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectMother();

// Dialog Data
	enum { IDD = IDD_SELECTMOTHER };
	CString m_rowidF;
	CString m_rowidM;
	CString m_mother;
	int		m_mother_index;
	int		m_numberOfWifes;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;

	CListCtrlEx m_ListCtrl;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
