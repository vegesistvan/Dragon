#pragma once
#include "afxwin.h"


// CLifespanParam dialog

class CLifespanParam : public CDialogEx
{
	DECLARE_DYNAMIC(CLifespanParam)

public:
	CLifespanParam(CWnd* pParent = NULL);   // standard constructor
	virtual ~CLifespanParam();

// Dialog Data
	enum { IDD = IDD_CHECK_LIFESPAN };

	CString _info;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	FILE * fl;
	CString m_command;

	int _lifespan;
	BOOL _mfc;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnBnClickedOk();
	CButton m_RadioMfc;
};
