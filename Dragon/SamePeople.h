#pragma once
#include "listctrlex.h"


// CSamePeople dialog

class CSamePeople : public CDialogEx
{
	DECLARE_DYNAMIC(CSamePeople)
	DECLARE_EASYSIZE

public:
	CSamePeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSamePeople();

// Dialog Data
	enum { IDD = IDD_SAMEPEOPLE };

	BOOL	_onlyList;
	BOOL	_html;
	BOOL	_unite;
	BOOL	_loop;
	CString _fullname;
	CString _first_name;
	CString _last_name;
	
	int		_azonos;			// azonossági kritériumok fennállásánalk elõírt száma ( default = 1 );

	void	samePeople();
	void	findSamePeople( CString firstName, CString lastName, BOOL conc );
	void	sameMarriages( int iter );


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;
	
	CString fileSpec;
	CString str;
	CString m_command;
	CString m_identical;
	int		m_azonos;			// azonossági kritériumok fennállásánalk aktuális száma 
	
	CString m_timeconflict;

	CString p_fields;
	CString s_fields;
	CString k_fields;

	
	CString _contractions;

	FILE *	fl;
	FILE* fh1;
	FILE* fh2;
	int		m_cnt;

	int		m_inputP;
	int		m_allP;
	int		m_unitedP;
	int		m_maradtP;
	int		m_deletedP;

	int		m_inputM;
	int		m_deletedM;
	int		m_maradtM;

	int		m_numberOfColumns;
	int		nItem;

	CString _tag;

	CString drive;
	CString path;
	CString filename;
	CString ext;


	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	CStringArray m_colors;
	UINT	m_rgb[10];
	
	std:: vector<SAME> vSame;
	std::vector<REPLACE> v_Replace;
	std::vector<SPOUSES> v_spouses1;
	std::vector<SPOUSES> v_spouses2;
	std::vector<KIDS> v_kids1;
	std::vector<KIDS> v_kids2;

	
	void same();
	void listSameVector( CString name );

	void listColumnHeader( int i );
	BOOL sameLastName();
	void processSame();
	void getSpouses( CString rowid, CString sex_id );
	void getKids( CString rowid, CString sex_id );

	int		markGroup();
	BOOL	identical( int ix, int jx );
	int		sameBirth( int i, int j );
	int		sameDeath( int i, int j );
	int		dateSame( CString date1, CString date2 );
	int		sameFather( int i, int j );
	int		sameMother( int i, int j );
	int		sameSpouse( int ix, int jx );
	int		sameKids( int ix, int jx );
	int		timeConflict( int ix, int jx);
	void	listRecord(  int i, int cnt, int volt );
	void	to_v_Replace( int replace, int by );
	void	unite( int i );
	
	BOOL	dateDiff(  CString date1, CString date2, int month  );
	CString	getDateI( CString dateS, int month ) ;

	void	pushSame();
	void	htmlHeader();
	void	htmlFooter();

	
	void	collectSameName( CString name );


	BOOL	query( CString command );
	BOOL	query1( CString command );
	BOOL	query2( CString command );
	BOOL	query3( CString command );
	BOOL	query4( CString command );

	
	void	title( int contracted );
	void	createColumns();
	void	fillTable( int contracted );




	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtmlEdit();
	afx_msg void OnHtmlShows();
	afx_msg void OnHtmlPeoplefather();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnContracted();
	afx_msg void OnNotcontracted();
	afx_msg void OnAll();
};
