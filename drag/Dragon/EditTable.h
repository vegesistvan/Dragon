#pragma once
#include "afxwin.h"
#include "colorstatic.h"

// CEditTable dialog

class CEditTable : public CDialogEx
{
	DECLARE_DYNAMIC(CEditTable)

public:
	CEditTable(CWnd* pParent = NULL);   // standard constructor
	virtual ~CEditTable();

// Dialog Data
	enum { IDD = IDD_EDIT_TABLE };

	CString m_tableNumber;
	CString m_caption;
	CStringArray romanA;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CString str;
	CString m_command;

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset2;

	int		m_familyNumber;


	CString m_tableHeader;
	CString m_tableHeader2;
	CString m_type;
	CString m_familyName;
	CString m_alias;
	CString m_titolo;
	CString m_csalad;
	CString m_roman;
	CString m_comment;
	CString m_root;
	CString m_ag;
	CString m_joint;
	CListCtrlEx m_ListCtrl;

	void colorP();
	void createTableHeader();
	void fillBlobTable();
	void EditUpdate( bool all );
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CComboBox m_comboRoman;
	CComboBox m_comboType;
	CComboBox m_ComboJoint;
	
	afx_msg void OnKillfocusEditAuxname();
	afx_msg void OnKillfocusEditComment();
	afx_msg void OnKillfocusEditFamilyname();
	afx_msg void OnKillfocusEditRoot();
//	afx_msg void OnKillfocusEditOthername();
	afx_msg void OnBnClickedOk();

	afx_msg void OnKillfocusEditAlias();
	afx_msg void OnKillfocusEditAg();
	afx_msg void OnSelchangeComboRoman();
	afx_msg void OnSelchangeComboType();
	afx_msg void OnSelchangeComboJoint();
	afx_msg LRESULT OnListCtrlMenu(WPARAM wParam, LPARAM lParam);
	
	afx_msg void OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEditUpdate();
	afx_msg void OnEditInsert();
	afx_msg void OnEditDelete();
	afx_msg void OnClickedPhotos();
	CColorStatic colorPhotos;
};
