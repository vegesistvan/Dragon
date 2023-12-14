#pragma once


// CEditMarriage dialog

class CEditMarriage : public CDialogEx
{
	DECLARE_DYNAMIC(CEditMarriage)

public:
	CEditMarriage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditMarriage();

// Dialog Data
	enum { IDD = IDD_EDIT_MARRIAGE };

	CString m_rowid;	// marriage rowid
	CString m_rowidH;		// husband rowid;
	CString m_rowidW;		// wife rowid;

	CString m_whichHusband;
	CString m_whichWife;
	CString m_place;
	CString m_date;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString m_command;
	CString str;

	
	
	CString getSpouse( CString rowid );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
