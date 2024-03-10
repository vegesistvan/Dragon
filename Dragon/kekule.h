#pragma once
#include "afxdialogex.h"
namespace CK
{
	typedef struct
	{
		int g;
		_int64 k;
		CString r;
		CString name;
		CString birth;
		CString death;
		int		repeated;
		int		root;
		int		cnt;
	}GKR;

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
class CKekule : public CDialogEx
{
	DECLARE_DYNAMIC(CKekule)

public:
	CKekule(CWnd* pParent = nullptr);   // standard constructor
	virtual ~CKekule();
	CString m_htmlFile;
	CString m_rowid;
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_KEKULE };
#endif

protected:
// vGKR-hez
	CString m_father_id;
	CString m_mother_id;
	CString m_name;


// el��rhat� param�terek
	CString s_editK;
	CString s_editG;

	int m_numOfColors = sizeof(CK::colors) / sizeof(CK::COLORS);
	int	m_repeated;			// 0: nem ism�telt felmen�, i: az ism�telt felmen� sorsz�m

	BOOL m_bold;
	BOOL m_NN;
	BOOL m_peerPrint;
	BOOL m_checkColor;

//
	std::vector<CK::GKR> vGKR;

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
//




	CString m_command;
	CString str;
	CSqliteDBRecordSet* m_recP;
	CSqliteDBRecordSet* m_recM;
	HBRUSH m_brush;
	CString m_ulStyle;
	int		m_GPrev;
	int		m_cnt;
	int		m_cnt_ul;

	FILE* fl;
	FILE* fh;
	CString m_fileSpec;
	CString fileSpec;


	CColorStatic colorGroupInfoText;
	CColorStatic colorAscendantsText;
	CColorStatic colorGenerationsText;
	CColorStatic colorGroupListText;
	CColorStatic colorGroupKekuleText;
	CColorStatic colorGroupOtherText;


	int		m_kMin;			// az el�zo gener�ci� els� kek�l� sz�ma
	int		m_kMax;			
	_int64	m_kMinAct;		// az aktu�lis gener�ci� k intewrvalluma
	_int64	m_kMaxAct;
	_int64  m_kLast;
	_int64  m_kLastW;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnCancel();

	void CKekule::fillVector();
	bool CKekule::getParentsId( _int64 k, int i );
	bool CKekule::getPeople(CString rowid);
	BOOL CKekule::queryP(CString command);
	BOOL CKekule::queryM(CString command);
	void CKekule::colorFrame(CPaintDC* dc, int IDC, COLORREF color);
	void CKekule::calcNotableNumbers();
	bool CKekule::openHtml(CString filename, CString title);
	void CKekule::listPeople(int i);
	void CKekule::listNN(_int64 k);
	void CKekule::listAscendants(int i);
	void CKekule::print(CString str );
	CString CKekule::indent(int gen, int genPrev);
	CString CKekule::getPeopleString( int i );
	CString CKekule::getMarriage(CString rowid, CString rowidS);
	UINT CKekule::getGfromK(_int64 k);
	int CKekule::getRepeated(CString rowid);

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

};
