#pragma once

#include "resource.h"
#include "listctrlex.h"
#include "colorstatic.h"
#include "SqliteDBEx.h"
#include <algorithm>
#include "uniteEntries.h"
#include "uniteStructures.h"
namespace PP
{
	typedef struct
	{
		int g;
		int k;
		CString r;
		CString name;
	}GKR;
};

enum
{
	ALL = 0,
	ROWIDPEOPLE,
	FULLNAME,
	LASTNAME,
	FIRSTNAME,
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CTablePeople : public CDialogEx
{
	DECLARE_DYNAMIC(CTablePeople)

public:
	CTablePeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTablePeople();

// Dialog Data
	enum { IDD = IDD_TABLE_PEOPLE };

	int m_list;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	int m_orderix;
	int m_columnsCount;
	CString str;
	CString m_command;

	CWnd* m_dlg;
	CMenu menu;

	CHARFORMAT cf = { 0 };

	CRichEditCtrl captionCtrl;

	BOOL m_getFirstPeople;

	std::vector<TCHAR*> v_individuals;
	std::vector<UNITE_ENTRIES> vUnite;
	std::vector<USAME> vSameNames;


	CString m_filterText;
	CString	m_filter;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;
	

	void CTablePeople::enableMenu( BOOL flag );
	void CTablePeople::createListColumns();
	void CTablePeople::fillTable( UINT nItem );

	void CTablePeople::addEntry();
	void CTablePeople::updateEntry();

	void CTablePeople::push( CString str );
	CString CTablePeople::ancestor( CString father_id );
	bool CTablePeople::setAncestor( CString rowid  );

	void CTablePeople::updateField( int nItem, int i, CString str );
	bool CTablePeople::contractSameItems();


	void keress( int start );


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
	CString	m_rowid;			// a szerkesztett bejegyzás rowid-je
	int		m_nItem;			// a szerkesztendõ sor nItem-je
	CString	m_sex_id;
	CString m_rowid_father;
	CString m_htmlFile;


	int		m_insert;
	FILE* fl;

	CColorStatic colorKeres;
	CColorStatic colorNext;
	CListCtrlEx m_ListCtrl;
	bool CTablePeople::getParentsId(CString rowid);

	std::vector<PP::GKR> vGKR;
	CString m_filespecTxt;
	CString m_father_id;
	CString m_mother_id;
	CString m_father;
	CString m_mother;
	
	
	
	



	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	

	afx_msg void OnFilterUnfilter();
	afx_msg void OnFilterAncestors();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
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
	afx_msg void OnClose();
	afx_msg void OnAzonosemberIdent();
	afx_msg void OnGroupbyBirthplace();
	afx_msg void OnGroupbyDeathplace();
	afx_msg void OnGroupbyFirstname();
	afx_msg void OnGroupbyLastname();
	afx_msg void OnGroupbyTitolo();
	afx_msg void OnFilterDescendants();
	afx_msg void OnFilterFathers();
	afx_msg void OnFilterMothers();
	afx_msg void OnFilterSpouses();
	afx_msg void OnFilterSpousespouses();
	afx_msg void OnGroupbyBirthDate();
	afx_msg void OnGroupbyDeathdate();
	afx_msg void OnGroupbyComment();
	
	afx_msg void OnGivenname();
	afx_msg void OnInputManual();
	afx_msg void OnGedcomOutput();
	afx_msg void OnFilterBisex();
	afx_msg void OnClickedKeress();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnSyncronize(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnHtmlEditLines();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlNotepadParents();
	afx_msg void OnHtmlFatherAndSiblings();

	afx_msg void OnEditDelete();
	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();
	afx_msg void OnHtmlChildren();
	afx_msg void OnInfo();
	afx_msg void OnClickedNext();

	afx_msg void OnSetAncestor();
	afx_msg void OnGedcomIndi();
	afx_msg void OnFilterTableAncestors();
	afx_msg void OnFilterFamilyAncestors();
	afx_msg void OnGroupbyPosterior();
	afx_msg void OnUnite();
	afx_msg void OnCompare2People();
	afx_msg void OnKekule();
	afx_msg void OnCompareAsc();
	
	afx_msg void OnEditNotepad();
	afx_msg void OnRokonok();
	afx_msg void OnBirthDeath();
	afx_msg void OnDescendants();
	afx_msg void OnGedcomDelete();
};
