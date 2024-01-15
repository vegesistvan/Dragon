#pragma once
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsFull.h"
#include "CDescendantsUnique.h"

enum
{
	L_CLRTEXTBK = 0,		// hidden
	L_CLRTEXT,
	L_NUMOFD,
	L_ISM,
	L_ID,
	L_IDC,
	L_IDF,
	L_MINDEX,
	L_DBC,
	L_GEN,
	L_DESCENDANT,
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CDescendantsLinearTable : public CDialogEx
{
	DECLARE_DYNAMIC(CDescendantsLinearTable)

public:
	CDescendantsLinearTable(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CDescendantsLinearTable();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DESCENDANTS_LINEARTABLE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	CString m_command;
	CString str;
	int m_tabCtrl;

	bool m_log;

	CHARFORMAT cf = { 0 };

	CDescendantsFull m_aF;
	CDescendantsUnique m_aU;

	CSqliteDBRecordSet* m_recP;
	CSqliteDBRecordSet* m_recM;

	FILE* fh_Unique;
	FILE* fh_Full;
	FILE* fh_Unique_P;
	FILE* fh_Full_P;

	int nItem;

	CTabCtrl m_TabCtrl;
	

	CString m_titleF;
	CString m_titleU;

	int m_gPrev;
	bool m_gFlag;

	CBrush m_brRed;
	CBrush m_brYellow;

	CEdit m_SearchCtrl;
	CComboBox m_comboCtrl;

	//////////////

	CString m_familyName;
	CString m_lastname;
	int	m_cnt;
	CString genPrev;
	CString m_os;
	CString m_descendantsPath;

	CSqliteDBRecordSet rs;
	CSqliteDBRecordSet rs2;
	CSqliteDBRecordSet rsP;
	CSqliteDBRecordSet rsM;
	FILE* fl;
	FILE* text;
	FILE* txtOut;
	FILE* fhDescTable;
	FILE* fhPrintable;

	int m_listedP;
	int	m_listedD;
	int	m_indent;

	std::vector<UINT>vSerial;			// A Meurgey de Tupigny számozáshoz a sorszámok nyilvántartása
	std::vector<DE::LMX>vLMX;				// a generáció utolsó kiírt motherIndexe a leszármazotti listában
	std::vector<DE::LEN>* vLenOu;
	std::vector<CString>vFullname;		// már listázott házastársak neve
	std::vector<CString>vSpouseIndex;
	std::vector<int> vSib;

	DE::PEO peoS;

	bool gflag;
	void CDescendantsLinearTable::keress(int start);
	void CDescendantsLinearTable::dataTable(int which);
	int CDescendantsLinearTable::sort_vDesc();
	BOOL CDescendantsLinearTable::query(CString command);
	BOOL CDescendantsLinearTable::queryP(CString command);
	BOOL CDescendantsLinearTable::queryM(CString command);
	bool CDescendantsLinearTable::printTopContainer(CString title, int which);
	TCHAR CDescendantsLinearTable::get_gABC(int g);
	void CDescendantsLinearTable::printOnly(CString str, int which);
	CString CDescendantsLinearTable::getComplexDescription(int i, bool parentIndex);
	void CDescendantsLinearTable::printDesc(int i, int which);
	CString CDescendantsLinearTable::getColoredString(CString str, int index);
	CString CDescendantsLinearTable::getLastFirst(DE::PPEOPLE* p);
	CString CDescendantsLinearTable::getPlaceDateBlock(CString place, CString date, CString jel);
	void CDescendantsLinearTable::queryPeople(CString rowid, DE::PPEOPLE* p);
	CString CDescendantsLinearTable::getTableHeader();
	void CDescendantsLinearTable::print(CString str);
	bool CDescendantsLinearTable::voltmar(CString index);
	CString CDescendantsLinearTable::createSpRelatives();
	CString CDescendantsLinearTable::createSpouse();
	CString CDescendantsLinearTable::createMarriage(CString place, CString date, int i, int numberOfSpouses);
	CString CDescendantsLinearTable::createSpouses(int i);
	CString CDescendantsLinearTable::createDescendant(int i, bool parentIndex);


	CString CDescendantsLinearTable::getComplexDescriptionL(int i, bool parentIndex);
	CString CDescendantsLinearTable::createDescendantL(int i, bool parentIndex);
	CString CDescendantsLinearTable::createSpousesL(int i);
	CString CDescendantsLinearTable::createMarriageL(CString place, CString date, int i, int numberOfSpouses);
	CString CDescendantsLinearTable::createSpouseL();
	CString CDescendantsLinearTable::createSpRelativesL();


	DECLARE_MESSAGE_MAP()
public:

	BOOL CDescendantsLinearTable::attachDialogs();

	CRichEditCtrl m_RichEditCtrl;
	CListCtrlEx* m_ListCtrl;
	CString m_title;
	CString m_name;

	CString s_editK;
	CString s_editG;
	BOOL m_bold;
	BOOL m_firstBold;
	BOOL m_peerPrint;
	BOOL m_checkColor;
	int  m_type;
	BOOL m_listRepeated;
	CString m_htmlFile;

	
	bool m_checkConnect;
	bool m_checkMother;
	bool m_checkFolyt;
	bool m_checkCapital;
	bool m_checkBold;
	bool m_checkCRLF;
	bool m_radioOne;

	CString m_editWidth;
	CString m_editGenMax;

	int m_radioDNameX;

	int m_comboDAttrib;
	int m_comboFontSize;
	int m_comboComment;
	int m_comboSpec;
	int m_comboOtherName;

	int m_colorBgrnd;

	int m_repeated;
	int m_repeatedColor;
	int m_radioOrder;
	bool m_folyt;
	bool m_html;

	COLORREF m_clrTextBk;
	COLORREF m_clrText;

	DE::PPEOPLE p;
	DE::PPEOPLE s;
	DE::PPEOPLE sf;
	DE::PPEOPLE sm;
	DE::PPEOPLE ss;

	std::vector<DE::DESC>* vDesc;


	afx_msg void OnPaint();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedStaticNext();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnHtmLista();
	afx_msg void OnHtmlListaFix();
	afx_msg void OnHtmlListaPrintable();
	bool CDescendantsLinearTable::createHtmlFileP(int which);
	bool CDescendantsLinearTable::createHtmlFileF(int which);
	bool CDescendantsLinearTable::createHtmlFile(int i, int which);
	afx_msg void OnPiramis();
	afx_msg LRESULT CDescendantsLinearTable::OnReloadListCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT CDescendantsLinearTable::OnSetRowColor(WPARAM wParam, LPARAM lParam);//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
	virtual BOOL OnInitDialog();
	afx_msg void OnHtmllistaFix();
	afx_msg void OnHtmllistaPrintable();
	afx_msg void OnIndentedList();
	void CDescendantsLinearTable::fullTable();
	void CDescendantsLinearTable::uniqueTable();

};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////