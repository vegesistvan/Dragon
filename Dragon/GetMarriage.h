#pragma once
#include "colorstatic.h"
#include "afxwin.h"


// CGetMarriage dialog

class CGetMarriage : public CDialogEx
{
	DECLARE_DYNAMIC(CGetMarriage)

public:
	CGetMarriage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGetMarriage();

// Dialog Data
	enum { IDD = IDD_GET_MARRIAGE };

	CString	m_sex_id;
	CString m_date;
	CString m_place;
	
	CString m_name;
	CString m_spouse;
	CString m_husband;
	CString m_wife;

	CString m_rowidM;
	
	CString m_rowidS1;
	CString m_rowidS2;
	
	CString m_command;
	
	int m_numberOfWifes;
	int	m_orderH;
	int	m_orderW;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CColorStatic colorSpouse;
	CString str;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnClickedStaticSpouse();
	afx_msg void OnBnClickedOk();

	CComboBox m_ComboHOrder;
	CComboBox m_ComboWOrder;
};
