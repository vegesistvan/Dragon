#pragma once


// CEditTwoLines dialog

class CEditTwoLines : public CDialogEx
{
	DECLARE_DYNAMIC(CEditTwoLines)

public:
	CEditTwoLines(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTwoLines();

// Dialog Data
	enum { IDD = IDD_EDITTWOLINES };
	std::vector<CString> *vLines;
	CString m_line1;
	CString m_line2;
	bool m_change1;
	bool m_change2;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_EASYSIZE
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnChangeEdit1();
	afx_msg void OnChangeEdit2();
	afx_msg void OnBnClickedOk();
};
