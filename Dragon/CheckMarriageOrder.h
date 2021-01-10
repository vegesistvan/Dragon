#pragma once
#include "listctrlex.h"
#include "ProgressWnd.h"
#include "colorstatic.h"


typedef struct 
{
	CString linenumber;
	CString sex_id;
	CString source;

	CString rowid;
	CString name;
	CString birth;
	CString death;

	CString rowidF;
	CString father;
	CString birthF;
	CString deathF;

	CString rowidM;
	CString mother;
	CString birthM;
	CString deathM;
}SPOUSE1;

typedef struct 
{
	CString rowidS;
	CString place;
	CString date;
	CString order;

	CString linenumber;
	CString sex_id;
	CString source;

	CString rowid;
	CString name;
	CString birth;
	CString death;

	CString rowidF;
	CString father;
	CString birthF;
	CString deathF;

	CString rowidM;
	CString mother;
	CString birthM;
	CString deathM;
}SPOUSE2;




class CCheckMarriageOrder : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckMarriageOrder)
	DECLARE_EASYSIZE

public:
	CCheckMarriageOrder(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckMarriageOrder();

// Dialog Data
	enum { IDD = IDD_CHECK_MARRIAGEORDER };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


	std::vector<SPOUSE1>	vSpouse1;
	std::vector<SPOUSE2>	vSpouse2;
	std::vector<byte>		vColor;

	std::vector<MORESPOUSES> vSpouses;
	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	void marriages();
	void push( CString str );
	void htmlHeader( CString title );
	void collectHusband();
	void collectWife();
	void fillSpouse1();
	void fillSpouse2();
	void listHtml();
	void fillTable();
	void emptyRow();

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	CProgressWnd wndP;

	CString fileSpec;

	FILE* fh1;

	std::vector<TCHAR*> v_tableMarriages; 
	CString m_command;
	CString str;
	CString rowid;
	int		m_orderix;

/*
	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;

	CString _info;
*/
	CString m_explanation;
	CString p_fields;
	int		m_columnsCount;
	int		nItem;
	int m_cnt;
	CString m_last_name_h;
	CString m_last_name_w;

	BOOL PreTranslateMessage(MSG* pMsg);
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrlEx m_ListCtrl;
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnKillfocusSearch();
	afx_msg void OnClickedKeres();
	CColorStatic colorKeres;
};
