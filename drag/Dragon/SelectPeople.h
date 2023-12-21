#pragma once
#include "listctrlex.h"


// CSelectPeople dialog

class CSelectPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectPeople)

public:
	CSelectPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectPeople();

// Dialog Data
	enum { IDD = IDD_SELECT_PEOPLE };
//	int		m_sex;
	CString	m_birth_date;

	CString	str;
	CString	m_command;
	CString m_caption;
	CString	m_rowid;
	CString m_filter;
	CSqliteDBRecordSet*	 m_recordset;
	INT_PTR	m_orderix;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CSelectPeople::createColumns();
	void CSelectPeople::fillTable();
	BOOL CSelectPeople::query( CString command );
	void CSelectPeople::keress( int start );
	DECLARE_MESSAGE_MAP()
public:
	CListCtrlEx m_ListCtrl;
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnClickedButtonClear();
	afx_msg void OnClickedKeress();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
