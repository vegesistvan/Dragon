#pragma once
#include "afxdialogex.h"
#include "ColorStatic.h"

// CBrackets dialog

enum
{
	ROUND = 0,
	SQUARE,
	BRACES,
};

typedef struct
{
	int error;
	int linenumber;
	CString line;
}BRACKETS;
enum
{
	ERRORX = 0,
	LINENUMBER,
	LINE,
	RECORDSIZE,
};


class CBrackets : public CDialogEx
{
	DECLARE_DYNAMIC(CBrackets)

public:
	CBrackets(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CBrackets();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BRACKETS };
#endif

	int		m_bracketCode;
	CString m_htmlPathName;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	TCHAR m_bracket1;
	TCHAR m_bracket2;
	
	CString str;
	CString m_caption;
	CString m_caption2;
	CString m_brackets;
	CString m_Brackets;
	int m_fileLength;
	int m_table;
	int m_orderix;
	CStdioFile file;

	CColorStatic colorKeres;
	// columns
	int m_percent;
	int m_error;
	int m_linenumber;
	CString m_line;
	CListCtrlEx m_ListCtrl;
	std::vector<TCHAR*> vBrackets;
	std::vector<TCHAR*> vB;

	void CBrackets::push(BRACKETS br);
	void CBrackets::pushStr(CString str);
	void CBrackets::fillVector();
	CString CBrackets::cleanHtmlTags(CString cLine);
	bool CBrackets::findBrackets(CString cLine);
	void CBrackets::fillTable();
	void CBrackets::keress(int start);
	DECLARE_MESSAGE_MAP()
	
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDblclkList(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUnfiltered();
	afx_msg void OnWrongBrackets();
	afx_msg void OnBracketsFamily();
	afx_msg void OnBracketsTable();
	afx_msg void OnBracketsGood();
	afx_msg void OnBracketsTorzs();
	afx_msg void OnBracketsCsalad();
	afx_msg void OnBracketsAg();
	afx_msg void OnBracketsDesc();
	afx_msg void OnClickedStaticKeres();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
};
