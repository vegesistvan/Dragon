#pragma once
#include "colorstatic.h"
#include "afxwin.h"
#include "listctrlex.h"


class CCheckParam4 : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckParam4)

public:
	CCheckParam4(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckParam4();

// Dialog Data
	enum { IDD = IDD_CHECK_PARAM4 };


	CString _info;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	CComboBox m_Combo;

	CListCtrlEx m_ListCtrl;
	CButton		m_listCtrl;
	CButton		m_allCtrl;
	CButton		m_loopCtrl;

	void fillTable();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	CColorStatic colorAzonos;
	BOOL m_all;
	BOOL m_list;
	BOOL m_loop;
	int _azonos;

	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedRadioAll();
	afx_msg void OnRadioAll1();
};
