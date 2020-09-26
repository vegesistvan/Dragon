#pragma once
#include "listctrlex.h"


// CSameCouples dialog

class CSameCouples : public CDialogEx
{
	DECLARE_DYNAMIC(CSameCouples)
	DECLARE_EASYSIZE


public:
	CSameCouples(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSameCouples();

// Dialog Data
	enum { IDD = IDD_SAME_COUPLES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CProgressWnd wndP; 

	CString _birth;
	CString _death;
	CString _father;
	CString _mother;
	CString _spouses;

	CString str;
	CString m_command;
	CString m_explanation;
	CString	_contractions;
	CString fileSpec1;
	CString fileSpec2;

	FILE* fh1;
	FILE* fh2;
	int nItem;
	int	m_cnt;
	int m_deleted;

	CString p_fields;
	CString _tag;

	CString m_rowid;
	CString m_sex_id;

	UINT	m_numOfGroups;
	CString rowidM;
	UINT	group;
	CString rowidS1;
	CString generationS1;
	int		statusS1;
	CString sourceS1;
	CString unitedS1;
	CString lineS1;
	CString spouse1;
	CString birthS1;
	CString deathS1;
	CString motherS1;

	CString rowidS2;
	CString generationS2;
	int		statusS2;
	CString sourceS2;
	CString unitedS2;
	CString lineS2;
	CString spouse2;
	CString birthS2;
	CString deathS2;
	CString motherS2;




	CString rowidM_0;
	CString sex_idS1_0;
	CString rowidS1_0;
	CString generationS1_0;
	int		sourceS1_0;
	CString unitedS1_0;
	CString lineS1_0;
	CString spouse1_0;
	CString birthS1_0;
	CString deathS1_0;
	CString motherS1_0;

	CString rowidS2_0;
	CString sex_idS2_0;
	CString generationS2_0;
	int		sourceS2_0;
	CString unitedS2_0;
	CString lineS2_0;
	CString spouse2_0;
	CString birthS2_0;
	CString deathS2_0;
	CString motherS2_0;

	CString rowidM_I;
	CString sex_idS1_I;
	CString rowidS1_I;
	CString generationS1_I;
	int		sourceS1_I;
	CString unitedS1_I;
	CString lineS1_I;
	CString spouse1_I;
	CString birthS1_I;
	CString deathS1_I;
	CString motherS1_I;

	CString rowidS2_I;
	CString sex_idS2_I;
	CString generationS2_I;
	int		sourceS2_I;
	CString unitedS2_I;
	CString lineS2_I;
	CString spouse2_I;
	CString birthS2_I;
	CString deathS2_I;
	CString motherS2_I;

	CString rowidM_1;
	CString sex_idS1_1;
	CString rowidS1_1;
	CString generationS1_1;
	int		sourceS1_1;
	CString unitedS1_1;
	CString lineS1_1;
	CString spouse1_1;
	CString birthS1_1;
	CString deathS1_1;
	CString motherS1_1;

	CString rowidS2_1;
	CString sex_idS2_1;
	CString generationS2_1;
	int		sourceS2_1;
	CString unitedS2_1;
	CString lineS2_1;
	CString spouse2_1;
	CString birthS2_1;
	CString deathS2_1;
	CString motherS2_1;

	CString rowidM_2;
	CString sex_idS1_2;
	CString rowidS1_2;
	CString generationS1_2;
	int		sourceS1_2;
	CString unitedS1_2;
	CString lineS1_2;
	CString spouse1_2;
	CString birthS1_2;
	CString deathS1_2;
	CString motherS1_2;

	CString rowidS2_2;
	CString sex_idS2_2;
	CString generationS2_2;
	int		sourceS2_2;
	CString unitedS2_2;
	CString lineS2_2;
	CString spouse2_2;
	CString birthS2_2;
	CString deathS2_2;
	CString motherS2_2;



	CString m_rowidMother;


	CListCtrlEx m_ListCtrl;


	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	std::vector<COUPLES> vCouples;
	std::vector<COUPLES> vSame;
	std::vector<COUPLES> vWife;

	void createColumns();
	void sameSpouses();
	void getSameCouples();
	void listCtrlLine( UINT i );
	void data0();
	void dataI( UINT i );
	void setData( UINT i1, UINT i2 );

	void processGroup();
	void processSame();
	void uniteGroup();
	void clearStatus2();
	int	 setStatus2();
	void listGroup();
	void listContracted();
	void htmlHeader1();
	void htmlHeader2();
	UINT getNumOfGroups();
	void contract();
	void printDiffI( UINT i, int nItem  );
	void printYellow( UINT i, int nItem );
	void printBackground( int nItem );

	bool identical( UINT ix1, UINT ix2 );


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );

	DECLARE_MESSAGE_MAP()

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnHtml();
	afx_msg void OnLineHusband();
	afx_msg void OnLineWife();
	afx_msg void OnInfo();
};
