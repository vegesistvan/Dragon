#pragma once


// CSaveFirstName dialog

class CSaveFirstName : public CDialogEx
{
	DECLARE_DYNAMIC(CSaveFirstName)

public:
	CSaveFirstName(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSaveFirstName();


// Dialog Data
	enum { IDD = IDD_SAVEFIRSTNAME };
	CString m_first_name;
//	int m_sex_id;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();

	int m_sex_id;
};
