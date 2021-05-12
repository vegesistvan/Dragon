#pragma once
#include "afxwin.h"

#include "GA_ascendants_str.h"

// CGaAscendantsChain dialog

class CGaAscendantsChain : public CDialogEx
{
	DECLARE_DYNAMIC(CGaAscendantsChain)

public:
	CGaAscendantsChain(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGaAscendantsChain();

// Dialog Data
	enum { IDD = IDD_GA_ASCENDANTSCHAIN };

	CString m_name;
	CString m_rowid;
	CString m_sex_id;
	CString m_father_id;
	CString m_mother_id;
	CString m_spouse_id;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;
	int		m_KStart;
	int		m_GMax;
	int		m_kFirst;

	int		GMAX;	// egy felmenõi lista generációinak száma ( 5 )
//	int		KMAX;
	int		KNEXT;
	int		GNEXT;
	int		KBIAS;
	int		GBIAS;
	int		m_cnt_ul;

	FILE* fh1;

	CString m_htmlFile;

	CSqliteDBRecordSet m_recordset;
	CSqliteDBRecordSet m_recordset1;
	CSqliteDBRecordSet m_recordset2;
	CSqliteDBRecordSet m_recordset3;
	CSqliteDBRecordSet m_recordset4;

	std::vector<FELMENOK> vFel;
	std::vector<KEKULE> vKekule;
	std::vector<ASC> vA;

	CString openFile();
	BOOL getProbantus( CString m_rowid, int m_kekule );
	BOOL getPeople( CString rowid );
	void setVA();
	BOOL fillVA();
	void printVA( CString title );
	void printAscendants(int i );
	void print0();
	void print1();
	CString getPeopleString( int i );
	CString getName( CString rowid );
	CString getMarriage( CString rowid, CString rowidS );
	void fillVKekule();
	void fillVKekuleNew();
	void printVKekule( CString title);

	CString indent( int gen, int genPrev );


	BOOL query( CString command );
	BOOL query1( CString command );
	BOOL query2( CString command );
	BOOL query3( CString command );
	BOOL query4( CString command );
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	int m_numOfP;
//	CComboBox m_ComboKekule;
	afx_msg void OnBnClickedOk();
	BOOL m_list;
	BOOL m_NN;
	int m_kekule;
	BOOL m_bold;
	BOOL m_code;
};
