


#pragma once

#if !defined(AFX_DESCENDANT_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_)
#define AFX_DESCENDANT_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_

enum 
{
	SZLUHA = 0,
	VIL,
	TUP,
};


#include "afxwin.h"
#include "GA_structures.h"
#include "colorstatic.h"
class CGaDescendants : public CDialogEx
{
	DECLARE_DYNAMIC(CGaDescendants)

public:
	CGaDescendants(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaDescendants();

// Dialog Data
	enum { IDD = IDD_GA_DESCENDANTS };
	BOOL m_join;
	BOOL m_limited;
	BOOL _woman;				// nõk leszármazottait is kistázza-e

	CString m_rowid1;
	CString m_name;
	CString m_tableNumber;
	int		m_source;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	CString m_htmlFile;
	CString cLine;

	UINT m_genPrev;
	int		cnt_ol;
	UINT	m_ixBgrd;
	UINT	m_colorBgrnd;

	CString m_tag1;		// <ol> vagy <ul>
	CString m_tag2;		// </ol> vagy </ul>

	int		m_sex_id;
	CString m_mother_index;

	BOOL m_numOfSpouses;
	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;
	CSqliteDBRecordSet m_recordset4;
	CSqliteDBRecordSet recP;

	PPEOPLE p;
	PPEOPLE s;
	PPEOPLE sf;
	PPEOPLE sm;
	PPEOPLE ss;

	FILE* fl;

	std::vector<DESCENDANTS> vDesc;		// A vDesc vektoban egy leszármazott addig marad benn, amíg minden gyermekét listáztuk
	std::vector<UINT>vSerial;			// A Meurgey de Tupigny számozáshoz a sorszámok nyilvántartása

	DESCENDANTS desc;

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );
	
	void CGaDescendants::treePeople();
	void CGaDescendants::treeTables();

	BOOL openHtml( CString file, CString title, UINT colorBgrnd );
	void closeHtml();
	void queryPeople( CString rowid, PPEOPLE* p );
	void queryP( CString rowid );
	int	 getNumberOfChildren( CString rowid, int ix );
	CString getNextChildRowid( UINT ix );
	void descendants();

	void print( CString str );
	void printGAline( UINT ix );
	void printBegining( int ix );
	void printDescendant( int ix );
	void printMarriages();
	void printMarriage( CString place, CString date, int i, int numberOfSpouses );
	void printSpouse();
	void printSpRelatives();

	void createStyle();

	CString getLastname( PPEOPLE* p );
	CString getFirstname( PPEOPLE* p );
	CString getLastFirst( PPEOPLE* p );
	CString getFullname( PPEOPLE* p );
	CString getPlaceDateBlock( CString place, CString date, TCHAR jel );
	CString getCommentBlock( CString comment, CString occupation  );

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	BOOL m_connect;
	BOOL m_woman;

	int		m_numbering;

	CComboBox m_ComboSpec;
	int	m_ixName;
	int	m_ixSpec;
	int	m_ixComment;

	BOOL m_CheckLastName;
	BOOL m_code;

	CComboBox m_ComboComm;
	CComboBox m_ComboName;
	int m_setCombo;

	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedRadioClear();
	afx_msg void OnRadioDefault();

	CComboBox m_ComboBgrd;

	afx_msg LRESULT OnCtlColorBtn( WPARAM wparam, LPARAM lparam );
	afx_msg void OnClickedCheckWoman();
	afx_msg void OnClickedCheckConnect();

	afx_msg void OnClickedSzluha();
	afx_msg void OnTupigny();
	afx_msg void OnVillers();
};
#endif // !defined(AFX_DESCENDANT_H__930379D8_BDD1_4973_93FF_041F3F3811E4__INCLUDED_)