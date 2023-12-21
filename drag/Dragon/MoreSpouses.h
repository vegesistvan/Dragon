#pragma once
#include "resource.h"
#include "listctrlex.h"


class CMoreSpouses : public CDialogEx
{
	DECLARE_DYNAMIC(CMoreSpouses)

public:
	CMoreSpouses(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMoreSpouses();

// Dialog Data
	enum { IDD = IDD_MORESPOUSES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;

	CString m_command;

	void CMoreSpouses::createScreen();
	void CMoreSpouses::fillTable();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	
	// bemenet:
	CString m_rowid;		// bemenet és kimenet
	CString	m_sex_id;

	// kimenet
	int		m_mother_index;		// a kiválasztott házastárs sorszáma

	afx_msg void OnBnClickedOk();
};
