#pragma once
#include "afxdialogex.h"
#include "compareEntries.h"
#include "ColorStatic.h"

// CCheckOrderWife dialog

class CCheckOrderWife : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckOrderWife)

public:
	CCheckOrderWife(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCheckOrderWife();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CHECK_ORDERWIFE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	CString m_command;
	PEOPLESTRUCT p;
	int nItem;
	int m_cnt;

	int m_order;

	const CString m_info = L"\
A listán azok a férjek találhatóak, akiknek több házasságuk volt, és ezek között vannak olyanok, amelyek \r\
az adatbázisban azonos sorszámú házasságként vannak nyilvántartva. A férjek után ezek a házasságok vannak felsorolva.\r\n\
\r\n\
Az oszlopok jentése:\r\n\
\r\n\
cnt\t\ta férj sorszáma\r\n\
s\t\ta férj/feleség elõfordulásának rangja\r\n\
u\t\tegyesített bejegyzések száma\r\n\
#házastárs\tházasságainak száma\r\n\
sorrend\t\ta házasság sorszáma\r\n\
rowid\t\ta férj/feleség azonosítója\r\n\
name\t\ta férj/feleség neve\r\n\
birth\t\ttszületési dátum\r\n\
death\t\telhalálozási dátum\r\n\
\r\n\
Az azonos sorszámú feleségek elõfordulásának több oka lehet:\r\n\
1. Valójában a 2 feleség egy és ugyanaz, de valamilyen azonosítási adatuk különbözõsége miatt nem egyesítette \r\
õket a program\r\n\
2. A feleségek valóban különbözõek, de a bemeneti htm fájlban a feleség valamely elõfordulásában hiányos vagy rossz adat miatt a sorszám hibás.\r\n\
\r\n\
Az adatok vizsgálatára, a hiba okának megállapítására és javítására a jobb egérgomb kattintásával legördülõ \r\
menübõl válaszhatunk eszközöket.\r\n\
A bal egérgombbal duplán kattintva egy emberre, megjelenik aannak teljes rokonságát tartalmazó ablak.\r\
";


	void CCheckOrderWife::createColumns();
	bool CCheckOrderWife::fillTable();
	bool CCheckOrderWife::listCouple(int i1, CString whichWife);
	bool CCheckOrderWife::getPeople(CString rowid);
	void CCheckOrderWife::keress(int start);

	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnHtmlEditLines();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlNotepadParents();
	afx_msg void OnHtmlFatherAndSiblings();
	afx_msg void OnHtmlChildren();
	afx_msg void OnDbEdit();
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUniteData();

	BOOL PreTranslateMessage(MSG* pMsg);

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CListCtrlEx m_ListCtrl;
	afx_msg void OnClickedStaticNext();
	afx_msg void OnStaticKeress();
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
private:
	CColorStatic colorKeres;
	CColorStatic colorNext;
public:
	afx_msg void OnInfo();
};
