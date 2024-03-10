#pragma once

#include "ListCtrlEx.h"
#include "GEDCOM_typdef.h"
// CGedcomOut dialog
typedef struct
{
	int rowid;
	CString title;
	CString titolo;
	CString givn;
	CString surn;
	char	sex;
	CString birthDate;
	CString birthPlace;
	CString deathDate;
	CString deathPlace;
	CString comment;
	int		rowidF;
	int		rowidM;
	int		famc_id;		// a vFam indexe   melyik családban fordul elõ gyerekként
	CString file;
}INDI_S;

typedef struct
{
	int child_id;
	int	father_id;
	int mother_id;
}NEWFAMILIES;

typedef struct
{
	int		husb_id;		// a vIndi indexe
	int		wife_id;		// a vIndi indexe
	int		rowidH;
	int		rowidW;
	CString place;
	CString date;
}FAM_S;

typedef struct
{
	int		fam_id;			// a vFam indexe
	int		chil_id;		// a vIndi indexe
}CHIL_S;



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CGedcomOut : public CDialogEx
{
	DECLARE_DYNAMIC(CGedcomOut)

public:
	CGedcomOut(CWnd* pParent = NULL);   // standard constructor
	virtual ~CGedcomOut();

// Dialog Data
	enum { IDD = IDD_GEDCOM_OUT };

	CListCtrlEx* p_ListCtrl;
	CString m_filter;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	CString str;
	
	CString m_gedFolderPath;
	CString m_blobFolder;
	CString m_relFolder; // blobok relatív útvonala
	
	CString m_gedPathName;
	CString m_fileName;
	CString m_command;

	const static int nincs = 99999;
	
	std::vector<int> vRowid;

	std::vector<INDI_S> vINDI;
	std::vector<FAM_S> vFAM;
	std::vector<CHIL_S> vCHIL;
	std::vector<PHOTOS> vPhotos;
	
	void fillINDI( CListCtrlEx* p_ListCtrl );
	void fillFAM();
	void copyBlobs();
	void list();

	void printHeader();
	void printINDI();
	void printFAM();

	CString dateConv( CString input );

	FILE* fl;
	FILE* fg;
	DECLARE_MESSAGE_MAP()
public:

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedOk();
};
