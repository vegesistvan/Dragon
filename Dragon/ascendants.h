#pragma once
#include "afxdialogex.h"
#include "ColorStatic.h"

#define WM_RELOAD			WM_USER + 0x1FF9

typedef struct
{
	int		cnt;		// az elõfordulás sorszáma, cnt ==0 csak egyszer fordul elõ
	int		cntMax;		// összesen hányszor fordiul elõ
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
	_int64	gcnt;		// generáción belüli sorszám
	int		ixC;		// gyereke indexe	(<i) (ami a felmenõk listájában õt generálta
	int		ixF;		// apja indexe		(>i) 	
	int		ixSp;		// párja indexe			( ami a k+1 Kekule számú házastárs
	int		ixM;		// anya indexe
	int		numOfSp;	// házasatársak száma	
	int		toggle;
	int		moreC;
	int		x;			// piramis koordináták 
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
	CString rowidC;		// apa rowid az azonos apák megtalálásához hell
}FC;

typedef struct
{
	int ix;				// felmenõ indexe
	int ixSp;			// házastárs indexe
}SP;

typedef struct
{
	int ix;				// felmenõ indexe
	int	ixSib;			// testvér indexe
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

// elõírható paraméterek
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
// vGKR-bõl meghatározott adatok

	CString s_numOfAF;
	CString s_numOfAM;
	CString s_numOfGF;
	CString s_numOfGM;
	CString s_numOfGDF;
	CString s_numOfGDM;

	int m_numOfAF;		// apai ági felmenõk száma
	int m_numOfAM;		// anyai ági felmenõk száma
	int m_numOfGF;		// apai generációk száma
	int m_numOfGM;		// anyai generációk száma
	int m_numOfGDF;		// egyenes ági apai generációk száma
	int m_numOfGDM;		// egyenes ági anyai generációk száma

	int m_numOfG;		// generációk száma
	int m_numOfA;		// felmenõk száma
	int m_maxK;
	CString m_numOfFG;
	CString m_numOfFA;

	_int64	m_editK;
	int		m_editG;
	int		m_gBias;			// a piramis eltolása miatt generáció változás
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
	int		m_indent;	// behúzások aktuális száma( <ol>/<il>-ek száma ) 
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
		int		cnt;		// sorszám
		CString rep;		// ismétlések száma	
		CString idSib;		// testvér azonosítója;
		CString idF;		// apa azonosítója
		CString idM;		// anya azonosítója
		CString idA;		// felmeõ azonosítója
		CString idSp;		// házastárs azonosítója
		CString idC;		// gyerek azonosítója
		CString ascString;	// felmenõk
	}col;


	CString m_sep;			// generációkat elválasztü vonal

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


	int		m_kMin;			// az elõzo generáció elsõ kekülé száma
	int		m_kMax;			
	_int64	m_kMinAct;		// az aktuális generáció k intewrvalluma
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
