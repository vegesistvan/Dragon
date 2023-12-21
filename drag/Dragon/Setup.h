#pragma once


// CSetup dialog

class CSetup : public CDialogEx
{
	DECLARE_DYNAMIC(CSetup)

public:
	CSetup(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSetup();

// Dialog Data
	enum { IDD = IDD_SETUP };

	CString m_fatherAgeMin;
	CString m_fatherAgeMax;

	CString m_motherAgeMin;
	CString m_motherAgeMax;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnKillfocusFatheragemax();
	afx_msg void OnKillfocusFatheragemin();
	afx_msg void OnKillfocusMotheragemax();
	afx_msg void OnKillfocusMotheragemin();
	afx_msg void OnBnClickedOk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
//	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};
