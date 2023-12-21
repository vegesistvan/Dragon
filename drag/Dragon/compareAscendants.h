#pragma once
#include "afxdialogex.h"
#include "ListCtrlColoredHeader.h"

namespace CA
{
	typedef struct
	{
		int g;
		int k;
		CString r;
		CString s;
		CString peer;
		CString title;
		CString titolo;
		CString name;
		CString birthPlace;
		CString birth;
		CString deathPlace;
		CString death;
		CString comment;
	}GKR;
};

class CCompareAsc : public CDialogEx
{
	DECLARE_DYNAMIC(CCompareAsc)

public:
	CCompareAsc(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CCompareAsc();
	CString rowid1;
	CString rowid2;
	CString name1;
	CString name2;

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMPARE_ASC };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	afx_msg void OnCustomdrawList(NMHDR* pNMHDR, LRESULT* pResult);
	void CCompareAsc::coloredCells( int i1, int i2 );

	CString m_command;
	CString str;
	CSqliteDBRecordSet* m_recP;
	FILE* fl;

	CA::GKR gkr;		// egy ember g-k-r-p adatai	

	std::vector<CA::GKR> vGKR1;
	std::vector<CA::GKR> vGKR2;

	std::vector<CA::GKR>* vP1;
	std::vector<CA::GKR>* vP2;

	std::vector<int> vColor;
	UINT m_color;

	CString m_filespecTxt;
	CString m_father_id;
	CString m_mother_id;
	
	int nItem;
	int m_genPrev;

	BOOL CCompareAsc::queryP(CString command);
	bool CCompareAsc::fillVector( CString rowid, std::vector<CA::GKR>* vGKR );
	bool CCompareAsc::getPeople(CString rowid);
	bool CCompareAsc::getParentsId(CString rowid);
	void CCompareAsc::listVectors();
	void CCompareAsc::listBoth( int i1, int i2 );
	void CCompareAsc::list1(int i );
	void CCompareAsc::list2(int i);
	void CCompareAsc::list_vP1( int i);
	void CCompareAsc::list_vP2( int i);
	DECLARE_MESSAGE_MAP()
public:
	

	virtual BOOL OnInitDialog();
private:
	CListCtrlColoredHeader m_ListCtrl;
};
