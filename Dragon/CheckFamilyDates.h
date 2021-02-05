#pragma once
#include "listctrlex.h"
#include "colorstatic.h"
#include "ProgressWnd.h"

typedef struct
{
	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString death;
	int		numOfSpouses;
}HUSBAND;

typedef struct
{
	CString rowidH;

	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString death;
	CString marriage;
	int		motherOrder;
}WIFES;


typedef struct
{
	CString rowid;
	CString linenumber;
	CString name;
	CString birth;
	CString death;
	CString father_id;
	CString mother_id;
	int		mother_index;
	CString message;
}CHILDREN;




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CCheckFamilyDates : public CDialogEx
{
	DECLARE_DYNAMIC(CCheckFamilyDates)

public:
	CCheckFamilyDates(CWnd* pParent = NULL);   // standard constructor
	virtual ~CCheckFamilyDates();

// Dialog Data
	enum { IDD = IDD_CHECK_FAMILYDATES };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
	DECLARE_EASYSIZE

	_int64 oneyear;

	CString m_command;
	CString str;
	CString m_filespec;

	FILE * textF;
	CProgressWnd wndP;

	std::vector<HUSBAND> vHusband;
	std::vector<WIFES> vWifes;
	std::vector<CHILDREN> vChildren;
	
	HUSBAND h;
	WIFES w;
	CHILDREN c;

	bool collectChildren();
	void printVector();
	void printFamily();
	void checkMother();
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);

	CListCtrlEx m_ListCtrl;
	CColorStatic colorKeres;
	CColorStatic colorNext;
	afx_msg void OnClickedKeres();
	afx_msg void OnClickedNext();
};
