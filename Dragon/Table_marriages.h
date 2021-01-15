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

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CTableMarriages::fillTable( UINT nItem  );
	void CTableMarriages::push( CString str );

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
	virtual void PostNcDestroy();

	void OnHtmlEdit();
	void OnHtmlNotepad();
	void OnHtmlShows();

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
	void collectHusband();
	void collectWife();
	void fillSpouse1();
	void fillSpouse2();
	void listHtml();
	void fillTable();
	void emptyRow();
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

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnExportAll();
	afx_msg void OnExportSelected();
	afx_msg void OnAzonosMarriagelist();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedKeres();


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
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnGahtmlLine();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditDelete();
	afx_msg void OnEditGahtml();
	afx_msg void OnMoreMarriages();
	CColorStatic colorNext;
	afx_msg void OnClickedNext();
};
