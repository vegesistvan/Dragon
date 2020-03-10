#pragma once
#include "colorstatic.h"
#include "afxwin.h"
#include "afxdtctl.h"
#include "listctrlex.h"

// CRelations dialog

enum
{
	PP = 0,
	FM,
	M,
	S,
	C
};


class CRelations : public CDialogEx
{
	DECLARE_DYNAMIC(CRelations)

public:
	CRelations(CWnd* pParent = NULL);   // standard constructor
	virtual ~CRelations();
	
	CWnd* m_pParent;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset2;
// Dialog Data
	enum { IDD = IDD_RELATIONS };
	CString m_command;
	CString m_rowid;


	int nItem;
//	CListCtrlEx* m_LCtrl;
	
	
	int		m_order1;
	CString m_order;
	int		m_title_id;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	bool	m_changed;
	bool	m_paint;
	CString	m_fileSpec;
	CRect	rectW;

	CColorStatic colorSiblings;
	CColorStatic colorMarriages;
	CColorStatic colorNewFather;
	CColorStatic colorNewMother;

	int		listCtrlFlag;

	CString p_rowid;
	CString m_name;
	
	CString	str;
	CString m_father_id;
	CString m_mother_id;

	CString m_father;
	CString m_mother;

	int		m_sex_id;
	int		m_menuValid;

	int		m_type_id;
	CString	m_start;
	CString	m_end;
	

	void	createScreen( CString rowid );
	void	displayPicture( CString rowid );
	bool	writeBlockToFile( void* block, int blob_size  );

	BOOL	query( CString command );
	BOOL	query2( CString command );

	void	people( CString rowid );
	void	szulok( CString father_id, CString mother_id );
	void	testverek( CString rowid, CString father_id, CString mother_id );
	void	hazastarsak( CString rowid, int sex_id );
	void	gyerekek( CString rowid, int sex_id );

	bool	newSpouse( int db );
//	bool	newFather();
//	bool	newMother();
	void	insertMarriage();
	void	savePeople();

	void	editMarriage( int nItem );

	int		getOrderMother( CString rowid );
	int		getOrderFather( CString rowid );

	CListCtrlEx m_ListCtrlP;
	CListCtrlEx m_ListCtrlM;
	CListCtrlEx m_ListCtrlS;
	CListCtrlEx m_ListCtrlC;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();


protected:

	CString m_rowidFirst;

	CString m_tableNumber;
	CString m_title;
	CString m_titolo;
	CString m_last_name;
	CString m_first_name;
	CString m_birth_date;
	CString m_birth_place;
	CString m_death_date;
	CString m_death_place;
	CString m_comment;
//	CString m_occupation;

public:
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEditDelete();
	afx_msg void OnEditUpdate();
	afx_msg void OnEditGyerek();

	afx_msg void OnDblclkListP(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListM(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListC(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDblclkListS(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult);
	
//	afx_msg void OnRdblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkListP(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkListM(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkListC(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnRdblclkListS(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnReset();
//	afx_msg void OnPhoto();
	
	afx_msg void OnClickedMarriages();
	afx_msg void OnClickedNewFather();
	afx_msg void OnClickedNewMother();
	afx_msg void OnClickedSiblings();
//	afx_msg void OnClickedChildren();
	afx_msg void OnPaint();
	afx_msg void OnHelp();

	afx_msg void OnPictures();
//	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	CComboBox comboSex;
	CComboBox comboTitle;
	afx_msg void OnBnClickedOk();

	
	CComboBox comboBirth;
	CComboBox comboDeath;
	afx_msg void OnDblclkTabla();
	CColorStatic colorTable;
	afx_msg void OnClickedStaticComment();
	CString m_occupation;
	CColorStatic colorComment;
	afx_msg void OnThemechangedStaticTable(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnSelchangeComboSex();
	afx_msg void OnSelchangeComboTitle();
	afx_msg void OnChangeTitolo();
	afx_msg void OnChangeLastName();
	afx_msg void OnChangeFirstName();
	afx_msg void OnChangeBirthPlace();
	afx_msg void OnChangeBirthDate();
	afx_msg void OnSelchangeComboBirth();
	afx_msg void OnChangeEditOccupation();
	afx_msg void OnChangeDeathPlace();
	afx_msg void OnChangeDeathDate();
	afx_msg void OnSelchangeComboDeath();
	afx_msg void OnChangeComment();
	afx_msg void OnClickedChildren();
	CColorStatic colorChildren;
	CColorStatic colorName;
};
