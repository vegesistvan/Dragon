#pragma once


#include "Table_people.h"
#include "Table_marriages.h"
#include "Table_html.h"
#include "html_Descendants.h"
#include "html_Spouses.h"
#include "html_SpouseSpouses.h"
#include "afxwin.h"
#include "CheckFamilyDates.h"
#include "listctrlex.h"
#include "colorstatic.h"

#ifdef RESAMPLE_EXPORTS
#define RESAMPLE_API __declspec(dllexport)
#else
#define RESAMPLE_API __declspec(dllimport)
#endif

//RESAMPLE_API HBITMAP CreateResampledBitmap(HDC hdc, HBITMAP hBmpSource, DWORD dwWidth, DWORD dwHeight, DWORD dwFilter);

//#define WM_NEWHTML		WM_USER + 0x1FFB
#define WM_NEWHTML		0xC000



class CDragonDlg : public CDialogEx
{
	DECLARE_MESSAGE_MAP()

public:
	CDragonDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CDragonDlg();

	enum { IDD = IDD_DRAGON_DIALOG };


//	int m_rowid;
//	int m_sex_id;
/*
	CString	m_familyNumber;
	CString	m_tableNumber;
	CString m_rowid_father;
	CString m_rowid_mother;
	CString m_gener;
	CString m_name;
*/
	void	mainTitle();
	int _xDest;
	int _yDest;
	int _wDest;
	int _hDest;

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support

	BOOL owners;
	CString m_title;

	CStdioFile file;

protected:

	CWnd* m_dlg; 
//	HICON m_hIcon;

	CString BUILT;
	CMenu menu;


	CString drive;
	CString dir;
	CString fname;
	CString ext;

	CString errorMsg;

	BOOL m_html_open;
	
	int	m_table_number;
	
	int m_indent;
	BOOL	m_join;		// összekapcsolja-e az elágazásokat?
	BOOL	m_limited;	// TRUE: csak a nála nagyobb generációkat listázza
	BOOL	WOMEN_DESCENDANT;

	int	m_numberOfPixelsHorizontal;
	int m_numberOfPixelsVertical;
	CString	str;
	CString	m_command;

	std::vector<TCHAR*> v_individuals;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	CString TABLE;

	CString m_join_father;
	CString	m_join_child;

	CString m_filterNew;
	CString	m_filter;

	CString		m_filterText;
	CString		m_filterTextNew;

	CString		m_substring;

	int		m_orderix;
	FILE * fl;

	int		m_fileNumber;
	int		m_lineNumber;
	CString m_familyName;
	CString m_generation;

	


	int		m_tableNumberSelected;
	CString m_tableNameSelected;

	std::vector<TCHAR*> v_peoples;
	
	HMENU	m_hMenuDefault;

	CString m_htmlFile;

	

	int		m_columnsCount;
	CString m_save;

	
	BOOL	query( CString command );
	BOOL	query1( CString command );
	BOOL	query2( CString command );
	BOOL	query3( CString command );
	BOOL	query4( CString command );
	
	void CDragonDlg::ChangeMenu();


	virtual BOOL OnInitDialog();



	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	int		m_generation_previous;
	int		m_parent_id;
	

public:

	CStatic m_picture;
///////////////////////////////////////

	afx_msg void OnDatabaseInformation();

	afx_msg void OnMarriagesFile();
	afx_msg void OnMarriagesTable();
	afx_msg void OnMarriagesLine();

	afx_msg void OnSsFile();
	afx_msg void OnSsTable();
	afx_msg void OnSsLine();


	afx_msg void OnOpenDb();
	afx_msg void OnDisplayMarriages();
	afx_msg void OnDisplayFilestable();
	afx_msg void OnInfoOnprogram();
	afx_msg void OnDisplayIndividuals();
	afx_msg void OnDisplayTables();

	afx_msg void OnPaint();

	afx_msg void OnCsaladTorzs();
	afx_msg void OnCheckLifespan();
	afx_msg void OnInputManual();
	afx_msg void OnListPeople();
	afx_msg void OnCheckTags();
	afx_msg void OnGedcomInput();
	afx_msg void OnListFamilyByName();
	afx_msg void OnGedcomVindi();
	afx_msg void OnCheckFather9();

	afx_msg void OnCheckSpousesDiff();
	afx_msg void OnFatherchildGreater();
	afx_msg void OnFatherchildLess();
	afx_msg void OnMotherchildGreater();
	afx_msg void OnMotherchildLess();
	afx_msg void OnHusbandAgeG();
	afx_msg void OnHusbandAgeL();
	afx_msg void OnWifeAgeG();
	afx_msg void OnWifeAgeL();
	afx_msg void OnCheckSpousesSex();
	afx_msg void OnSameNameAndBirth();
	afx_msg void OnSameNameAndDeath();
	afx_msg void OnSameNameAndFather();
	afx_msg void OnSameNameAndMother();
	afx_msg void OnCheckSameSpouseName();
	afx_msg void OnCheckNames();
	afx_msg void OnMotherIndex();
	afx_msg void OnDisplayBlob();
	afx_msg void OnGedcomINDIFAMS();
	afx_msg void OnClose();
	afx_msg void OnSamenameandspouse();

	afx_msg LRESULT OnMainTitle(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDisplayFamilies();
	afx_msg void OnCheckWeddingDate();
	afx_msg void OnSpousesSex();
	afx_msg void OnUnreferencedItems();
	afx_msg void OnCheckFolyt();
	afx_msg void OnFatherSamechildren();
	afx_msg void OnCheckNumofspouses();
	afx_msg void OnCheckOrderwife();
	afx_msg void OnCheckNumofchildren();
	afx_msg void OnCheckDateFormat();
	afx_msg void OnInputGA();
	afx_msg void OnOlList();
	afx_msg void OnCheckFamilyDates();
	afx_msg void OnCheckGenerations();
	afx_msg void OnCheckIntegrity();
	afx_msg void OnCheckMotherDeath();
	afx_msg void OnInputGedcom();
	afx_msg void OnListHEAD();
	afx_msg void OnListZero();
	afx_msg void OnListFile();
	afx_msg void OnGedcomVFAM();
	afx_msg void OnListChain();
	afx_msg void OnListFileTab();
	afx_msg void OnFilterIrregular();
	afx_msg void OnFolytLines();
	afx_msg void OnDescendantFile();
	afx_msg void OnDescendantLine();
	afx_msg void OnDescendantTable();
	afx_msg void OnGaEditor();
	afx_msg void OnGaOpen();
	afx_msg void OnRepeatedFamilies();
	afx_msg void OnCheckParents();

	afx_msg void OnUniteEntries();
	afx_msg void OnRichEdit();
//	afx_msg void OnGedcomSelect();
	afx_msg void OnBracketsRound();
	afx_msg void OnBracketsSquare();
	afx_msg void OnBracketsBraces();
//	afx_msg void OnMarriages2();
};
