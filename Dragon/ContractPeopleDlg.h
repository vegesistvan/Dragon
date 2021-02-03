#pragma once


// CContractPeopleDlg dialog

class CContractPeopleDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CContractPeopleDlg)

public:
	CContractPeopleDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CContractPeopleDlg();

// Dialog Data
	enum { IDD = IDD_CONTRACT_PEOPLE_DLG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
