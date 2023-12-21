#pragma once


// CMessage dialog

class CMessage : public CDialogEx
{
	DECLARE_DYNAMIC(CMessage)

public:
	CMessage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMessage();

// Dialog Data
	enum { IDD = IDD_MESSAGE };
	CString message;
	CString caption;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
