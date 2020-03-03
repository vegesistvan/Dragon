#pragma once
#include "colorstatic.h"
#include "afxwin.h"
#include "listctrlex.h"
//#include "picturectrl.h"


// CEditPeople dialog

class CEditPeople : public CDialogEx
{
	DECLARE_DYNAMIC(CEditPeople)

public:
	CEditPeople(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditPeople();

// Dialog Data
	enum { IDD = IDD_EDIT_PEOPLE };

	CString m_caption;
	CString	m_tableNumber;
	CString m_father_id;
	CString m_mother_id;
	int		m_mother_index;
	int		m_numberOfWifes;
	CString m_title;
	CString m_table;


	CString m_m;
	CString m_rowid;
	int		m_role;
	int		m_sex_id;
	int		m_titleX;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CColorStatic colorTable;

	CRect rectW;	// a dlg ablak adatai  ( 0,0 pont a menü alatt!!

	bool	m_paint;
	int		m_orderix;

	CString	m_fileSpec;
	CString str;
	CString m_command;

	int		m_orderH;
	int		m_orderW;

	CString m_orderPeople;

	CComboBox m_ComboSex;
	CComboBox m_ComboTitle;
	CComboBox m_ComboRole;

	CString m_titolo;
	CString m_last_name;
	CString m_first_name;
	CString m_comment;
	CString m_occupation;
	CString m_csalad;
	CString m_death_date;
	CString m_death_place;
	CString m_birth_date;
	CString m_generation;
	CString m_folyt;
//	CString m_sources;
	CString m_nameChanged;
	
	CString m_name;

	CString m_husband;
	CString m_wife;

	CString m_birth_place;
	CString m_mother;
	CString m_father;
	CString m_csalad_ose;
//	CColorStatic colorMarriages;
	CColorStatic colorFather;
	CColorStatic colorMother;

	CListCtrlEx m_ListCtrl;

	void CEditPeople::displayPicture();
	void CEditPeople::EditUpdate( bool all );
	void CEditPeople::initialScreen();
	void CEditPeople::fillMarriageTable();
	void CEditPeople::fillBlobTable();
	int CEditPeople::getTitleX( CString title );
	CString CEditPeople::getTableName( CString tableNumber );
	CString CEditPeople::getParent( CString rowid );
	void CEditPeople::displayParent( int IDC, CString rowid );
	
	CString CEditPeople::getPeopleString( CString rowid );
	CString CEditPeople::getPlaceDateBlock( CString place, CString date, TCHAR jel );
	CString CEditPeople::getCommentBlock( CString comment, CString occupation  );
	void CEditPeople::updateM( int nItem );

	bool CEditPeople::writeBlockToFile( void* block, int blob_size  );
	CColorStatic colorDeleteFather;
	CColorStatic colorDeleteMother;

	BOOL m_tableAncestry;
	CColorStatic colorPhotos;	
	CListCtrlEx m_ListCtrlB;

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);

	afx_msg void OnClickedStaticTable();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedStaticFather();
	afx_msg void OnClickedStaticMother();
	afx_msg void OnKillfocusEditFirstname();
	afx_msg void OnKillfocusEditBirthd();
	afx_msg void OnKillfocusEditDeathd();
	afx_msg void OnClickedDeleteFather();
	afx_msg void OnClickedDeleteMother();
	afx_msg void OnKillfocusComboSex();
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnKillfocusLastName();
	afx_msg void OnClickedTableAncestry();
	
	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();

	afx_msg void OnEditUpdateM();
	afx_msg void OnEditInsertM();
	afx_msg void OnEditDeleteM();

	afx_msg void OnDblclkBlobs(NMHDR *pNMHDR, LRESULT *pResult);
	
	afx_msg void OnPaint();
	afx_msg void OnClickedPhotos();
	afx_msg LRESULT OnSetColumnColor(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnColumnSorted(WPARAM wParam, LPARAM lParam);

};
