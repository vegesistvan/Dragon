#pragma once


// CGetLastFirst dialog

class CGetLastFirst : public CDialogEx
{
	DECLARE_DYNAMIC(CGetLastFirst)

public:
	CGetLastFirst(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGetLastFirst();

// Dialog Data
	enum { IDD = IDD_GETLASTFIRST };

	CString _lastname;
	CString _firstname;
	CString _fullname;
	CString	_sex_id;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
};
