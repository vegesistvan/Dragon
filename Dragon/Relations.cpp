// EditPeople.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "utilities.h"
#include "Relations.h"
#include "afxdialogex.h"
#include "SelectParent.h"
#include "Date.h"
#include "SelectPeople.h"
#include "atlimage.h"
#include "GA_input.h"
#include "Table_tables.h"
#include "setup.h"
#include "html_Lines.h"
#include "pictures.h"
#include "NewPeople.h"
#include "EditPeople.h"
#include "EditMarriage.h"
#include "Help.h"
#include "EditComment.h"
#include "MoreSpouses.h"


#define BISEX 0
#define MAN   1
#define WOMAN 2

#define FATHER 0
#define MOTHER 1

enum
{
	LIST_LINENUMBER = 0,
	LIST_SEX,
	LIST_ROWID,
	LIST_NAME,
	LIST_BIRTH,
	LIST_DEATH,
	LIST_COMMENT
};

// szülõk
enum
{
	LISTP_LINENUMBER = 0,
	LISTP_SEX,
	LISTP_ROWID,
	LISTP_NAME,
	LISTP_BIRTH,
	LISTP_DEATH,
	LISTP_COMMENT
};



// testvérek
enum
{
	LISTS_LINENUMBER = 0,
	LISTS_NUMBER,
	LISTS_ROWID,
	LISTS_NAME,
	LISTS_BIRTH,
	LISTS_DEATH,
	LISTS_COMMENT
};


// házastársak
enum
{
	LISTM_LINENUMBER = 0,
	LISTM_ROWIDM,				// házasság azonosító
	LISTM_ORDER,
	LISTM_ROWID,				// házastárs azonosító
	LISTM_NAME,					
	LISTM_BIRTH,
	LISTM_DEATH,
	LISTM_COMMENT,
	LISTM_MARRIAGE
};


// gyereke
enum
{
	LISTC_LINENUMBER = 0,
	LISTC_NUMBER,
	LISTC_ROWID,
	LISTC_ORDERFATHER,
	LISTC_ORDERMOTHER,
	LISTC_MOTHER_INDEX,
	LISTC_NAME,
	LISTC_BIRTH,
	LISTC_DEATH,
	LISTC_COMMENT
};
// CRelations dialog

IMPLEMENT_DYNAMIC(CRelations, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRelations::CRelations(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRelations::IDD, pParent)
	, m_titolo(_T(""))
	, m_last_name(_T(""))
	, m_first_name(_T(""))
	, m_birth_date(_T(""))
	, m_birth_place(_T(""))
	, m_death_date(_T(""))
	, m_death_place(_T(""))
	, m_comment(_T(""))
	, m_occupation(_T(""))
	, m_posterior(_T(""))
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;

	m_pParent		= pParent;
	m_paint			= false;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CRelations::~CRelations()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTM, m_ListCtrlM);
	DDX_Control(pDX, IDC_LISTS, m_ListCtrlS);
	DDX_Control(pDX, IDC_LISTC,	m_ListCtrlC);
	DDX_Control(pDX, IDC_LISTP,	m_ListCtrlP);

	DDX_Control(pDX, IDC_SIBLINGS, colorSiblings);
	DDX_Control(pDX, IDC_MARRIAGES, colorMarriages);
	DDX_Control(pDX, IDC_NEW_FATHER, colorNewFather);
	DDX_Control(pDX, IDC_NEW_MOTHER, colorNewMother);
	DDX_Control(pDX, IDC_COMBO_SEX, comboSex);
	DDX_Control(pDX, IDC_COMBO_TITLE, comboTitle);
	DDX_Text(pDX, IDC_TITOLO, m_titolo);
	DDX_Text(pDX, IDC_FIRST_NAME, m_first_name);
	DDX_Text(pDX, IDC_DEATH_DATE, m_death_date);
	DDX_Text(pDX, IDC_COMMENT, m_comment);
	DDX_Text(pDX, IDC_BIRTH_DATE, m_birth_date);
	DDX_Text(pDX, IDC_BIRTH_PLACE, m_birth_place);
	DDX_Text(pDX, IDC_LAST_NAME, m_last_name);
	DDX_Text(pDX, IDC_DEATH_PLACE, m_death_place);
	DDX_Control(pDX, IDC_COMBO_BIRTH, comboBirth);
	DDX_Control(pDX, IDC_COMBO_DEATH, comboDeath);
	DDX_Control(pDX, IDC_TABLE, colorTable);
	DDX_Text(pDX, IDC_EDIT_OCCUPATION, m_occupation);
	DDX_Control(pDX, IDC_STATIC_COMMENT, colorComment);
	DDX_Control(pDX, IDC_CHILDREN, colorChildren);
	DDX_Control(pDX, IDC_NAME, colorName);
	DDX_Text(pDX, IDC_EDIT_POSTERIOR, m_posterior);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CRelations, CDialogEx)
ON_COMMAND(ID_RESET, &CRelations::OnReset)
	
ON_NOTIFY(NM_DBLCLK, IDC_LISTP, &CRelations::OnDblclkListP)
ON_NOTIFY(NM_DBLCLK, IDC_LISTM, &CRelations::OnDblclkListM)
ON_NOTIFY(NM_DBLCLK, IDC_LISTC, &CRelations::OnDblclkListC)
ON_NOTIFY(NM_DBLCLK, IDC_LISTS, &CRelations::OnDblclkListS)

ON_NOTIFY(NM_RDBLCLK, IDC_LISTP, &CRelations::OnRdblclkListP)
ON_NOTIFY(NM_RDBLCLK, IDC_LISTM, &CRelations::OnRdblclkListM)
ON_NOTIFY(NM_RDBLCLK, IDC_LISTC, &CRelations::OnRdblclkListC)
ON_NOTIFY(NM_RDBLCLK, IDC_LISTS, &CRelations::OnRdblclkListS)

//ON_COMMAND(ID_PHOTO, &CRelations::OnPhoto)
ON_STN_CLICKED(IDC_MARRIAGES, &CRelations::OnClickedMarriages)
ON_STN_CLICKED(IDC_NEW_FATHER, &CRelations::OnClickedNewFather)
ON_STN_CLICKED(IDC_NEW_MOTHER, &CRelations::OnClickedNewMother)
ON_STN_CLICKED(IDC_SIBLINGS, &CRelations::OnClickedSiblings)
ON_STN_CLICKED(IDC_CHILDREN, &CRelations::OnClickedChildren)


ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
ON_BN_CLICKED(ID_EDIT_DELETE, &CRelations::OnEditDelete )
ON_BN_CLICKED(ID_EDIT_UPDATE, &CRelations::OnEditUpdate )
ON_BN_CLICKED(ID_EDIT_GYEREK, &CRelations::OnEditGyerek )

ON_WM_PAINT()
ON_COMMAND(ID_HELP, &CRelations::OnHelp)
ON_COMMAND(ID_PICTURES, &CRelations::OnPictures)
ON_BN_CLICKED(IDOK, &CRelations::OnBnClickedOk)
ON_STN_DBLCLK(IDC_TABLE, &CRelations::OnDblclkTabla)
ON_STN_CLICKED(IDC_STATIC_COMMENT, &CRelations::OnClickedStaticComment)
ON_NOTIFY(NM_THEMECHANGED, IDC_STATIC_TABLE, &CRelations::OnThemechangedStaticTable)
ON_CBN_SELCHANGE(IDC_COMBO_SEX, &CRelations::OnSelchangeComboSex)
ON_CBN_SELCHANGE(IDC_COMBO_TITLE, &CRelations::OnSelchangeComboTitle)
ON_EN_CHANGE(IDC_TITOLO, &CRelations::OnChangeTitolo)
ON_EN_CHANGE(IDC_LAST_NAME, &CRelations::OnChangeLastName)
ON_EN_CHANGE(IDC_FIRST_NAME, &CRelations::OnChangeFirstName)
ON_EN_CHANGE(IDC_BIRTH_PLACE, &CRelations::OnChangeBirthPlace)
ON_EN_CHANGE(IDC_BIRTH_DATE, &CRelations::OnChangeBirthDate)
ON_CBN_SELCHANGE(IDC_COMBO_BIRTH, &CRelations::OnSelchangeComboBirth)
ON_EN_CHANGE(IDC_EDIT_OCCUPATION, &CRelations::OnChangeEditOccupation)
ON_EN_CHANGE(IDC_DEATH_PLACE, &CRelations::OnChangeDeathPlace)
ON_EN_CHANGE(IDC_DEATH_DATE, &CRelations::OnChangeDeathDate)
ON_CBN_SELCHANGE(IDC_COMBO_DEATH, &CRelations::OnSelchangeComboDeath)
ON_EN_CHANGE(IDC_COMMENT, &CRelations::OnChangeComment)



ON_EN_CHANGE(IDC_EDIT_POSTERIOR, &CRelations::OnChangeEditPosterior)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelations::OnInitDialog()
{
	CDialogEx::OnInitDialog();

//	SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	colorTable.SetTextColor( theApp.m_colorClick );
	colorSiblings.SetTextColor( theApp.m_colorClick );
	colorMarriages.SetTextColor( theApp.m_colorClick );
	colorNewFather.SetTextColor( theApp.m_colorClick );
	colorNewMother.SetTextColor( theApp.m_colorClick );
	colorComment.SetTextColor( theApp.m_colorClick );
	colorChildren.SetTextColor( theApp.m_colorClick );

	m_command = L"SELECT title FROM titles";
	if( ! theApp.querySystem( m_command ) ) return FALSE;

	// szülõk
	
	m_ListCtrlP.KeepSortOrder(TRUE);
	m_ListCtrlP.SetExtendedStyle( m_ListCtrlP.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );	

	m_ListCtrlP.InsertColumn( LISTP_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrlP.InsertColumn( LISTP_SEX,		L"#",			LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrlP.InsertColumn( LISTP_ROWID,		L"rowid",		LVCFMT_RIGHT,	 50,-1,COL_NUM );
	m_ListCtrlP.InsertColumn( LISTP_NAME,		L"név",			LVCFMT_LEFT,	210,-1,COL_TEXT );
	m_ListCtrlP.InsertColumn( LISTP_BIRTH,		L"született",	LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrlP.InsertColumn( LISTP_DEATH,		L"meghalt",		LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrlP.InsertColumn( LISTP_COMMENT,	L"leírás",		LVCFMT_LEFT,	240,-1,COL_TEXT);

	// házasságok
	m_ListCtrlM.KeepSortOrder(TRUE);
	m_ListCtrlM.SetExtendedStyle(m_ListCtrlM.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );	

	m_ListCtrlM.InsertColumn( LISTM_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrlM.InsertColumn( LISTM_ROWIDM,		L"házasság",	LVCFMT_RIGHT,	 50,-1,COL_HIDDEN );
	m_ListCtrlM.InsertColumn( LISTM_ORDER,		L"#",			LVCFMT_RIGHT,	 20,-1,COL_NUM );
	m_ListCtrlM.InsertColumn( LISTM_ROWID,		L"rowid",		LVCFMT_RIGHT,	 50,-1,COL_NUM );
	m_ListCtrlM.InsertColumn( LISTM_NAME,		L"név",			LVCFMT_LEFT,	170,-1,COL_TEXT);
	m_ListCtrlM.InsertColumn( LISTM_BIRTH,		L"született",	LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrlM.InsertColumn( LISTM_DEATH,		L"meghalt",		LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrlM.InsertColumn( LISTM_COMMENT,	L"leírás",		LVCFMT_LEFT,	120,-1,COL_TEXT);
	m_ListCtrlM.InsertColumn( LISTM_MARRIAGE,	L"házasság",	LVCFMT_LEFT,	120,-1,COL_TEXT);


	// testvérek
	m_ListCtrlS.SortByHeaderClick(TRUE);
	m_ListCtrlS.KeepSortOrder(TRUE);
	m_ListCtrlS.SetExtendedStyle(m_ListCtrlS.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );	

	m_ListCtrlS.InsertColumn( LISTS_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrlS.InsertColumn( LISTS_NUMBER,		L"#",			LVCFMT_RIGHT,	 40,-1,COL_NUM );
	m_ListCtrlS.InsertColumn( LISTS_ROWID,		L"rowid",		LVCFMT_RIGHT,	 50,-1,COL_NUM );
	m_ListCtrlS.InsertColumn( LISTS_NAME,		L"név",			LVCFMT_LEFT,	170,-1,COL_TEXT);
	m_ListCtrlS.InsertColumn( LISTS_BIRTH,		L"született",	LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrlS.InsertColumn( LISTS_DEATH,		L"meghalt",		LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrlS.InsertColumn( LISTS_COMMENT,	L"leírás",		LVCFMT_LEFT,	240,-1,COL_TEXT);

	// gyerekek
	m_ListCtrlC.SortByHeaderClick(TRUE);
	m_ListCtrlC.KeepSortOrder(TRUE);
	m_ListCtrlC.SetExtendedStyle(m_ListCtrlS.GetExtendedStyle()| LVS_EX_GRIDLINES | LVS_EX_HEADERDRAGDROP );	

	m_ListCtrlC.InsertColumn( LISTC_LINENUMBER,		L"line#",				LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrlC.InsertColumn( LISTC_NUMBER,			L"#",					LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrlC.InsertColumn( LISTC_ROWID,			L"rowid",				LVCFMT_RIGHT,	 50,-1,COL_NUM );
	m_ListCtrlC.InsertColumn( LISTC_ORDERFATHER,	L"apja n. gyereke",		LVCFMT_RIGHT,	100,-1,COL_NUM );
	m_ListCtrlC.InsertColumn( LISTC_ORDERMOTHER,	L"anyja n. gyereke",	LVCFMT_RIGHT,	100,-1,COL_NUM );
	m_ListCtrlC.InsertColumn( LISTC_MOTHER_INDEX,	L"#mother",				LVCFMT_RIGHT,	 40,-1,COL_NUM);
	m_ListCtrlC.InsertColumn( LISTC_NAME,			L"név",					LVCFMT_LEFT,	150,-1,COL_TEXT);
	m_ListCtrlC.InsertColumn( LISTC_BIRTH,			L"született",			LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrlC.InsertColumn( LISTC_DEATH,			L"meghalt",				LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrlC.InsertColumn( LISTC_COMMENT,		L"leírás",				LVCFMT_LEFT,	100,-1,COL_TEXT );

	m_command = L"SELECT title FROM titles ORDER BY title";
	if( ! theApp.querySystem( m_command ) ) return false;

	m_rowidFirst = m_rowid;
	createScreen( m_rowid );
	m_menuValid = 0;
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnReset()
{
	savePeople();
	createScreen( m_rowidFirst );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations:: createScreen( CString rowid )
{
	displayPicture( rowid );

	m_rowid = rowid;

	people(rowid);
	szulok( m_father_id, m_mother_id );
	hazastarsak( rowid, m_sex_id );
	testverek( rowid, m_father_id, m_mother_id );
	gyerekek( rowid, m_sex_id );

//	if( theApp.m_inputMode == GAHTML )
//		str.Format( L"%s - %s       LDblClick: váltás az ember rokonságára   RDblClick: a list aembereinek ga.html sorai", tableHeader, theApp.sourceA[sourceI] );
//	else
//		str.Format( L"%s - %s       LDblClick: váltás az ember rokonságára", tableHeader, theApp.sourceA[sourceI] );
	str.Format( L"Hároom generációs személyi adatlap -  %s (rowid=%s) és rokonai", m_name, rowid );
	SetWindowText( str );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// S Z Ü L Õ K //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::people( CString rowid )
{
	CString fileNumber;
	CString familyNumber;
	CString lineNumber;
	CString title;

	CString sex;
	CString name;
	CString birth;
	CString death;
	CString sources;

	CString str1;
	CString table;
	CString source;
	CString generation;

	CString familyName;
	CString tableHeader;
	CString tableRoman;

	CString orderFather;
	CString orderMother;	
	CString motherIndex;
	CString folyt;


	int		sourceI;
	int		tableAncestry;


	m_command.Format(L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid );
	if( !query( m_command ) ) return;

	fileNumber		= m_recordset->GetFieldString( PEOPLE_FILENUMBER );
	familyNumber	= m_recordset->GetFieldString( PEOPLE_FAMILYNUMBER );
	m_tableNumber	= m_recordset->GetFieldString( PEOPLE_TABLENUMBER );
	lineNumber		= m_recordset->GetFieldString( PEOPLE_LINENUMBER );

	if( !query2( L"SELECT rowid FROM tables" ) ) return;
	if( !m_recordset2->RecordsCount() )
		GetDlgItem( IDC_TABLE )->EnableWindow( false );

	m_command.Format( L"SELECT rowid,* FROM tables WHERE rowid='%s'", m_tableNumber );
	if( !query2( m_command ) ) return;


	familyName	= m_recordset2->GetFieldString( TABLES_FAMILY_NAME );
	tableRoman		= m_recordset2->GetFieldString( TABLES_TABLENUMBERROMAN );
	tableHeader	= m_recordset2->GetFieldString( TABLES_TABLEHEADER );

	GetDlgItem( IDC_STATIC_TABLE )->SetWindowTextW( tableHeader );

	sourceI			= _wtoi( m_recordset->GetFieldString( PEOPLE_SOURCE ) );
	generation		= m_recordset->GetFieldString( PEOPLE_GENERATION);
	generation.Trim();
	tableAncestry	= _wtoi( m_recordset->GetFieldString( PEOPLE_TABLEANCESTRY ));

	sex			= m_recordset->GetFieldString( PEOPLE_SEX_ID );
	m_sex_id	= _wtoi( sex );
	m_title		= m_recordset->GetFieldString( PEOPLE_TITLE );
	m_titolo		= m_recordset->GetFieldString( PEOPLE_TITOLO );
	m_last_name	= m_recordset->GetFieldString( PEOPLE_LAST_NAME );
	m_first_name	= m_recordset->GetFieldString( PEOPLE_FIRST_NAME);
	m_posterior		= m_recordset->GetFieldString( PEOPLE_POSTERIOR);

	m_birth_place	= m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE );
	m_birth_date	= m_recordset->GetFieldString( PEOPLE_BIRTH_DATE );
	m_death_place	= m_recordset->GetFieldString( PEOPLE_DEATH_PLACE );
	m_death_date	= m_recordset->GetFieldString( PEOPLE_DEATH_DATE );

	m_comment		= m_recordset->GetFieldString( PEOPLE_COMMENT );
	m_occupation	= m_recordset->GetFieldString( PEOPLE_OCCUPATION );

	comboSex.ResetContent();
	comboSex.AddString( L"" );
	comboSex.AddString( L"ffi" );
	comboSex.AddString( L"nõ" );
	comboSex.SetCurSel( m_sex_id );

	
	comboBirth.ResetContent();
	for( UINT i = 0; i < dateModifiersSize; ++i )
	{
		comboBirth.AddString( dateModifiers[i].modifier );
	}
	comboBirth.SetCurSel(0);

	comboDeath.ResetContent();
	for( UINT i = 0; i < dateModifiersSize; ++i )
	{
		comboDeath.AddString( dateModifiers[i].modifier );
	}
	comboDeath.SetCurSel(0);
	
	
	int		titleX = 0;
	comboTitle.AddString( L"" );
	for( UINT i = 0; i < theApp.m_recordsetSystem->RecordsCount(); ++i )
	{
		title =  theApp.m_recordsetSystem->GetFieldString( 0 ); 
		comboTitle.AddString( title );
		if( title == m_title ) 
			titleX = i+1;
		theApp.m_recordsetSystem->MoveNext();
	}
	comboTitle.SetCurSel( titleX );

	UpdateData( TOSCREEN );

	sources		= m_recordset->GetFieldString( PEOPLE_SOURCES );
	m_father_id	= m_recordset->GetFieldString( PEOPLE_FATHER_ID );
	m_mother_id	= m_recordset->GetFieldString( PEOPLE_MOTHER_ID );


	orderFather	= m_recordset->GetFieldString( PEOPLE_ORDERFATHER );
	orderMother	= m_recordset->GetFieldString( PEOPLE_ORDERMOTHER );

	motherIndex	= m_recordset->GetFieldString( PEOPLE_MOTHER_INDEX );
	folyt		= m_recordset->GetFieldString( PEOPLE_FOLYT );

	name.Format( L"%s %s %s", m_titolo, m_last_name, m_first_name );
	m_name.Format( L"%s %s", m_last_name, m_first_name );
	p_rowid = m_rowid;

	birth.Format( L"%s %s", m_birth_place, m_birth_date );
	death.Format( L"%s %s", m_death_place, m_death_date );
	birth.Trim();
	death.Trim();

	colorName.SetTextColor( colorRed );
	GetDlgItem( IDC_NAME )->SetWindowText( m_name );
	m_changed = false;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// S Z Ü L Õ K //////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::szulok( CString father_id, CString mother_id )
{
	int nItem;
	CString lineNumber(L"");
	CString rowid;
	CString name;
	CString birth;
	CString death;
	CString comment;

	m_father.Empty();
	m_mother.Empty();

	m_ListCtrlP.DeleteAllItems();
	if( !father_id.IsEmpty() && father_id.Compare( L"0" ) )
	{
		m_command.Format( L"SELECT rowid, * FROM people WHERE rowid = '%s'", father_id );
		if( !query( m_command ) ) return;
		
		lineNumber = m_recordset->GetFieldString( PEOPLE_LINENUMBER );

		m_father.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		m_father.Trim();

		name.Format( L"%s %s %s", m_recordset->GetFieldString( PEOPLE_TITOLO ), m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		name.Trim();
		birth.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ), m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		birth.Trim();
		death.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ), m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		death.Trim();
		comment = m_recordset->GetFieldString( PEOPLE_COMMENT );

		nItem = m_ListCtrlP.InsertItem( 0, lineNumber );
		m_ListCtrlP.SetItemText( 0,  LISTP_ROWID, father_id );
		m_ListCtrlP.SetItemText( 0,  LISTP_NAME, name );


		m_ListCtrlP.SetItemText( 0,  LISTP_BIRTH, birth );
		m_ListCtrlP.SetItemText( 0,  LISTP_DEATH, death );
		m_ListCtrlP.SetItemText( 0,  LISTP_COMMENT, comment );
	}
//	else
//	{
//		nItem = m_ListCtrlP.InsertItem( 0, lineNumber );
//	}
	
	if( !mother_id.IsEmpty() && mother_id.Compare( L"0" ) )
	{
		m_command.Format( L"SELECT rowid, * FROM people WHERE rowid = '%s'", mother_id );
		if( !query( m_command ) ) return;

		lineNumber = m_recordset->GetFieldString( PEOPLE_LINENUMBER );
		m_mother.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		m_mother.Trim();
		name.Format( L"%s %s %s", m_recordset->GetFieldString( PEOPLE_TITOLO ), m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		name.Trim();
		birth.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ), m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		birth.Trim();
		death.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ), m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		death.Trim();
		comment = m_recordset->GetFieldString( PEOPLE_COMMENT );

		if( !m_ListCtrlP.GetItemCount() )							// nincs apa, azt a sort üresen hagyjuk
			nItem = m_ListCtrlP.InsertItem( 0, lineNumber );

		nItem = m_ListCtrlP.InsertItem( 1, lineNumber );
		m_ListCtrlP.SetItemText( 1,  LISTP_ROWID, mother_id );
		m_ListCtrlP.SetItemText( 1,  LISTP_NAME, name );


		m_ListCtrlP.SetItemText( nItem,  LISTP_BIRTH, birth );
		m_ListCtrlP.SetItemText( nItem,  LISTP_DEATH, death );
		m_ListCtrlP.SetItemText( nItem,  LISTP_COMMENT, comment );
	}
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// H Á Z A S T Á R S A K ////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::hazastarsak( CString rowid, int sex_id )
{
	CString lineNumber;
	CString rowidM;
	CString order1;
	CString order2;
	CString rowidS;
	CString	name;
	CString	birth;
	CString	death;
	CString comment;
	CString marriage;
	
	CString spouse1_id;
	CString	spouse2_id;
	CString	orderHusband;
	CString	orderWife;
	CString order;
	
	
	CString parent2Index;

	if( sex_id == MAN )
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse1_id=%s OR spouse2_id=%s ORDER BY orderWife", rowid, rowid );
	else
		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse1_id=%s OR spouse2_id=%s ORDER BY orderHusband", rowid, rowid );
	if( !query( m_command ) ) return;
	


	m_ListCtrlM.DeleteAllItems();
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		
		rowidM			= m_recordset->GetFieldString( MARRIAGES_ROWID );
		spouse1_id		= m_recordset->GetFieldString( MARRIAGES_SPOUSE1_ID );
		spouse2_id		= m_recordset->GetFieldString( MARRIAGES_SPOUSE2_ID );
		orderHusband	=  m_recordset->GetFieldString( MARRIAGES_ORDERHUSBAND );
		orderWife		=  m_recordset->GetFieldString( MARRIAGES_ORDERWIFE );


		if( rowid == spouse1_id )
		{
			m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'",spouse2_id );
			order = orderWife;
		}
		else
		{
			m_command.Format( L"SELECT rowid,* FROM people WHERE rowid ='%s'",spouse1_id );
			order = orderHusband;
		}
		lineNumber		= m_recordset->GetFieldString( PEOPLE_LINENUMBER );
		if( !query2( m_command ) ) return;
		
		
		rowidS		= m_recordset2->GetFieldString( PEOPLE_ROWID );
		name.Format( L"%s %s", m_recordset2->GetFieldString( PEOPLE_LAST_NAME ), m_recordset2->GetFieldString( PEOPLE_FIRST_NAME ) );
		name.Trim();

		birth.Format( L"%s %s", m_recordset2->GetFieldString( PEOPLE_BIRTH_PLACE ), m_recordset2->GetFieldString( PEOPLE_BIRTH_DATE ) );
		death.Format( L"%s %s", m_recordset2->GetFieldString( PEOPLE_DEATH_PLACE ), m_recordset2->GetFieldString( PEOPLE_DEATH_DATE ) );
		marriage.Format( L"%s %s", m_recordset->GetFieldString( MARRIAGES_PLACE ), m_recordset->GetFieldString( MARRIAGES_DATE ) );
			
		comment		= m_recordset2->GetFieldString( PEOPLE_COMMENT );

		m_ListCtrlM.InsertItem( i, lineNumber );
		m_ListCtrlM.SetItemText( i, LISTM_ROWIDM, rowidM );
		m_ListCtrlM.SetItemText( i, LISTM_ORDER, order );
		m_ListCtrlM.SetItemText( i, LISTM_ROWID, rowidS );
		m_ListCtrlM.SetItemText( i, LISTM_NAME, name );
		m_ListCtrlM.SetItemText( i, LISTM_BIRTH, birth );
		m_ListCtrlM.SetItemText( i, LISTM_DEATH, death );
		m_ListCtrlM.SetItemText( i, LISTM_MARRIAGE, marriage );
		m_ListCtrlM.SetItemText( i, LISTM_COMMENT, comment );
	}
	m_order1 = m_ListCtrlM.GetItemCount() + 1;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// T E S T V É R E K ///////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// nincs editálás, csak felsorolás /////////////////////////////////////////////////////////////////////////////////
void CRelations::testverek( CString rowid, CString father_id, CString mother_id )
{
	CString lineNumber;
	CString rowidS;
	CString name;
	CString birth;
	CString death;
	CString comment;

	CString condition(L"");

	BOOL	fatherYes = !father_id.IsEmpty() & father_id.Compare(L"0" );
	BOOL	motherYes = !mother_id.IsEmpty() & mother_id.Compare(L"0" );

	m_ListCtrlS.DeleteAllItems();


	if( fatherYes && motherYes )
		condition.Format( L"rowid!='%s' AND (father_id='%s' OR mother_id='%s')", rowid, father_id, mother_id );
	else if( fatherYes && !motherYes )
		condition.Format( L"rowid!='%s' AND father_id='%s'", rowid, father_id );
	else if( !fatherYes && motherYes )
		condition.Format( L"rowid!='%s' AND mother_id='%s'", rowid, mother_id );
	else
		return;

	m_command.Format( L"SELECT rowid,* FROM people WHERE %s ORDER BY orderFather", condition );
	if( !query( m_command ) ) return;

	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		lineNumber	= m_recordset->GetFieldString( PEOPLE_LINENUMBER );
		rowidS		= m_recordset->GetFieldString( PEOPLE_ROWID );
		name.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		birth.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ), m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		death.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ), m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		comment = m_recordset->GetFieldString( PEOPLE_COMMENT );

		m_ListCtrlS.InsertItem( i, lineNumber );
		str.Format( L"%d", i+1 );
		m_ListCtrlS.SetItemText( i, LISTS_NUMBER, str );
		m_ListCtrlS.SetItemText( i, LISTS_ROWID, rowidS );
		m_ListCtrlS.SetItemText( i, LISTS_NAME, name );
		m_ListCtrlS.SetItemText( i, LISTS_BIRTH, birth );
		m_ListCtrlS.SetItemText( i, LISTS_DEATH, death );
		m_ListCtrlS.SetItemText( i, LISTS_COMMENT, comment );
		m_recordset->MoveNext();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////// G Y E R E K E K //////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// semmmi editálás nincs, csak listázás ////////////////////////////////////////////////////////////////////////////////
void CRelations::gyerekek( CString rowid, int sex_id )
{
	if( !m_rowid ) return;

	CString lineNumber;
	CString rowidC;
	CString name;
	CString birth;
	CString death;
	CString comment;


	CString condition;
	if( sex_id == MAN )
		condition.Format( L"father_id='%s'", rowid );
	else
		condition.Format( L"mother_id='%s'", rowid );

	m_command.Format( L"SELECT rowid,* FROM people WHERE %s ORDER BY orderFather", condition );
	if( !query( m_command ) ) return;

	int nItem;
	m_ListCtrlC.DeleteAllItems();
	for(UINT i = 0; i < m_recordset->RecordsCount(); ++i )
	{
		lineNumber	= m_recordset->GetFieldString( PEOPLE_LINENUMBER );
		rowidC		= m_recordset->GetFieldString( PEOPLE_ROWID );
		name.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_LAST_NAME ), m_recordset->GetFieldString( PEOPLE_FIRST_NAME ) );
		birth.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_BIRTH_PLACE ), m_recordset->GetFieldString( PEOPLE_BIRTH_DATE ) );
		death.Format( L"%s %s", m_recordset->GetFieldString( PEOPLE_DEATH_PLACE ), m_recordset->GetFieldString( PEOPLE_DEATH_DATE ) );
		comment = m_recordset->GetFieldString( PEOPLE_COMMENT );

		nItem = m_ListCtrlC.InsertItem( i, lineNumber );
		str.Format( L"%d", i + 1 );
		m_ListCtrlC.SetItemText( nItem, LISTC_NUMBER, str );
		m_ListCtrlC.SetItemText( nItem, LISTC_ROWID, rowidC );
		m_ListCtrlC.SetItemText( nItem, LISTC_ORDERFATHER, m_recordset->GetFieldString( PEOPLE_ORDERFATHER ) );
		m_ListCtrlC.SetItemText( nItem, LISTC_ORDERMOTHER, m_recordset->GetFieldString( PEOPLE_ORDERMOTHER ) );
		m_ListCtrlC.SetItemText( nItem, LISTC_MOTHER_INDEX, m_recordset->GetFieldString( PEOPLE_MOTHER_INDEX ) );
		m_ListCtrlC.SetItemText( nItem, LISTC_NAME, name );
		m_ListCtrlC.SetItemText( nItem, LISTC_BIRTH, birth );
		m_ListCtrlC.SetItemText( nItem, LISTC_DEATH, death );
		m_ListCtrlC.SetItemText( nItem, LISTC_COMMENT, comment );
		m_recordset->MoveNext();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////// U T I L I T I E S ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelations::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CRelations::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnDblclkListM(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem	= pNMItemActivate->iItem;
	CString rowid = m_ListCtrlM.GetItemText( nItem, LISTM_ROWID );
	if( m_changed ) savePeople();
	createScreen( rowid );
	*pResult = 0;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnDblclkListS(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem	= pNMItemActivate->iItem;
	CString rowid = m_ListCtrlS.GetItemText( nItem, LISTP_ROWID );
	if( m_changed ) savePeople();
	createScreen( rowid );

	*pResult = 0;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnDblclkListP(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem	= pNMItemActivate->iItem;
	CString rowid = m_ListCtrlP.GetItemText( nItem, LISTP_ROWID );
	if( m_changed ) savePeople();
	createScreen( rowid );
	*pResult = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnDblclkListC(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem	= pNMItemActivate->iItem;
	CString rowid = m_ListCtrlC.GetItemText( nItem, LISTP_ROWID );
	if( m_changed ) savePeople();
	createScreen( rowid );
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnRdblclkListP(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( theApp.m_inputMode != GAHTML ) return;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");
	CString lineNumber;

	vLines.clear();
	for( int i = 0; i < m_ListCtrlP.GetItemCount(); ++i )
	{
		lineNumber = m_ListCtrlP.GetItemText( i, LISTP_LINENUMBER );
		if( !lineNumber.IsEmpty() )
			vLines.push_back( lineNumber );
	}

	CHtmlLines dlg;
	dlg.child = L"Szülõk";
	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();
	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnRdblclkListM(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( theApp.m_inputMode != GAHTML ) return;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");
	CString lineNumber;

	vLines.clear();
	for( int i = 0; i < m_ListCtrlM.GetItemCount(); ++i )
	{
		lineNumber = m_ListCtrlM.GetItemText( i, LISTM_LINENUMBER );
		if( !lineNumber.IsEmpty() )
			vLines.push_back( lineNumber );
	}

	CHtmlLines dlg;
	dlg.child = L"Házastársak";
	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnRdblclkListC(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( theApp.m_inputMode != GAHTML ) return;
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
		std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");
	CString lineNumber;

	vLines.clear();
	for( int i = 0; i < m_ListCtrlC.GetItemCount(); ++i )
	{
		lineNumber = m_ListCtrlC.GetItemText( i, LISTC_LINENUMBER );
		if( !lineNumber.IsEmpty() )
			vLines.push_back( lineNumber );
	}

	CHtmlLines dlg;
	dlg.child = L"Gyerekek";
	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();


	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnRdblclkListS(NMHDR *pNMHDR, LRESULT *pResult)
{
	if( theApp.m_inputMode != GAHTML ) return;
	std::vector<CString> vLines;

	int cnt = 0;
	CString name(L"");
	CString lineNumber;

	vLines.clear();
	for( int i = 0; i < m_ListCtrlS.GetItemCount(); ++i )
	{
		lineNumber = m_ListCtrlS.GetItemText( i, LISTS_LINENUMBER );
		if( !lineNumber.IsEmpty() )
			vLines.push_back( lineNumber );
	}

	CHtmlLines dlg;
	dlg.child = L"Testvérek";
	dlg._what = 1;
	dlg.vLines = &vLines;

	dlg.DoModal();

	*pResult = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnClickedMarriages()
{
	int db = m_ListCtrlM.GetItemCount();

	if( !db )
	{
		if( !newSpouse(++db) ) return;
	}
	else
	{
		str.Format( L"%s %d. házastársát adarod megadni?", m_name, ++db );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
		if( !newSpouse( db ) ) return;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CRelations::newSpouse( int db )
{
	CString caption;
	CString rowidH;
	CString rowidW;
	int		orderHusband = 1;
	int		orderWife = 1;

	if( m_sex_id == MAN )
	{
		rowidH = m_rowid;
		caption.Format( L"Add meg %s feleségét!", m_name );
		orderWife = m_ListCtrlM.GetItemCount() + 1;
	}
	else
	{
		rowidW = m_rowid;
		caption.Format( L"Add meg %s férjét!", m_name );
		orderHusband = m_ListCtrlM.GetItemCount() + 1;
	}

	CNewPeople dlg;
	dlg.m_caption = caption;
	if( dlg.DoModal() == IDCANCEL ) return false;


	if( m_sex_id == dlg.m_sex_id )
	{
		AfxMessageBox( L"A házaspár azonos nemû!" );
		return( false );
	}
	if( m_sex_id == MAN )
		m_command.Format( L"INSERT INTO marriages ( spouse1_id, spouse2_id, orderHusband  ) VALUES ('%s', '%s', %d )", m_rowid, dlg.m_rowid, orderWife );
	else
		m_command.Format( L"INSERT INTO marriages ( spouse1_id, spouse2_id, orderWife ) VALUES ('%s', '%s', %d )", dlg.m_rowid, m_rowid, orderHusband );
	theApp.execute( m_command );

	hazastarsak( p_rowid, m_sex_id );
	return true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnClickedNewFather()
{
	CString rowidF;
	rowidF = m_ListCtrlP.GetItemText( 0, 2 );
	if( !rowidF.IsEmpty() )
	{
		str.Format( L"Meg akarod változtatni %s apját?", m_name );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
	}

	CNewPeople dlg;
	
	dlg.m_last_name = m_last_name;
	if( dlg.DoModal() == IDCANCEL ) return;

	m_father_id = dlg.m_rowid;
	m_command.Format( L"UPDATE people SET father_id ='%s' WHERE rowid='%s'", m_father_id, m_rowid );
	if( !theApp.execute( m_command ) ) return;

	szulok( m_father_id, m_mother_id );
	insertMarriage();
	return;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnClickedNewMother()
{
	CString rowidM;
	rowidM = m_ListCtrlP.GetItemText( 1, 2 );
	if( !rowidM.IsEmpty() )
	{
		str.Format( L"Meg akarod változtatni %s anyját?", m_name );
		if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;
	}

	CNewPeople dlg;
	dlg.m_last_name.Empty();
	if( dlg.DoModal() == IDCANCEL ) return;

	m_mother_id = dlg.m_rowid;
	m_command.Format( L"UPDATE people SET mother_id ='%s' WHERE rowid='%s'", m_mother_id, m_rowid );
	if( !theApp.execute( m_command ) ) return;

	szulok( m_father_id, m_mother_id );
	insertMarriage();
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::insertMarriage()
{
	if( !m_father.IsEmpty() && !m_mother.IsEmpty() )
	{
		m_command.Format( L"SELECT count() FROM marriages WHERE spouse1_id=='%s' AND spouse2_id=='%s'", m_father_id, m_mother_id );
		if( !theApp.query2( m_command ) ) return;
		if( !_wtoi( theApp.m_recordset2->GetFieldString(0 ) ) )
		{
			str.Format( L"%s és %s házastársak?", m_father, m_mother );
			if( AfxMessageBox( str, MB_YESNO ) == IDYES )
			{
				m_command.Format( L"INSERT INTO marriages (spouse1_id, spouse2_id ) VALUES ( '%s', '%s' )", m_father_id, m_mother_id ); 
				if( !theApp.execute( m_command ) ) return;
			}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnClickedSiblings()
{
	int parentCnt = m_ListCtrlM.GetItemCount();

	CString father_id;
	CString mother_id;
	int		parent2Index = 1;

	CNewPeople dlg;
	dlg.m_last_name = m_last_name;

	if( dlg.DoModal() == IDCANCEL ) return;
	switch( parentCnt )
	{
	case 0:			// csak az egyik szülõ ismert
		if( m_sex_id == MAN )
		{
			father_id = m_rowid;
			mother_id.Empty();
		}
		else
		{
			father_id.Empty();
			mother_id = m_rowid;
		}
		break;
	case 1:
		if( m_sex_id == MAN )
		{
			father_id = m_rowid;
			mother_id = m_ListCtrlM.GetItemText( 0, LISTM_ROWID );
		}
		else
		{
			father_id = m_ListCtrlM.GetItemText( 0, LISTM_ROWID );
			mother_id = m_rowid;
		}
		break;
	default:
		CMoreSpouses dlg;
		dlg.m_rowid = m_rowid;
		dlg.m_sex_id = m_sex_id;
		if( dlg.DoModal() == IDCANCEL  ) return;
		parent2Index = dlg.m_mother_index;
		if( m_sex_id == MAN )
		{
			father_id = m_rowid;
				mother_id = m_ListCtrlM.GetItemText( parent2Index - 1, LISTM_ROWID );
		}
		else
		{
			father_id = m_ListCtrlM.GetItemText( parent2Index - 1, LISTM_ROWID );
			mother_id = m_rowid;
		}
		break;
	}
	m_command.Format( L"UPDATE people SET father_id='%s', mother_id='%s', parent2Index=%d WHERE rowid = '%s'", father_id, mother_id, parent2Index, dlg.m_rowid );
	theApp.execute( m_command );
	testverek( p_rowid, father_id, mother_id );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CRelations::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;

	CRect rectP;
	CRect rectM;
	CRect rectS;
	CRect rectC;

	listCtrlFlag = -1;

	GetDlgItem( IDC_LISTP )->GetWindowRect( rectP );
	GetDlgItem( IDC_LISTM )->GetWindowRect( rectM );
	GetDlgItem( IDC_LISTS )->GetWindowRect( rectS );
	GetDlgItem( IDC_LISTC )->GetWindowRect( rectC );

	if( rectP.top <= point->y && point->y <= rectP.bottom )			// szülei
	{
		if( m_ListCtrlP.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false; 
		listCtrlFlag = FM;
    }
	else if( rectM.top <= point->y && point->y <= rectM.bottom )			// házastársak
	{
		if( m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false; 
		listCtrlFlag = M;
		if(Menu.LoadMenu( IDR_DROPDOWN_SPOUSE ))
		{
			pPopup = Menu.GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
		}
		return TRUE;
	}
	else if( rectS.top <= point->y && point->y <= rectS.bottom )			// testvérek
	{
		if( m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false; 
		listCtrlFlag = S;
	}
	else if( rectC.top <= point->y && point->y <= rectC.bottom )			// gyerekek
	{
		if( m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED) == -1 ) return false; 
		listCtrlFlag = C;
	}
	if( listCtrlFlag != -1 )
	{
		if(Menu.LoadMenu( IDR_DROPDOWN_DELETE_UPDATE ))
		{
			pPopup = Menu.GetSubMenu(0);
			pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
		}
	}
	return TRUE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnEditDelete()
{
	int	nItem;
	CString name;
	CString rowid;
	switch( listCtrlFlag )
	{
	case FM:
		nItem	= m_ListCtrlP.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name	= m_ListCtrlP.GetItemText( nItem, LISTP_NAME );
		rowid	= m_ListCtrlP.GetItemText( nItem, LISTP_ROWID );
		str.Format( L"Tényleg töröli alkarod %s-t a szülõk közül?", name);
		break;
	case M:
		nItem	= m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name	= m_ListCtrlM.GetItemText( nItem, LISTM_NAME );
		rowid	= m_ListCtrlM.GetItemText( nItem, LISTM_ROWID );
		str.Format( L"Tényleg töröli alkarod %s-t a házastársak közül?", name);
		break;
	case S:
		nItem	= m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name	= m_ListCtrlS.GetItemText( nItem, LISTS_NAME );
		rowid	= m_ListCtrlS.GetItemText( nItem, LISTS_ROWID );
		str.Format( L"Tényleg töröli alkarod %s-t a testvérek közül?", name);
		break;
	case C:
		nItem	= m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name	= m_ListCtrlC.GetItemText( nItem, LISTC_NAME );
		rowid	= m_ListCtrlC.GetItemText( nItem, LISTC_ROWID );
		str.Format( L"Tényleg töröli alkarod %s-t a gyerekek közül?", name);
		break;
	}
	if( AfxMessageBox( str, MB_YESNO ) == IDNO ) return;

	CString command;
	switch( listCtrlFlag )
	{
	case FM:
		if( nItem == 0 )
		{
			command.Format( L"UPDATE people SET father_id = 0 WHERE rowid == '%s'", p_rowid );
			m_father_id = "";
		}
		else
		{
			command.Format( L"UPDATE people SET mother_id = 0 WHERE rowid == '%s'", p_rowid );
			m_mother_id = "";
		}
		if( !theApp.execute( command ) ) return;
		szulok( m_father_id, m_mother_id );
		break;
	case M:
		if( m_sex_id == MAN )
			command.Format( L"DELETE FROM marriages WHERE spouse1_id='%s' AND spouse2_id == '%s'", p_rowid, rowid );
		else
			command.Format( L"DELETE FROM marriages WHERE spouse1_id='%s' AND spouse2_id == '%s'", rowid, p_rowid );
		if( !theApp.execute( command ) ) return;
		hazastarsak( p_rowid, m_sex_id );
		break;
	case S:
		command.Format( L"UPDATE people SET father_id=0, mother_id=0 WHERE rowid = '%s'", rowid );
		if( !theApp.execute( command ) ) return;
		testverek( p_rowid, m_father_id, m_mother_id  );
		break;
	case C:
		command.Format( L"UPDATE people SET father_id=0, mother_id=0 WHERE rowid = '%s'", rowid );
		if( !theApp.execute( command ) ) return;
		gyerekek( p_rowid, m_sex_id );
		break;
	}


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnEditGyerek()
{
	int nItem	= m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED); 
	CString rowidM;
	rowidM = m_ListCtrlM.GetItemText( nItem, LISTM_ROWID );

	CNewPeople dlg;

	if( m_sex_id == MAN )
	{
//		dlg.m_orderFather = m_ListCtrlC.GetItemCount() + 1;
//		dlg.m_orderMother = getOrderMother( rowidM );		// az anya rowid-ja
	}
	else
	{
//		dlg.m_orderFather = getOrderFather( rowidM );		// az apa rowid-ja
//		dlg.m_orderMother = m_ListCtrlC.GetItemCount() + 1;
	}

	if( dlg.DoModal() == IDCANCEL ) return;

	gyerekek( p_rowid, m_sex_id );  // m_sex_id: apa vagy anya gyerekeit listázzuk? 

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnEditUpdate()
{

	int	nItem;
	CString name;

	CNewPeople dlg;

	switch( listCtrlFlag )
	{
	case PP:
/*
		nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name = m_ListCtrl.GetItemText( nItem, LIST_NAME );
		
		dlg.m_rowidClick	= m_ListCtrl.GetItemText( nItem, LIST_ROWID );
		str.Format( L"%s szerkesztése", name );

		dlg.m_newPeople = -1;
		dlg.DoModal();
		people( dlg.m_rowidClick );
*/
		break;
	case FM:
		nItem	= m_ListCtrlP.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name = m_ListCtrlP.GetItemText( nItem, LISTP_NAME );
		dlg.m_rowid	= m_ListCtrlP.GetItemText( nItem, LISTP_ROWID );
		str.Format( L"%s szerkesztése", name );
//		dlg.m_newPeople = -1;
		dlg.DoModal();
		szulok( m_father_id, m_mother_id );
		break;
	case M:
		nItem	= m_ListCtrlM.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name = m_ListCtrlM.GetItemText( nItem, LISTM_NAME );
//		dlg.m_rowidClick	= m_ListCtrlM.GetItemText( nItem, LISTM_ROWID );
		str.Format( L"%s szerkesztése", name );
//		dlg.m_newPeople = -1;
		dlg.DoModal();
		hazastarsak( p_rowid, m_sex_id );
		break;
	case S:
		nItem	= m_ListCtrlS.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
		name = m_ListCtrlS.GetItemText( nItem, LISTS_NAME );
//		dlg.m_rowidClick	= m_ListCtrlS.GetItemText( nItem, LISTS_ROWID );
		str.Format( L"%s szerkesztése", name );
//		dlg.m_newPeople = -1;
		dlg.DoModal();
		testverek( p_rowid, m_father_id, m_mother_id  );
		break;
	case C:
		nItem	= m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED); 
		if( nItem == -1 ) return;
//		dlg.m_rowidClick	= m_ListCtrlC.GetItemText( nItem, LISTC_ROWID );
		name = m_ListCtrlC.GetItemText( nItem, LISTC_NAME );
		str.Format( L"%s szerkesztése", name );
//		dlg.m_newPeople = -1;
		dlg.DoModal();
		gyerekek( p_rowid, m_sex_id );
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CRelations::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	listCtrlFlag = PP;
	OnEditUpdate();
	*pResult = 0;
}
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::editMarriage( int nItem )
{
	CEditMarriage dlg;
	dlg.m_rowid.Empty();
	if( m_sex_id == MAN )
	{
		dlg.m_rowidH = m_rowid;
		dlg.m_rowidW = m_ListCtrlM.GetItemText( nItem, LISTM_ROWID );
	}
	else
	{
		dlg.m_rowidW = m_rowid;
		dlg.m_rowidH = m_ListCtrlM.GetItemText( nItem, LISTM_ROWID );
	}
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::displayPicture( CString rowid )
{

	m_command.Format( L"SELECT rowid FROM pictures WHERE id='%s' AND table_id=%d AND primaryPic=1", rowid, PEOPLE );
	if( !theApp.queryBlob( m_command ) ) return;

	InvalidateRect( NULL, true );
	m_paint = false;
	CString rowidBlob = theApp.m_recordsetBlob->GetFieldString( 0 );
	if( !rowidBlob.IsEmpty() )
	{
		_int64 blob_size;
		void* block = theApp.blobDB->blobRead( "pictures", "picture", rowidBlob, &blob_size );
		if( block == NULL ) return;
		if( !writeBlockToFile( block, blob_size ) ) return;
		m_paint = true;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CRelations::writeBlockToFile( void* block, int blob_size  )
{
	m_fileSpec.Format( L"%s\\tmp.jpg", theApp.m_workingDirectory );
	if( !openFileSpec( &theApp.fl, m_fileSpec, L"wb" ) ) return false;

	if( fwrite( block, blob_size, 1, theApp.fl ) != 1 )
	{
		AfxMessageBox( L"fwrite failed!" );
		fclose( theApp.fl );
		return false;
	}
	fclose( theApp.fl );
	return true;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnPaint()
{
	if( !m_paint ) 
	{
		CDialogEx::OnPaint();
		return;
	}
	CPaintDC dc(this);
	CDC dcMemory;
	CImage image;
	CBitmap bitmap;


	image.Load( m_fileSpec ); // just change extension to load jpg
	bitmap.Attach(image.Detach());

	dcMemory.CreateCompatibleDC( &dc);
    dcMemory.SelectObject(&bitmap);

	BITMAP bm;						// bitmap in memory
	bitmap.GetBitmap( &bm );
	int bmWidth		= bm.bmWidth;
	int bmHeight	= bm.bmHeight;
	float bmRatio	= (float)bm.bmWidth/(float)bmHeight;

	CRect rect;		// a dlg ablak méretei 0,0 pontból ( Windows koordinátákban )
	CRect rectE;	

	GetClientRect(&rect);
	GetWindowRect( rectW );

	int height = rectW.Height()/2;				// a transzformát kép magassága az ablak feléig!
	height = 160;

	int width = (int) (height * bmRatio );		// a transzformált kép szélessége a bmRatio-val arányos

	int cW = (rectW.right - rectW.left)/2;  // ablak középpontjának x koordinátája
	int cB = width/2;						// a sarokban lévõ kép középpontjának x koordinátája
	int x = cW - cB;						// az ablak új sarka

	x = rect.right - width - 20;

	int	y = 120;

	dc.SetStretchBltMode( STRETCH_HALFTONE );
	dc.StretchBlt( x, y, width, height, &dcMemory, 0,0, bmWidth, bmHeight, SRCCOPY);	
	
	CDialogEx::OnPaint();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CRelations::getOrderMother( CString rowid )
{
	m_command.Format( L"SELECT max(orderMother) FROM people WHERE rowid IN ( SELECT rowid FROM people WHERE mother_id = '%s') ORDER BY orderMother", rowid );
	if( !theApp.query3( m_command ) ) return 0;
	
//	if( theApp.m_recordset3->GetFieldString( 0 ) != NULL )
	int order = _wtoi( theApp.m_recordset3->GetFieldString( 0 ) );
	return order + 1;



}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CRelations::getOrderFather( CString rowid )
{
	m_command.Format( L"SELECT max(orderFather) FROM people WHERE rowid IN (SELECT rowid FROM people WHERE father_id = '%s') ORDER BY orderfather", rowid );
	if( !theApp.query3( m_command ) ) return 0;
//	if( theApp.m_recordset3->RecordsCount() )
	int order = _wtoi( theApp.m_recordset3->GetFieldString( 0 ) );
	return order+1;



}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnHelp()
{
	CHelp dlg;
	dlg.m_caption = L"Egy ember közvetlen családtagjainak megadása és szerkesztése";
	dlg.m_help = L"\
Az ablak elsõ listáján piros színnel feltüntetet ember közvetlen rokonait láthatjuk a következõ listákon. Ha bármelyik emberre a bal egérgombbal duplán rákattintunk, akkor az õ rokonai jelennek meg. \
Így akár az egész családfán végigpásztázhatunk. A Reset menüpont az eredeti emberhez tér vissza, és ismét az õ rokonai lesznek láthatók.\
\r\n\r\n\
A kék színû feliratra kattintva, a nevezett rokont be tudjuk vinni az adatbázisba. A rokon lehet új vagy az adatbázisban már létezõ ember. \
A megjelenõ ablakban az illetõ ember fontosabb adatait lehet megadni. Vezeték- és keresztnév megadása kötelezõ, más nem. A vezeték- és a keresztnév lehet: \
teljes, pontos név vagy a vezeték és/vagy a keresztnév tetszõleges számú kezdõ karaktere.\
\r\n\
Ha az adatbázisban vannak a megadott névvel azonos/azonos karakterekkel kezdõdõ nevû emberek, akkor listázza azokat, és lehetõségünk van \
választani az általunk megadott és a listázott emberek közül. Így elkerülhetjük, hogy egy amber többször az adatbázisba kerüljön.\
\r\n\r\n\
A jobb egérgombbal egy ember sorára kattintva egy legördülõ menü jelenik meg, amelyben az alábbi funkciók lehetnek:\
\r\n\
'Szerkeszt' - a kijelölt ember adatait módisíthatjuk.\r\n\
'Töröl' - a rokoni kapcsolatot törölhetjük, tehát nem magát az embert! Ha magát az embert akarjuk törölni, azt az emberek listájában tehetjük meg.\
\r\n\
'Gyerek' - egy házastársra kattintve van ilyen funkció, amivel a házaspár egy gyerekét adhatjuk meg.\
A gyerekek megadásának sorrendje a születésük sorrendje legyen!!\
";
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CRelations::OnPictures()
{
	CString rowidB(L"");
	CPictures dlg;

	dlg.m_filter.Format( L"WHERE table_id='%d' AND id='%s'", PEOPLE, m_rowid ); 
	dlg.m_name		= m_name;	// az ember neve
	dlg.m_rowidP	= m_rowid;	// ember rowid-ja, akinek a képeit browse-olja
	dlg.m_rowidB	= rowidB;	// kép rowid-ja, amit ábrázolni fog

	dlg.DoModal();

	displayPicture( m_rowid );


}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnBnClickedOk()
{
	savePeople();
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::savePeople()
{
	CString title;
	CString titolo;

	int titleX = comboTitle.GetCurSel();
	comboTitle.GetLBText( titleX, m_title );

	UpdateData( FROMSCREEN );

	int sex = comboSex.GetCurSel();

	CString set;
	set.Format( L"\
tableNumber='%s',\
title='%s',\
titolo='%s',\
sex_id='%d',\
first_name='%s',\
last_name='%s',\
posterior='%s',\
birth_place='%s',\
birth_date='%s',\
death_place='%s',\
death_date='%s',\
occupation='%s',\
comment='%s'\
",\
m_tableNumber,\
m_title,\
m_titolo,\
sex,\
m_first_name,\
m_last_name,\
m_posterior,\
m_birth_place,\
m_birth_date,\
m_death_place,\
m_death_date,\
m_occupation,\
m_comment\
);
	m_command.Format( L"UPDATE people SET %s WHERE rowid='%s'", set, m_rowid );
	if( !theApp.execute( m_command ) ) return;
	return;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnDblclkTabla()
{
	CTableTables dlg;
	dlg.m_select = true;			// csak táblát választ
	dlg.m_caption = L"Válaszd ki a családot!";
	if( dlg.DoModal() == IDCANCEL ) return;
	
	CString tableHeader;
	tableHeader	= dlg.m_tableHeader;
	m_tableNumber	= dlg.m_id;
	m_last_name		= dlg.m_last_name;
	m_mother_id.Empty();
	m_father_id.Empty();
	GetDlgItem( IDC_STATIC_TABLE )->SetWindowText( tableHeader );
	GetDlgItem( IDC_LAST_NAME )->SetWindowText( m_last_name );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnClickedStaticComment()
{
	CEditComment dlg;

	str.Format( L"%s %s leírása", m_last_name, m_first_name );
	dlg.m_caption = str;
	dlg.m_comment = m_comment;
	if( dlg.DoModal() == IDCANCEL ) return;
	m_comment = dlg.m_comment;
	UpdateData( TOSCREEN );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnThemechangedStaticTable(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_changed = true;
	*pResult = 0;
}
void CRelations::OnSelchangeComboSex()
{
	m_changed = true;
}
void CRelations::OnSelchangeComboTitle()
{
	m_changed = true;
}
void CRelations::OnChangeTitolo()
{
	m_changed = true;
}
void CRelations::OnChangeLastName()
{
	m_changed = true;
}
void CRelations::OnChangeFirstName()
{
	m_changed = true;
}
void CRelations::OnChangeBirthPlace()
{
	m_changed = true;
}
void CRelations::OnChangeBirthDate()
{
	m_changed = true;
}

void CRelations::OnSelchangeComboBirth()
{
	m_changed = true;
}
void CRelations::OnChangeEditOccupation()
{
	m_changed = true;
}
void CRelations::OnChangeDeathPlace()
{
	m_changed = true;
}
void CRelations::OnChangeDeathDate()
{
	m_changed = true;
}
void CRelations::OnSelchangeComboDeath()
{
	m_changed = true;
}
void CRelations::OnChangeComment()
{
	m_changed = true;
}
void CRelations::OnChangeEditPosterior()
{
	m_changed = true;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelations::OnClickedChildren()
{
	CString last_name;
	CString father;
	CString mother;
	CString rowidF;		// apa 
	CString rowidM;		// anya
	int		parent2Index = 1;
	
	int n = m_ListCtrlM.GetItemCount();		// házasrtásak száma
	switch( n )
	{
		case 0:							// csak 1 szülõ van
			if( m_sex_id == MAN )		// apa van, anya nincs
			{
				if( AfxMessageBox( L"Az anya nincs megadva!\nAkkor is megadsz egy gyereket?", MB_YESNO ) == IDNO ) return;
				rowidF = m_rowid;
				father.Format( L"%s %s", m_last_name, m_first_name );
				rowidM.Empty();
				last_name = m_last_name;
			}
			else						// anya van, apa nincs
			{
				if( AfxMessageBox( L"Az apa nincs megadva!\nAkkor is megadsz egy gyereket?", MB_YESNO ) == IDNO ) return;
				rowidM = m_rowid;
				mother.Format( L"%s %s", m_last_name, m_first_name );
				rowidF.Empty();
				parent2Index = 1;
				last_name.Empty();
			}
			break;
		case 1:							// 1 apa és 1 anya van
			if( m_sex_id == MAN )
			{
				rowidF = m_rowid;										// apa a fõ ember
				rowidM = m_ListCtrlM.GetItemText( 0, LISTM_ROWID );	// anya a házastárs
				father.Format( L"%s %s", m_last_name, m_first_name );
				mother = m_ListCtrlM.GetItemText( 0, LISTM_NAME );
				last_name = m_last_name;
			}
			else
			{
				rowidM = m_rowid;										// anaya  fõ ember
				rowidF = m_ListCtrlM.GetItemText( 0, LISTM_ROWID );	// apa aházastárs
				mother.Format( L"%s %s", m_last_name, m_first_name );
				father = m_ListCtrlM.GetItemText( 0, LISTM_NAME );
				last_name = getFirstWord( father );
			}
			parent2Index= 1;
			break;
		default:						// Az egyik szülõnke több házastûársa van
			CMoreSpouses dlg;
			dlg.m_rowid = m_rowid;
			dlg.m_sex_id = m_sex_id;
			if( dlg.DoModal() == IDCANCEL  ) return;

			parent2Index = dlg.m_mother_index;
			if( m_sex_id == MAN )
			{
				rowidF = m_rowid;
				rowidM = dlg.m_rowid;
				father.Format( L"%s %s", m_last_name, m_first_name );
				mother = m_ListCtrlM.GetItemText( parent2Index-1, LISTM_NAME );
				last_name = m_last_name;
			}
			else
			{
				rowidF = dlg.m_rowid;
				rowidM = m_rowid;
				mother.Format( L"%s %s", m_last_name, m_first_name );
				father = m_ListCtrlM.GetItemText( parent2Index-1, LISTM_NAME );
				last_name = getFirstWord( father );
			}
			break;
	}

	CNewPeople dlg;
	dlg.m_last_name = last_name;

	if( !father.IsEmpty() && !mother.IsEmpty() )
		dlg.m_caption.Format( L"Add meg %s és %s gyerekét!", father, mother );
	else if ( !father.IsEmpty() )
		dlg.m_caption.Format( L"Add meg %s gyerekét!", father );
	else 
		dlg.m_caption.Format( L"Add meg %s gyerekét!", mother );

	int nItem	= m_ListCtrlC.GetNextItem(-1, LVNI_SELECTED); 
	dlg.nItem			= nItem + 1;		// a gyerek sorszáma!!!

	int orderFather;
	int orderMother;

	if( m_sex_id == MAN )
	{
		orderFather = m_ListCtrlC.GetItemCount() + 1;
		orderMother = getOrderMother( rowidM );		// az anya rowid-ja
	}
	else
	{
		orderFather = getOrderFather( rowidF );		// az apa rowid-ja
		orderMother = m_ListCtrlC.GetItemCount() + 1;
	}
	if( dlg.DoModal() == IDCANCEL ) return;
	m_command.Format( L"UPDATE people SET father_id='%s', mother_id='%s', orderFather=%d, orderMother=%d WHERE rowid = '%s'", rowidF, rowidM, orderFather, orderMother, dlg.m_rowid );
	if( !theApp.execute( m_command ) ) return;

	gyerekek( p_rowid, m_sex_id );  // m_sex_id: apa vagy anya gyerekeit listázzuk? 
}

