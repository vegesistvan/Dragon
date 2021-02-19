#pragma once
#include "listctrlex.h"
#include "colorstatic.h"
#include "ProgressWnd.h"

typedef struct
{
	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString death;
	bool	birthJo;
	bool	deathJo ;
	CString age;
	CString diffH;
	CString diffW;
	int		numOfSpouses;
	CString message;
}HUSBAND;

typedef struct
{
	CString rowidH;

	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString death;
	bool	birthJo;
	bool	deathJo;
	CString	age;
	CString diffH;
	CString diffW;
	CString marriage;
	bool	marriageJo;
	int		motherOrder;
	CString message;
}WIFES;


typedef struct
{
	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString death;
	bool	birthJo;
	bool	deathJo;
	CString	age;
	CString diffH;
	CString diffW;
	CString father_id;
	CString mother_id;
	int		mother_index;
	CString message;
}CHILDREN;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCheckFamilyDates : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckFamilyDates)

public:
	CCheckFamilyDates(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckFamilyDates();

// Dialog Data
	enum { IDD = IDD_CHECK_FAMILYDATES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_EASYSIZE

	_int64 oneyear;

	CString m_info;
	CString m_command;
	CString str;
	CString m_filespec;


	CProgressWnd wndP;

	CListCtrlEx m_ListCtrl;
	CColorStatic colorKeres;
	CColorStatic colorNext;

	std::vector<WIFES> vWifes;
	std::vector<CHILDREN> vChildren;
	std::vector<TCHAR*> vList;

	UINT	m_rgb[10];
	UINT	m_colorWife;

	HUSBAND h;
	WIFES w;
	CHILDREN c;

	// átvett adatok
	int	m_type;
    int	m_maxLifespan;
	int	m_maxAgeHAtWedd;
	int	m_minAgeHAtWedd;
	int	m_maxAgeWAtWedd;
	int	m_minAgeWAtWedd;
	int	m_maxDiffFC;
	int	m_minDiffFC;
	int	m_minDiffMC;
	int	m_maxDiffMC;



	int m_cnt;

	void createColumns();
	bool collectFamily();
	void printFamily();
	void checkFamily();
	void checkFamily1();
	void push( CString item );
	void emptyLine();
	void keress( int start );

	void OnHtmlEditLines();
	void OnHtmlNotepad();
	void OnHtmlNotepadParents();
	void OnHtmlFatherAndSiblings();
	void OnDbEdit();

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;


public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	
	afx_msg void OnClickedKeres();
	afx_msg void OnClickedNext();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnInfo();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
};
