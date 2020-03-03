#pragma once
#include "listctrlex.h"
#include "colorstatic.h"

// CMarriageEdit dialog

class CMarriageEdit : public CDialogEx
{
	DECLARE_DYNAMIC(CMarriageEdit)

public:
	CMarriageEdit(CWnd* pParent = NULL);   // standard constructor
	virtual ~CMarriageEdit();

// Dialog Data
	enum { IDD = IDD_MARRIAGEEDIT };
	CString m_rowid;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;

	void EditUpdate( bool all );
	void fillScreen();
	void fillBlobTable();
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
		afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();
	CString m_husband;
	CString m_wife;
	CString m_birthH;
	CString m_birthW;
	CString m_cntH;
	CString m_cntW;
	CColorStatic colorCntH;
	CColorStatic colorCntW;
	CString m_place;
	CString m_date;
	afx_msg void OnClickedPhotos();
	CColorStatic colorPhotos;
};
