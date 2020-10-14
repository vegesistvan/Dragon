#pragma once


// CCheckGenerations1 dialog

class CCheckGenerations1 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckGenerations1)

public:
	CCheckGenerations1(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckGenerations1();

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
//	afx_msg void OnBnClickedOk();
};
