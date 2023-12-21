#pragma once
#include "colorstatic.h"


// CSelectParent dialog

class CSelectParent : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectParent)

public:
	CSelectParent(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectParent();

// Dialog Data
	enum { IDD = IDD_SELECT_PARENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CSqliteDBRecordSet*	 m_recordset;


	BOOL CSelectParent::query( CString command );
	CString CSelectParent::getParent( CString rowid );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


	CString m_sex;

	CString m_caption;
	CString m_parent;
	CString m_order_parent;
	CString m_child;
	CString	m_rowid;

	CString m_birth_date;
	CString m_parent_id;
	CString m_command;
	CString str;
	CString m_filter;

	afx_msg void OnClickedStaticParent();
	CColorStatic m_parent_color;
};
