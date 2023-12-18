#pragma once

#include "listctrlex.h"
#include "colorstatic.h"
#include "ProgressWnd.h"
#include "ListCtrlColoredHeader.h"

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
	DECLARE_MESSAGE_MAP()
public:
	CTableMarriages(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTableMarriages();
	enum { IDD = IDD_TABLE_MARRIAGES };
	CString	m_rowid;
	int	m_what;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CTableMarriages::push( CString str );
	void CTableMarriages::updateEntry( CString whichHusband, CString whichWife, CString place, CString date );
	void CTableMarriages::updateField( int nItem, int i, CString str );
	void CTableMarriages::listMarriages();

	CHARFORMAT cf = { 0 };

	std::vector<TCHAR*> v_tableMarriages; 
	CString m_command;
	CString str;
	int		m_orderix;
	int		m_columnsCount;


	CString m_filter;
	CString m_filterText;
//	CWnd* m_pParent;

	void	enableMenu( BOOL flag );
	virtual BOOL PreTranslateMessage(MSG* pMsg);


	std::vector<SPOUSE11>	vSpouse1;
	std::vector<SPOUSE21>	vSpouse2;
	std::vector<byte>		vColor;

	CSqliteDBRecordSet*	 m_rsetM;
	CSqliteDBRecordSet*	 m_rsetP;

	void keress( int start );

	BOOL queryM( CString command );
	BOOL queryP( CString command );

	CString fileSpec;

	FILE* fh1;

	CString rowid;

	CString p_fields;
	int		nItem;
	int m_cnt;
	CString m_last_name_h;
	CString m_last_name_w;
	CColorStatic colorKeres;
	CColorStatic colorNext;
	int m_numOfSpouses;

	
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);
	afx_msg void OnExportAll();
	afx_msg void OnExportSelected();
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedKeress();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnClickedNext();


	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditDelete();

	afx_msg void OnRelatives();
	afx_msg void OnHtmlHusband();
	afx_msg void OnHtmlWifes();
	afx_msg void OnNotepadHusband();
	afx_msg void OnNotpadWife();
	afx_msg void OnCompareHusbands();
	afx_msg void OnCompareWifes();

	afx_msg void OnAllMarriages();
	afx_msg void On1wife();
	afx_msg void On2wifes();
	afx_msg void On3wifes();
	afx_msg void On4wifes();
	afx_msg void On5wifes();
	afx_msg void On6wifes();
	afx_msg void On7wifes();
	afx_msg void On8wifes();
	afx_msg void On9wifes();
	afx_msg void On10wifes();
	afx_msg void OnRowidMarriages();
	afx_msg void OnMarriagesMan();
	afx_msg void OnMarriagesAll();
	afx_msg void OnMarriagesWoman();
	afx_msg void OnMarriagesDescendants();
	afx_msg void OnMarriagesParents();
	afx_msg void OnMarriagesSpouses();
	CListCtrlColoredHeader m_ListCtrl;
	CRichEditCtrl m_RichEditCtrl;
	afx_msg void OnInfo();
	afx_msg void OnMarrigeGedcom();
};
