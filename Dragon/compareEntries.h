#pragma once
#include "afxdialogex.h"


// CCompareEntries dialog

typedef struct
{
	CString	sorszam;
	CString source;
	CString line;
	CString rowid;
	CString name;
	CString birth;
	CString death;
	CString lineF;
	CString rowidF;
	CString father;
	CString birthF;
	CString deathF;
	CString lineM;
	CString rowidM;
	CString mother;
	CString birthM;
	CString deathM;

	CString lineGF;
	CString rowidGF;
	CString grandfather;
	CString birthGF;
	CString deathGF;

	CString lineGM;
	CString rowidGM;
	CString grandmother;
	CString birthGM;
	CString deathGM;

	CString lineGF2;
	CString rowidGF2;
	CString grandfather2;
	CString birthGF2;
	CString deathGF2;

	CString lineGM2;
	CString rowidGM2;
	CString grandmother2;
	CString birthGM2;
	CString deathGM2;


	bool	printed;
}NONUNITED;

typedef struct
{
	CString line;
	CString source;
	CString united;
	CString numOfSpouses;
	CString numOfChildren;
	CString rowid;
	CString name;
	CString sex_id;
	CString birth;
	CString death;
	CString father_id;
	CString mother_id;
}PEOPLESTRUCT;


enum
{
	S = 0,
	X,  // source
	L,	// linenumber			emberünk
	R,	// rowid
	N,	// név
	B,	// születés
	D,	// halál

	LF,	// lineneumber			apja
	RF,	// rowid
	NF,	// apa neve
	BF,	// születés
	DF,	// halál

	LM,	// linenumber			anyja
	RM,	// rowid
	NM,	// anya neve
	BM,	// születés
	DM,	// halál
};


class CCompareEntries : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareEntries)

public:
	CCompareEntries(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCompareEntries();


#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_ENTRIES };
#endif

	CString rowid1;
	CString rowid2;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;

	int nItemS;
	int m_col;

	NONUNITED n1;		// egyik ember
	NONUNITED n2;		// másik ember

	std::vector<NONUNITED> vS;  // házastársak
	
	PEOPLESTRUCT	p;

	void CCompareEntries::createScreen();
	bool CCompareEntries::getRelatives( CString sorszam, CString rowid, NONUNITED* n );
	bool CCompareEntries::getPeople( CString rowid );
	void CCompareEntries::displayData();
	void CCompareEntries::listPeople(NONUNITED* n, int nItem, int cnt);
	void CCompareEntries::listSpouses();
	void CCompareEntries::emptyLine();
	void CCompareEntries::setItem(CListCtrlEx* lc, int nItem, int X, CString item, TCHAR kar);

	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnCustomdrawList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnCustomdrawList4(NMHDR* pNMHDR, LRESULT* pResult);

	afx_msg void OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList2(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList3(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclkList4(NMHDR* pNMHDR, LRESULT* pResult);

	CListCtrlEx m_ListCtrl1;
	CListCtrlEx m_ListCtrl2;
	CListCtrlEx m_ListCtrl3;
	CListCtrlEx m_ListCtrl4;

	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnRokonok();
	CListCtrlEx m_ListCtrl5;
	CListCtrlEx m_ListCtrl6;
};
