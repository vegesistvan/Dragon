#pragma once

#include "listctrlex.h"
#include "colorstatic.h"
// CTableMarriages dialog

class CTableMarriages : public CDialogEx
{
	DECLARE_DYNAMIC(CTableMarriages)
	DECLARE_EASYSIZE
public:
	CTableMarriages(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableMarriages();


	CWnd* m_pParent;
// Dialog Data
	enum { IDD = IDD_TABLE_MARRIAGES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CTableMarriages::fillTable( UINT nItem  );
	void CTableMarriages::push( CString str );

	std::vector<TCHAR*> v_tableMarriages; 
	CString m_command;
	CString str;
	int		m_orderix;

	int		m_columnsCount;

	CString m_filter;
	CString m_filterText;
	CString m_last_name_h;
	CString m_last_name_w;

	CListCtrlEx m_ListCtrl;
	
	void	enableMenu( BOOL flag );
	void CTableMarriages::azonosMarriages( int list );
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PostNcDestroy();

	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlShows();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
//	afx_msg void OnChangeSearch();
	afx_msg void OnExportAll();
	afx_msg void OnExportSelected();
	afx_msg void OnAzonosMarriagelist();

	afx_msg void OnUnfilter();
	afx_msg void OnFilterHlastname();
	afx_msg void OnFilterWlastname();
	afx_msg void OnClose();
	afx_msg void OnGroupbyPlace();
	afx_msg void OnMarriagesDescendants();
	afx_msg void OnMarriagesSpouses();
	afx_msg void OnMarriagesParents();
	afx_msg void OnRowidMarriages();
	afx_msg void OnNameMarriages();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGahtmlLine();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditDelete();
	afx_msg void OnEditGahtml();
	CColorStatic colorKeres;
	afx_msg void OnClickedKeres();
};
