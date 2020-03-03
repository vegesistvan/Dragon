#pragma once


// CFirstLetter dialog

class CFirstLetter : public CDialogEx
{
	DECLARE_DYNAMIC(CFirstLetter)

public:
	CFirstLetter(CWnd* pParent = NULL);   // standard constructor
	virtual ~CFirstLetter();

// Dialog Data
	enum { IDD = IDD_FIRSTLETTER };
	
	TCHAR m_firstChar;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();


};
