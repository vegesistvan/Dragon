#pragma once
#include "afxdialogex.h"
#include "ListCtrlColoredHeader.h"

// CAscendantsUnique dialog

class CAscendantsUnique : public CDialogEx
{
	DECLARE_DYNAMIC(CAscendantsUnique)

public:
	CAscendantsUnique(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAscendantsUnique();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASCENDANTS_UNIQUE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	CString str;
	FILE* flDesc;

	std::vector<int> vid;




	void CAscendantsUnique::OnKeressBeginning();
	void CAscendantsUnique::OnKeressNext();
	void CAscendantsUnique::OnKeressPrevious();
	void CAscendantsUnique::onAscendants(int print);
	void CAscendantsUnique::onDescendants(int print);
	DECLARE_MESSAGE_MAP()
public:
//	CListCtrlEx m_ListCtrlU;
	CListCtrlColoredHeader m_ListCtrlU;

	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnFunctionsNotepad();
	afx_msg void OnAllAscendants();
	afx_msg void OnAllDescendants();
	afx_msg void OnAllAscendants2();
	afx_msg void OnAllDescendants2();
	afx_msg void OnAscendants();
	afx_msg void OnDescendants();
	afx_msg void OnDblclkListUnique(NMHDR* pNMHDR, LRESULT* pResult);
};
