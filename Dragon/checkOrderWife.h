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
A list�n azok a f�rjek tal�lhat�ak, akiknek t�bb h�zass�guk volt, �s ezek k�z�tt vannak olyanok, amelyek \r\
az adatb�zisban azonos sorsz�m� h�zass�gk�nt vannak nyilv�ntartva. A f�rjek ut�n ezek a h�zass�gok vannak felsorolva.\r\n\
\r\n\
Az oszlopok jent�se:\r\n\
\r\n\
cnt\t\ta f�rj sorsz�ma\r\n\
s\t\ta f�rj/feles�g el�fordul�s�nak rangja\r\n\
u\t\tegyes�tett bejegyz�sek sz�ma\r\n\
#h�zast�rs\th�zass�gainak sz�ma\r\n\
sorrend\t\ta h�zass�g sorsz�ma\r\n\
rowid\t\ta f�rj/feles�g azonos�t�ja\r\n\
name\t\ta f�rj/feles�g neve\r\n\
birth\t\ttsz�let�si d�tum\r\n\
death\t\telhal�loz�si d�tum\r\n\
\r\n\
Az azonos sorsz�m� feles�gek el�fordul�s�nak t�bb oka lehet:\r\n\
1. Val�j�ban a 2 feles�g egy �s ugyanaz, de valamilyen azonos�t�si adatuk k�l�nb�z�s�ge miatt nem egyes�tette \r\
�ket a program\r\n\
2. A feles�gek val�ban k�l�nb�z�ek, de a bemeneti htm f�jlban a feles�g valamely el�fordul�s�ban hi�nyos vagy rossz adat miatt a sorsz�m hib�s.\r\n\
\r\n\
Az adatok vizsg�lat�ra, a hiba ok�nak meg�llap�t�s�ra �s jav�t�s�ra a jobb eg�rgomb kattint�s�val leg�rd�l� \r\
men�b�l v�laszhatunk eszk�z�ket.\r\n\
A bal eg�rgombbal dupl�n kattintva egy emberre, megjelenik aannak teljes rokons�g�t tartalmaz� ablak.\r\
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
