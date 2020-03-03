#pragma once
//#include "picturectrl.h"
#include "afxwin.h"


// CEditBlob dialog

#define PEOPLE		1
#define	MARRIAGE	2
#define TABLE		3

class CEditBlob : public CDialogEx
{
	DECLARE_DYNAMIC(CEditBlob)

public:
	CEditBlob(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditBlob();

// Dialog Data
	enum { IDD = IDD_EDIT_BLOB };
	CString m_rowid;
	CString m_ext;
	CString m_id;
	int		m_table_id;
	CString m_caption;
	CString m_title;
	int		m_primary;
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_fileSpec;

	bool m_paint;
	_int64 blob_size;
	void* block;

	bool CEditBlob::writeBlockToFile( void* block, int blob_size  );
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_CheckDisplay;
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedButtonSave();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClickedButtonView();
	afx_msg void OnClickedButtonDelete();
	afx_msg void OnPaint();
	
	afx_msg void OnClickedButtonNext();
};
