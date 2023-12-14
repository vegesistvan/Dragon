#pragma once
#include "colorstatic.h"


// CSelectTableName dialog

class CSelectTableName : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectTableName)

public:
	CSelectTableName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectTableName();

	CString str;
	std::vector<CString> vRowid;
// Dialog Data
	enum { IDD = IDD_SELECTTABLENAME };

	CString m_tableNameSelected;
	int		m_tableNumberSelected;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
	CColorStatic ColorTableName;
	afx_msg void OnClickedTableName();
	CString m_num;
	CString m_number;
};
