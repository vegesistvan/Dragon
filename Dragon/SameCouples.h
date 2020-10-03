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

	// házaspár, akiket vizsgálunk;
	CString	_husband;
	CString _wife;

	CString str;
	CString m_command;
	CString m_explanation;
	CString	_contractions;
	CString unitedSpec;
	CString differentSpec;

	FILE* fU;
	FILE* fD;
	int nItem;
	int	m_cnt;
	int m_deleted;

	CString p_fields;
	CString _tag;

	CString m_rowid;
	CString m_sex_id;

	UINT	m_numOfGroups;
	int		m_contracted;
	CString m_description;

//////////////////////////////////////////////////////////	
	CString rowidM_1;

	UINT	group_1;
	int		statusS1_1;
	CString sex_idS1_1;
	CString rowidS1_1;
	CString generationS1_1;
	CString	sourceS1_1;
	CString unitedS1_1;
	CString lineS1_1;
	CString spouse1_1;
	CString birthS1_1;
	CString deathS1_1;
	CString motherS1_1;
	CString fatherS1_1;

	int		statusS2_1;
	CString rowidS2_1;
	CString sex_idS2_1;
	CString generationS2_1;
	CString	sourceS2_1;
	CString unitedS2_1;
	CString lineS2_1;
	CString spouse2_1;
	CString birthS2_1;
	CString deathS2_1;
	CString motherS2_1;
	CString fatherS2_1;

	UINT	group_2;
	CString rowidM_2;
	int		statusS1_2;
	CString sex_idS1_2;
	CString rowidS1_2;
	CString generationS1_2;
	CString	sourceS1_2;
	CString unitedS1_2;
	CString lineS1_2;
	CString spouse1_2;
	CString birthS1_2;
	CString deathS1_2;
	CString motherS1_2;
	CString fatherS1_2;

	int		statusS2_2;
	CString rowidS2_2;
	CString sex_idS2_2;
	CString generationS2_2;
	CString	sourceS2_2;
	CString unitedS2_2;
	CString lineS2_2;
	CString spouse2_2;
	CString birthS2_2;
	CString deathS2_2;
	CString motherS2_2;
	CString fatherS2_2;
////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CString birth1Ref;
	CString death1Ref;
	CString mother1Ref;
	CString birth2Ref;
	CString death2Ref;
	CString mother2Ref;

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
	std::vector<CString> vDeleted;

	void openUnited();
	void openDifferent();

	void createColumns();
	void sameSpouses();
	void getSameCouples();
	void listCtrlLine( UINT i );
	void setData( UINT i1, UINT i2 );


	void referenceValues();
	void processSame();
	void listDiff();
	void listUnited();

	UINT getNumOfGroups();
	void contract();
	void printYellow( UINT i, int nItem );
	void printBackground( int nItem );

	bool identical( UINT i1, UINT i2 );


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
	afx_msg void OnLineHusband();
	afx_msg void OnLineWife();

};
