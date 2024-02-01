#pragma once
#include "afxdialogex.h"
#include "ColorStatic.h"

#define WM_RELOAD			WM_USER + 0x1FF9

typedef struct
{
	int		cnt;		// az el�fordul�s sorsz�ma, cnt ==0 csak egyszer fordul el�
	int		cntMax;		// �sszesen h�nyszor fordiul el�
	int		original;	// eredeti sorrend indexe
	CString rowidC;		// gyere rowid-ja
	CString rowid;		// apa rowid-ja
	CString familyName;
	CString name;
	CString birth;
	CString death;
	int		sex_id;
	int		g;
	TCHAR	gABC;
	_int64	k;
	_int64	gcnt;		// gener�ci�n bel�li sorsz�m
	int		ixC;		// gyereke indexe	(<i) (ami a felmen�k list�j�ban �t gener�lta
	int		ixF;		// apja indexe		(>i) 	
	int		ixSp;		// p�rja indexe			( ami a k+1 Kekule sz�m� h�zast�rs
	int		ixM;		// anya indexe
	int		numOfSp;	// h�zasat�rsak sz�ma	
	int		toggle;
	int		moreC;
	int		x;			// piramis koordin�t�k 
	int		y;			//
	CString linenumber;
}GKR;

typedef struct
{
	int ixF;			// apa indexe
	int ixC;			// gyerek indexe	
	int cnt;
	int sex_id;
	CString rowidF;		// gyerek rowid
	CString rowidC;		// apa rowid az azonos ap�k megtal�l�s�hoz hell
}FC;

typedef struct
{
	int ix;				// felmen� indexe
	int ixSp;			// h�zast�rs indexe
}SP;

typedef struct
{
	int ix;				// felmen� indexe
	int	ixSib;			// testv�r indexe
	int ixF;			// apa indexe
	int ixM;			// anya indexe
	bool listed;
}SIBLINGS;

namespace CK
{
	typedef struct
	{
		int ixF;		// 	apa indexe
		int ixC;		// gyerek indexe	
		int cnt;
		int sex_id;
		CString rowidF;
	}FC;

	typedef struct
	{
		CString color;
	}COLORS;

	const COLORS colors[] =
	{
		L"blue",
		L"darkcian",
		L"mediumseagreen",
		L"darkorange",
		L"slateblue",
		L"purple",
		L"green",
		L"deeppink",
		L"forestgreen",
		L"olive",
		L"sienna",
		L"darkmagenta",
		L"darkseegreen",
		L"darkred",
		L"darkviolet",
		L"darkkhaki",
	};
};
class CAscendants : public CDialogEx
{
	DECLARE_DYNAMIC(CAscendants)

public:
	CAscendants(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CAscendants();
	CString m_rowid;
// Dialog Data
#ifdef AFX_DESIGN_TIME
enum { IDD = IDD_ASCENDANTS };
#endif

protected:
// vGKR-hez
	CString m_father_id;
	CString m_mother_id;
	CString m_name;
	CString m_familyName;
	CString m_birth;
	CString m_death;
	int		m_sex_id;
	CString m_linenumber;

// el��rhat� param�terek
	CString s_editK;
	CString s_editG;

	int m_numOfColors = sizeof(CK::colors) / sizeof(CK::COLORS);

	BOOL m_bold;
	BOOL m_firstBold;
	BOOL m_NN;
	BOOL m_peerPrint;
	BOOL m_checkColor;
	int  m_type;
	BOOL m_listRepeated;
//
	std::vector<GKR> vGKR;
	std::vector <FC> vFC;
	std::vector<SP> vSp;
	std::vector<SIBLINGS> vSib;

	bool m_term;
// vGKR-b�l meghat�rozott adatok

	CString s_numOfAF;
	CString s_numOfAM;
	CString s_numOfGF;
	CString s_numOfGM;
	CString s_numOfGDF;
	CString s_numOfGDM;

	int m_numOfAF;		// apai �gi felmen�k sz�ma
	int m_numOfAM;		// anyai �gi felmen�k sz�ma
	int m_numOfGF;		// apai gener�ci�k sz�ma
	int m_numOfGM;		// anyai gener�ci�k sz�ma
	int m_numOfGDF;		// egyenes �gi apai gener�ci�k sz�ma
	int m_numOfGDM;		// egyenes �gi anyai gener�ci�k sz�ma

	int m_numOfG;		// gener�ci�k sz�ma
	int m_numOfA;		// felmen�k sz�ma
	int m_maxK;
	CString m_numOfFG;
	CString m_numOfFA;

	_int64	m_editK;
	int		m_editG;
	int		m_gBias;			// a piramis eltol�sa miatt gener�ci� v�ltoz�s
	int     m_count;
	int		m_gPrev;
	bool	m_gFlag;
//

	CString m_title;

	CString m_rootName;
	CString m_filespec;
	CString m_command;
	CString str;

	CSqliteDBRecordSet* m_recP;
	CSqliteDBRecordSet* m_recM;

	HBRUSH m_brush;
	CString m_ulStyle;
	int		m_GPrev;
	int		m_cnt;
	int		m_cnt_ul;
	int		m_indent;	// beh�z�sok aktu�lis sz�ma( <ol>/<il>-ek sz�ma ) 
	bool	m_log;


	FILE* flvGKR;		// log file
	FILE* flvGKR2;		// log file
	FILE* flvGKR3;		// log file
	FILE* flvFC;
	FILE* flvSib;
	FILE* flvSp;
	FILE* fh;

	FILE* fhLinListRep;
	FILE* fhLinListNoRep;
	FILE* fhLinTabRep;
	FILE* fhLinTabNoRep;
	FILE* fhfixHeader;
	FILE* fhfixHeaderRep;
	FILE* fhfixHeaderNoRep;

	FILE* fhSticky;

	CString fs_vGKR;
	CString fs_vGKR2;
	CString fs_vGKR3;
	CString fs_vFC;
	CString fs_vSib;
	CString fs_vSp;

	struct
	{
		int		cnt;		// sorsz�m
		CString rep;		// ism�tl�sek sz�ma	
		CString idSib;		// testv�r azonos�t�ja;
		CString idF;		// apa azonos�t�ja
		CString idM;		// anya azonos�t�ja
		CString idA;		// felme� azonos�t�ja
		CString idSp;		// h�zast�rs azonos�t�ja
		CString idC;		// gyerek azonos�t�ja
		CString ascString;	// felmen�k
	}col;


	CString m_sep;			// gener�ci�kat elv�laszt� vonal

	CString m_piramidFile;
	bool m_show;
	CString m_linearListFileRep;
	CString m_linearListFileNoRep;
	CString m_linearTableFileNoRep;
	CString m_linearTableFileRep;
	CString m_stickyFile;
	CString m_fixHeaderFile;
	CString m_fixHeaderFileRep;
	CString m_fixHeaderFileNoRep;

	CString m_ascendantsPath;

	CColorStatic colorGroupInfoText;
	CColorStatic colorAscendantsText;
	CColorStatic colorGenerationsText;
	CColorStatic colorGroupListText;
	CColorStatic colorGroupKekuleText;
	CColorStatic colorGroupOtherText;
	CColorStatic colorLinearTitle;


	int		m_kMin;			// az el�zo gener�ci� els� kek�l� sz�ma
	int		m_kMax;			
	_int64	m_kMinAct;		// az aktu�lis gener�ci� k intewrvalluma
	_int64	m_kMaxAct;
	_int64  m_kLast;
	_int64  m_kLastW;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	void CAscendants::printvGKR();
	void CAscendants::printvGKR2();
	void CAscendants::printvFC();

	void CAscendants::create_vGKR();
	void CAscendants::create_vGKR2();
	void CAscendants::correctReferences();
	CString CAscendants::createAscendant(int i, int cnt, int flag );
	bool CAscendants::getParentsId( int i);
	bool CAscendants::getPeople(CString rowid);
	BOOL CAscendants::queryP(CString command);
	BOOL CAscendants::queryM(CString command);
	void CAscendants::colorFrame(CPaintDC* dc, int IDC, COLORREF color);
	void CAscendants::parameters();
	bool CAscendants::openHtml( CString filename, CString title);
	
	void CAscendants::listPeople(int i);

	bool CAscendants::linearList( int which );
	bool CAscendants::linearTable( int which);
	void CAscendants::dataList(int which);
	void CAscendants::dataTable(int which );
	void CAscendants::printTopContainer(CString title, int which);
	bool CAscendants::fixTableHeader();

	void CAscendants::listNN(_int64 k);
	void CAscendants::print0(CString str);
	void CAscendants::print1(CString str );
	void CAscendants::print2(CString str);
	void CAscendants::print3(CString str);
	void CAscendants::print4(CString str);
	void CAscendants::printS(CString str);
	void CAscendants::printF(CString str);
	void CAscendants::printOnly(CString str, int which);
	void CAscendants::printvSib();
	void CAscendants::printvSp();
	CString CAscendants::indent(int gen, int genPrev);
	CString CAscendants::getPeopleString( int i );
	CString CAscendants::getMarriage(CString rowid, CString rowidS);
	UINT CAscendants::getGfromK(_int64 k);
	void CAscendants::saveParameters();
	void CAscendants::getParameters();
	TCHAR CAscendants::get_gABC(int g);
	bool CAscendants::sticky();
	bool CAscendants::tableHeaderFix(int which);
	CString CAscendants::konvTitolo(CString titolo);
	
	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnClickedDirectFatherAscendants();
	afx_msg void OnClickedDirectMotherAscendants();
	afx_msg void OnClickedDirectCouplesF();
	afx_msg void OnClickedDirectCouplesM();
	afx_msg void OnClickedPyramid();
	afx_msg void OnKillfocusEditG();
	afx_msg void OnKillfocusEditK();
	afx_msg void OnClickedCheckFirstBold();
	afx_msg void OnClickedLinear();
	afx_msg void OnClickedLinearSqlitetable();
	afx_msg void OnBnClickedGroupOther();
	BOOL p_titololower;
	afx_msg void OnClickedTitololower();
};
