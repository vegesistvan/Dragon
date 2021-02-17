#pragma once
#include "colorstatic.h"
#include "afxwin.h"

class CGaToDb : public CDialogEx
{
	DECLARE_DYNAMIC(CGaToDb)
public:
	CGaToDb(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaToDb();

	enum { IDD = IDD_GATODB };
	CString m_caption;
	CString m_inputMode;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CSqliteDBRecordSet*	 m_recordset;
	CColorStatic colorComment;
	CColorStatic colorHtml;
	BOOL m_sameDBName;
	BOOL REFRESH;

	CString str;
	CString m_command;
	BOOL CGaToDb::setDB();
	BOOL	query( CString command );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedButtonEmpty();
	afx_msg void OnClickedStaticHtml();
};
