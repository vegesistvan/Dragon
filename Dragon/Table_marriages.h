#pragma once

#include "listctrlex.h"
#include "colorstatic.h"
#include "ProgressWnd.h"
// CTableMarriages dialog

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
}SPOUSE11;

typedef struct 
{
	CString rowidS;
	CString place;
	CString date;
	CString order;
	CString sourceM;

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
}SPOUSE21;



class CTableMarriages : public CDialogEx
{
	DECLARE_DYNAMIC(CTableMarriages)
	DECLARE_EASYSIZE
public:
	CTableMarriages(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableMarriages();
	enum { IDD = IDD_TABLE_MARRIAGES };

	CString	m_rowid;
	int		m_nItem;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CTableMarriages::fillTable( UINT nItem  );
	void CTableMarriages::push( CString str );
	void CTableMarriages::updateEntry( CString orderHusband, CString orderWife, CString place, CString date );
	void CTableMarriages::updateField( int nItem, int i, CString str );

	std::vector<TCHAR*> v_tableMarriages; 
	CString m_command;
	CString str;
	int		m_orderix;
	int		m_columnsCount;

	CString m_filter;
	CString m_filterText;
	CListCtrlEx m_ListCtrl;
	CWnd* m_pParent;

	void	enableMenu( BOOL flag );
	void CTableMarriages::azonosMarriages( int list );
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	std::vector<SPOUSE11>	vSpouse1;
	std::vector<SPOUSE21>	vSpouse2;
	std::vector<byte>		vColor;

	std::vector<MORESPOUSES> vSpouses;
	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	
	void htmlHeader( CString title );

	void listHtml();
	void keress( int start );

	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	CProgressWnd wndP;

	CString fileSpec;

	FILE* fh1;

	CString rowid;

	CString m_explanation;
	CString p_fields;
	int		nItem;
	int m_cnt;
	CString m_last_name_h;
	CString m_last_name_w;
	CColorStatic colorKeres;
	CColorStatic colorNext;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnExportAll();
	afx_msg void OnExportSelected();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedKeress();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnUnfilter();
	afx_msg void OnFilterHlastname();
	afx_msg void OnFilterWlastname();
	afx_msg void OnClose();
	afx_msg void OnGroupbyPlace();
	afx_msg void OnMarriagesDescendants();
	afx_msg void OnMarriagesSpouses();
	afx_msg void OnMarriagesParents();
	afx_msg void OnRowidMarriages();
	afx_msg void OnNameMarriages();
	afx_msg void OnClickedNext();


	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditDelete();
/*
	afx_msg	void OnHtmlEditLines();
	afx_msg void OnHtmlNotepad();
	afx_msg void OnHtmlNotepadParents();
	afx_msg void OnHtmlFatherAndSiblings();
	afx_msg void OnDbEdit();

	afx_msg void OnHtmlChildren();
	afx_msg void On3generations();
*/
};
