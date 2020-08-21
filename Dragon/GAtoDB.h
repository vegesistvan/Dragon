#pragma once
#include "colorstatic.h"
#include "afxwin.h"


// CGaToDb dialog

class CGaToDb : public CDialogEx
{
	DECLARE_DYNAMIC(CGaToDb)

public:
	CGaToDb(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaToDb();

// Dialog Data

	enum { IDD = IDD_GATODB };
	CString m_caption;
	BOOL	m_connect;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CSqliteDBRecordSet*	 m_recordset;
	CColorStatic colorComment;
//	CColorStatic colorDB;

	CString str;
	CString m_command;
	BOOL CGaToDb::setDB();
	
	BOOL	query( CString command );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	BOOL REFRESH;
	CString m_inputMode;

	afx_msg void OnClickedButtonEmpty();
//	afx_msg void OnClickedStaticDb();
	CColorStatic colorHtml;
	afx_msg void OnClickedStaticHtml();
	BOOL m_sameDBName;
//	afx_msg void OnClickedSamedb();
//	CColorStatic colorSameDB;
	CButton CheckContraction;
	afx_msg void OnBnClickedOk();
};
