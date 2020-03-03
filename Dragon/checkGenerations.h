#pragma once


// CcheckGenerations dialog

class CcheckGenerations : public CDialogEx
{
	DECLARE_DYNAMIC(CcheckGenerations)

public:
	CcheckGenerations(CWnd* pParent = NULL);   // standard constructor
	virtual ~CcheckGenerations();

// Dialog Data
	enum { IDD = IDD_CHECK_GENERATIONS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	int		m_cnt;
	FILE*	fh1;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
