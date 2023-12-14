#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


// CTableGaTables dialog

class CTableGaTables : public CDialogEx
{
	DECLARE_DYNAMIC(CTableGaTables)
public:
	CTableGaTables(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableGaTables();

	CString m_fileName;
	CString m_fileSpec;
// Dialog Data
	enum { IDD = IDD_TABLE_GATABLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int m_orderix;
	CString str;
	FILE* fl1;

	CListCtrlEx m_ListCtrl;
	CColorStatic colorKeress;
	void keress( int start );
	void fillTable( );
	int CTableGaTables::extract( FILE* flOut, int tablePosition, int tableLength );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnExportAll();
	afx_msg void OnExportSelected();
	afx_msg void OnExtractTables();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedKeress();
};
