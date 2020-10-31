#pragma once
#include "listctrlex.h"
#include "colorstatic.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CTablePeople : public CDialogEx
{
	DECLARE_DYNAMIC(CTablePeople)
	DECLARE_EASYSIZE

public:
	CTablePeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTablePeople();

// Dialog Data
	enum { IDD = IDD_TABLE_PEOPLE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int m_orderix;
	int m_columnsCount;
	CString str;
	CString m_command;

	CWnd* m_dlg;
	CMenu menu;

	BOOL m_getFirstPeople;

	std::vector<TCHAR*> v_individuals;

	CString m_filterText;
	CString m_filterTextNew;
	CString	m_filterNew;
	CString	m_filter;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;
	

	void CTablePeople::listHtmlLine( int lineNumber );
	void CTablePeople::enableMenu( BOOL flag );
	void CTablePeople::createListColumns();
	void CTablePeople::fillTable( UINT nItem );
	void CTablePeople::fillRow( UINT row );
	void CTablePeople::push( CString str );

	int  CTablePeople::find_nItem( int rowid );

	void CTablePeople::closeHtml();


	void CTablePeople::updateField( int nItem, int i, CString str );
	void CTablePeople::updateRow( int nItem );
	void CTablePeople::insertRow();

	void CTablePeople::parent( CString rowid, PARENT* p);
	void CTablePeople::ChangeMenu();

	BOOL CTablePeople::query( CString command );
	BOOL CTablePeople::query1( CString command );
	BOOL CTablePeople::query2( CString command );
	BOOL CTablePeople::query3( CString command );
	BOOL CTablePeople::query4( CString command );

	BOOL	m_join;
	BOOL	m_limited;
	BOOL	WOMEN_DESCENDANT;

	CString m_name;
	CString	m_rowid;
	int		m_sex_id;
	CString m_rowid_father;
	CString m_htmlFile;

	int		cnt_ol;
	int		m_insert;
	FILE* fl;



	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditDelete();
//	afx_msg void OnEditInsert();
//	afx_msg void OnEditUpdate();
	afx_msg void OnRelations();
	afx_msg void OnFilterUnfilter();
	afx_msg void OnFilterAncestors();

	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);

//	afx_msg void OnChangeSearch();
	afx_msg void OnFilterFile();
	afx_msg void OnFilterMen();
	afx_msg void OnFilterNodescendant();
	afx_msg void OnFilterNoparents();
	afx_msg void OnFilterNospouse();
	afx_msg void OnFilterWomen();
	afx_msg void OnFilterFirstname();
	afx_msg void OnFilterLastname();
	afx_msg void OnFilterFamily();
	afx_msg void OnFilterTable();
	afx_msg void OnFilterIndividual();
	afx_msg void OnGaLine();
	afx_msg void OnExportSelected();
	afx_msg void OnExportAll();
	afx_msg void OnFilterFolyt();
	//afx_msg void OnAzonosember();
	afx_msg void OnClose();
	//afx_msg void OnAzonosemberConc();
	afx_msg void OnAzonosemberIdent();
	afx_msg void OnGroupbyBirthplace();
	afx_msg void OnGroupbyDeathplace();
	afx_msg void OnGroupbyFirstname();
	afx_msg void OnGroupbyLastname();
	afx_msg void OnGroupbyTitolo();
	virtual void PostNcDestroy();
	afx_msg void OnFilterDescendants();
	afx_msg void OnFilterFathers();
	afx_msg void OnFilterMothers();
	afx_msg void OnFilterSpouses();
	afx_msg void OnFilterSpousespouses();
	afx_msg void OnGroupbyBirthDate();
	afx_msg void OnGroupbyDeathdate();
	afx_msg void OnGroupbyComment();
	afx_msg void OnFilterSspouses();
	afx_msg void OnFilterSfather();
	afx_msg void OnFilterSmother();
	afx_msg void OnHtmlParents();
	afx_msg void OnHtmlLines();
	afx_msg void OnEditNotepad();
	afx_msg void OnGivenname();
	afx_msg void OnAscendants();
	afx_msg void OnAscendantsChain();
	afx_msg void OnInputManual();
	afx_msg void OnNcDestroy();
	afx_msg LRESULT OnUpdateRecord( WPARAM wParam, LPARAM lParam);
	afx_msg void OnIndividualDescendants();
	afx_msg void OnGedcomOutput();
	afx_msg void OnPrivateDescendants();
	afx_msg void OnFilterBisex();
	afx_msg void OnClickedKeres();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CColorStatic colorKeres;
};
