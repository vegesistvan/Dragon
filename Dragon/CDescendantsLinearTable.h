#pragma once
#include "afxdialogex.h"
#include "CDescendants.h"
#include "CDescendantsFull.h"
#include "CDescendantsUnique.h"

enum
{
	L_CLRTEXTBK = 0,		// hidden
	L_CLRTEXT,
	L_LINENUMBER,
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
	CString m_lastnamePrev;
	CString m_lastname;
	int	m_cnt;
	CString genPrev;
	CString m_os;
	

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
	void CDescendantsLinearTable::dataTable();
	int CDescendantsLinearTable::sort_vDesc();
	BOOL CDescendantsLinearTable::query(CString command);
	BOOL CDescendantsLinearTable::queryP(CString command);
	BOOL CDescendantsLinearTable::queryM(CString command);
	bool CDescendantsLinearTable::printTopContainer(CString title );
	void CDescendantsLinearTable::printDesc(int i);
	TCHAR CDescendantsLinearTable::get_gABC(int g);
	CString CDescendantsLinearTable::getComplexDescription(int i, bool parentIndex);
	CString CDescendantsLinearTable::getColoredString(CString str, int index);
	CString CDescendantsLinearTable::getLastFirst(DE::PPEOPLE* p, bool style );
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


	bool CDescendantsLinearTable::createHtmlFile(int i, int which);
	void CDescendantsLinearTable::fullTable();
	void CDescendantsLinearTable::uniqueTable();
	BOOL CDescendantsLinearTable::attachDialogs();

	CString getSeededName(int i);
	DECLARE_MESSAGE_MAP()

	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClickedStaticNext();
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnHtmlListaFix();
	afx_msg void OnHtmlListaPrintable();
	afx_msg LRESULT CDescendantsLinearTable::OnReloadListCtrl(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT CDescendantsLinearTable::OnSetRowColor(WPARAM wParam, LPARAM lParam);//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
	afx_msg void OnIndentedList();



	CRichEditCtrl m_RichEditCtrl;
	CListCtrlEx* m_ListCtrl;


	COLORREF m_clrTextBk;
	COLORREF m_clrText;

	DE::PPEOPLE p;
	DE::PPEOPLE s;
	DE::PPEOPLE sf;
	DE::PPEOPLE sm;
	DE::PPEOPLE ss;

	CString m_title;
public:
	std::vector<DE::DESC>* vDesc;
	
	CString m_name;
	CString m_descendantsPath;
	CString m_htmlFile;
	CString m_indentedFile;

	bool p_connect;
	bool p_womenDescendants;
	bool p_folyt;
	bool p_capital;
	bool p_bold;
	bool p_checkCRLF;
	bool p_radioOne;
	int p_lastname;


	CString p_editWidth;
	CString p_editGenMax;
	CString p_editK;
	CString p_editG;
	
	BOOL p_firstBold;
	BOOL p_peerPrint;
	BOOL p_checkColor;
	int  p_type;
	BOOL p_listRepeated;
	

	int p_repeatedColor;
	int p_radioOrder;
	int p_radioDNameX;

	int p_comboDAttrib;
	int p_comboFontSize;
	int p_comboComment;
	int p_comboSpec;
	int p_comboOtherName;
	
	int p_colorBgrnd;
	int p_repeated;
	
	bool p_html;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////