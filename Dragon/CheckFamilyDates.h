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
	CString marriage;
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
	CString father_id;
	CString mother_id;
	int		mother_index;
	CString message;
}CHILDREN;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

	int m_type;

	CString m_command;
	CString str;
	CString m_filespec;

	FILE * textF;
	CProgressWnd wndP;

	CListCtrlEx m_ListCtrl;
	CColorStatic colorKeres;
	CColorStatic colorNext;

//	std::vector<HUSBAND> vHusband;
	std::vector<WIFES> vWifes;
	std::vector<CHILDREN> vChildren;
	std::vector<TCHAR*> vList;

	CStringArray m_colors;
	UINT	m_rgb[10];

	HUSBAND h;
	WIFES w;
	CHILDREN c;


	
	int	m_maxLifeSpan;			// maximális életkor
	int	m_minAgeWedding; 


	int	m_maxDiffCM;		// gyerek és anya max korkülönbsége
	int	m_minDiffCM;		// gyerek és anya min korkülönbsége
	int m_minDiffCF;		// gyerek és apa mainimális korkülönbsége
	int	m_maxDiffCF;		// gyerek és apa maximális korkülönbsége


	int m_cnt;

	void createColumns();
	bool collectFamily();
	void printFamily();
	void checkFamily();
	void push( CString item );
	void emptyLine();
	void keress( int start );


	void CCheckFamilyDates::OnHtmlEditLines();
	void CCheckFamilyDates::OnHtmlNotepad();
	void CCheckFamilyDates::OnHtmlFamily();
	void CCheckFamilyDates::OnDbEdit();
	void OnHtmlFatherAndSiblings();

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
