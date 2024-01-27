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
	DECLARE_MESSAGE_MAP()
public:
//	CListCtrlEx m_ListCtrlU;
	CListCtrlColoredHeader m_ListCtrlU;

	virtual BOOL OnInitDialog();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDescendantsMarked();
	afx_msg void OnDescendantsListed();
	afx_msg void OnAscendantsMarked();
	afx_msg void OnAscendantsListed();
	afx_msg void OnFunctionsNotepad();
};
