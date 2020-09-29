#pragma once
#include "listctrlex.h"
#include "ProgressWnd.h"

// CSame dialog

class CSame : public CDialogEx
{
	DECLARE_DYNAMIC(CSame)
	DECLARE_EASYSIZE

public:
	CSame(CWnd* pParent = NULL);   // standard constructor
	virtual ~CSame();

// Dialog Data
	enum { IDD = IDD_SAME };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CProgressWnd wndP; 

	CString p_fields;
	CString m_command;
	CString str;
	CString unitedSpec;
	CString differentSpec;
	CString m_explanation;
	FILE* fU;
	FILE* fD;

	int		m_numOfGroups;
	int		m_deleted;
	int		m_contracted;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;
	CSqliteDBRecordSet*	 m_recordset2;
	CSqliteDBRecordSet*	 m_recordset3;
	CSqliteDBRecordSet*	 m_recordset4;

	std::vector<SAMENAMES> vPeople;


	int		_group;
	int		_status;
	CString _gen;
	CString _united;
	CString _line;
	CString _rowid;
	CString _name;
	CString _birth;
	CString _death;
	CString _mother;
	int		_married;

	int		_group2;
	int		_status2;
	CString _gen2;
	CString _united2;
	CString _line2;
	CString _rowid2;
	CString _name2;
	CString _birth2;
	CString _death2;
	CString _mother2;
	int		_married2;


	

	void createColumns();
	void collectPeople();
	void putPeople( UINT i );
	void processPeople();
	void getData( UINT i );
	void getData2( UINT i );
	void listPeople();
	UINT getNumOfGroups();
	int  identical( UINT i1, UINT i2 );
	void contract( UINT i1, UINT i2 );
	int  birth();
	int  death();
	int	 mother();

	void openUnited();
	void openDifferent();

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
	CListCtrlEx m_ListCtrl;
};
