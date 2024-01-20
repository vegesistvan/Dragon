#pragma once
#include "afxdialogex.h"
#include "listctrlex.h"
#include "colorstatic.h"
#include "SqliteDBEx.h"
#include "ascendants.h"
#include "ascendantsFull.h"
#include "ascendantsUnique.h"
// CAscendantsLinearTable dialog

class CAscendantsLinearTable : public CDialogEx
{
	DECLARE_DYNAMIC(CAscendantsLinearTable)

public:
	CAscendantsLinearTable(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAscendantsLinearTable();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ASCENDANTS_LINEAR };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString m_command;
	CString str;
	int m_tabCtrl;

	std::vector <FC> vFC;
	std::vector<SP> vSp;
	std::vector<SIBLINGS> vSib;

	FILE* flvGKR;		// log file
	FILE* flvGKR2;		// log file
	FILE* flvGKR3;		// log file
	FILE* flvFC;
	FILE* flvSib;
	FILE* flvSp;
	FILE* fh;

	CString fs_vGKR;
	CString fs_vGKR2;
	CString fs_vGKR3;
	CString fs_vFC;
	CString fs_vSib;
	CString fs_vSp;

	struct
	{
		int		cnt;		// sorszám
		CString rep;		// ismétlések száma	
		CString idSib;		// testvér azonosítója;
		CString idF;		// apa azonosítója
		CString idM;		// anya azonosítója
		CString idA;		// felmenõ azonosítója
		CString idSp;		// házastárs azonosítója
		CString idC;		// gyerek azonosítója
		CString idAL;
		CString spouses;
		CString siblings;
		CString children;
		CString name;
		CString description;

		int sex;		// felmenõ neme

	}col;

	bool m_log;

	CHARFORMAT cf = { 0 };

	CAscendantsFull m_aF;
	CAscendantsUnique m_aU;

	CSqliteDBRecordSet* m_recP;
	CSqliteDBRecordSet* m_recM;

	FILE* fh_Unique;
	FILE* fh_Full;
	FILE* fh_Unique_P;
	FILE* fh_Full_P;
	
	int nItem;

	CTabCtrl m_TabCtrl;
	CRichEditCtrl m_RichEditCtrl;

	CString m_titleF;
	CString m_titleU;
	CString m_familyNamePrev;

	int m_gPrev;
	bool m_gFlag;

	CEdit m_SearchCtrl;
	CComboBox m_comboCtrl;

	void CAscendantsLinearTable::printvSib();
	void CAscendantsLinearTable::printvSp();
	void CAscendantsLinearTable::printvGKR2();
	void CAscendantsLinearTable::printvFC();

	bool CAscendantsLinearTable::tableHeaderFix(int which);
	void CAscendantsLinearTable::printTopContainer(CString title, int which);
	void CAscendantsLinearTable::keress(int start);
	void CAscendantsLinearTable::dataTable(int which);
	BOOL CAscendantsLinearTable::attachDialogs();
	void CAscendantsLinearTable::parameters();
	void CAscendantsLinearTable::createColumns();
	void CAscendantsLinearTable::fillTable();
	TCHAR CAscendantsLinearTable::get_gABC(int g);
	void CAscendantsLinearTable::createAscendant(int i, int cnt, int which);
	CString CAscendantsLinearTable::getMarriage(CString rowid, CString rowidS);
	void CAscendantsLinearTable::getPeopleString(int i);
	BOOL CAscendantsLinearTable::queryP(CString command);
	BOOL CAscendantsLinearTable::queryM(CString command);
	void CAscendantsLinearTable::printOnly(CString str, int which);
	void CAscendantsLinearTable::create_vGKR2();
	void CAscendantsLinearTable::correctReferences();

	void CAscendantsLinearTable::fullTable();
	void CAscendantsLinearTable::uniqueTable();
	void CAscendantsLinearTable::getAscendant(int i, int which);
	void CAscendantsLinearTable::htmlListFull();
	void CAscendantsLinearTable::htmlListUnique();
	bool CAscendantsLinearTable::createHtmlFile( int which );
	void CAscendantsLinearTable::printF(CString str);
	void CAscendantsLinearTable::printU(CString str);
	void CAscendantsLinearTable::printData();


	CBrush m_brRed;
	CBrush m_brYellow;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

	int	m_numOfGF;
	int m_numOfGM;
	int m_numOfGDF;
	int m_numOfGDM;
	int m_numOfAF;
	int m_numOfAM;
	CString m_ascendantsPath;
	CString m_title;
	std::vector<GKR>* vGKR;

	CString m_indentedFile;

	CString m_name;
	CListCtrlEx* m_ListCtrl;

	CString s_editK;
	CString s_editG;
	BOOL m_bold;
	BOOL m_firstBold;
	BOOL m_peerPrint;
	BOOL m_checkColor;
	int  m_type;
	BOOL m_listRepeated;

	afx_msg void OnPaint();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedStaticNext();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnHtmLista();
	afx_msg void OnHtmlListaFix();
	afx_msg void OnHtmlListaPrintable();
	afx_msg void OnPiramis();
	afx_msg LRESULT CAscendantsLinearTable::OnReloadListCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT CAscendantsLinearTable::OnSetRowColor(WPARAM wParam, LPARAM lParam);//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
	afx_msg void OnIndentedAscendants();
};
