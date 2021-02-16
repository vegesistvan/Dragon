#pragma once
#include "listctrlex.h"


class CUnknownFirstNames : public CDialogEx
{
	DECLARE_DYNAMIC(CUnknownFirstNames)

public:
	CUnknownFirstNames(CWnd* pParent = NULL);   // standard constructor
	virtual ~CUnknownFirstNames();

	enum { IDD = IDD_UNKNOWNFIRSTNAMES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;

	CString m_command;
	CString str;
	CString m_info;

	bool inputFile();
	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnSaveAsMen();
	void OnSaveAsWomen();
	void OnSaveAsBisex();
	void OnDbEdit();

	bool insertName( CString name, int sex_id );
	bool findFirstNames();
	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLista();
	
	afx_msg void OnInfo();
};
