#pragma once


// CSelectCodesystem dialog

class CSelectCodesystem : public CDialogEx
{
	DECLARE_DYNAMIC(CSelectCodesystem)

public:
	CSelectCodesystem(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSelectCodesystem();

// Dialog Data
	enum { IDD = IDD_SELECTCODESYSTEM };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
