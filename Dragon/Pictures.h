#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "listctrlex.h"
#include "EasySizeHD.h"

#define PEOPLEX		1
#define	MARRIAGE	2
#define TABLE		3
// CPictures dialog

class CPictures : public CDialogEx
{
	DECLARE_DYNAMIC(CPictures)
	DECLARE_EASYSIZE

public:
	CPictures(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPictures();

// Dialog Data
	enum { IDD = IDD_PICTURES };

	CString m_filter;
	CString m_rowidB;		// kép rowid-ja
	CString m_rowidP;		// ember rowid-ja
	CString m_name;			// emnber neve
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	
	bool	m_paint;
	CString	m_ext;
	CString m_filename;
	CString m_fileSpec;
	int m_cnt;
	bool	m_modified;
	_int64 blob_size;
	void*	block;
	
	void CPictures::caption( int sorsz );
	bool CPictures::writeBlockToFile( void* block, int blob_size  );
	bool CPictures::paintBlob( int nIitem );
	bool CPictures::showPdf( int nItem );
	void CPictures::saveValues();
	void fillBlobTable();

	void CPictures::EditUpdate( bool all );
	void CPictures::displayPicture();

	CEdit m_TitleCtrl;
	CButton m_PrimaryCtrl;
	CSpinButtonCtrl m_SpinCtrl;
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();

	afx_msg void OnPaint();
	afx_msg void OnDeltaposSpin(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnChangeTitle();
	afx_msg void OnClickedView();
	afx_msg void OnClickedPrimary();
//	afx_msg void OnClickedDelete();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnBnClickedOk();
	CListCtrlEx m_ListCtrl;

	afx_msg void OnRdblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);

};
