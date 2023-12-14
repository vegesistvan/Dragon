// CCheckFamilyDates.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CheckFamilyDates.h"
#include "utilities.h"
#include "editHtmlLines.h"
#include "Relatives.h"
#include "EditPeople.h"
#include "CheckFamilyDatesStart.h"
#include "checkParam.h"
#include "EditPeople.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnCheckFamilyDates()
//{
//	CCheckFamilyDates dlg;
//	dlg.m_all = false;   // ki�r�s, ha a d�tumok k�z�tt ellentmond�s van
//	dlg.DoModal();
//}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnDisplayFamilies()
//{
//	CCheckFamilyDates dlg;
//	dlg.m_all = true;  // minden csal�dtagot �rjon ki
//	dlg.DoModal();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// SELECT  FROM people father
enum
{
	X_ROWID = 0,
	X_LINENUMBER,
	X_FIRST_NAME,
	X_LAST_NAME,
	X_BIRTH,
	X_DEATH,
	X_COMMENT,
	X_BIRTHP,
	X_DEATHP,
};

// SELECT from people mother
enum
{
	Y_ROWID = 0,
	Y_LINENUMBER,
	Y_FIRST_NAME,
	Y_LAST_NAME,
	Y_BIRTH,
	Y_DEATH,
	Y_COMMENT,
	Y_BIRTHP,
	Y_DEATHP,
};


// SELECT FROM people child
enum
{
	Z_ROWID = 0,
	Z_LINENUMBER,
	Z_FIRST_NAME,
	Z_LAST_NAME,
	Z_BIRTH,
	Z_DEATH,
	Z_MOTHER_ID,
	Z_MOTHERINDEX,
	Z_COMMENT,
	Z_BIRTHP,
	Z_DEATHP,
};

// m_ListCtrl oszlopok
enum
{
	L_CNT = 0,
	L_TYPE,
	L_LINENUMBER,
	L_WEDDING,
	L_INDEX,
	L_ROWID,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_AGE,
	L_APA,
	L_ANYA,
	L_YELLOW,
	L_MESSAGE,
	L_NUMOFCOLUMNS
};


enum
{
	LL_CNT = 0,
	LL_TYPE,
	LL_LINENUMBER,
	LL_WEDDING,
	LL_INDEX,
	LL_ROWID,
	LL_NAME,
	LL_BIRTHP,
	LL_BIRTH,
	LL_DEATHP,
	LL_DEATH,
	LL_AGE,
	LL_APA,
	LL_ANYA,
	LL_COMMENT,
	LL_NUMOFCOLUMNS
};

typedef struct
{
	int code;
	TCHAR* msg;
}ERR;

enum
{
	DEATHBIRTH = 0,

	BORNAFTERWEDDING,
	DIEDBEFOREWEDDING,
	BORNBEFOREWEDDING,

	DIEDBEFOREHUSBANDBIRTH,
	BORNAFTERHUSBANDDEATH,
	
	BORNBEFOREMOTHERBIRTH,
	BORNBEFOREFATHERBIRTH,
	
	BORNAFTERMOTHERDEATH,
	BORNAFTERFATHERDEATH9,
	
	DIEDBEFOREMOTHERBIRTH,
	DIEDBEFOREFATHERBIRTH,
};


ERR err[] = 
{																					// melyik sorban fordulhat el�?
	DEATHBIRTH,				L", hal�l < sz�let�s",									// f�rj, feles�g, gyerek

	BORNAFTERWEDDING,		L", esk�v� ut�n sz�letett",								// f�rj, feles�g
	DIEDBEFOREWEDDING,		L", esk�v� el�tt meghalt",								// f�rj, feles�g, gyerek
	BORNBEFOREWEDDING,		L", esk�v� el�tt sz�letett",							// gyerek

	DIEDBEFOREHUSBANDBIRTH, L", f�rje sz�let�se el�tt meghalt",						// feles�g
	BORNAFTERHUSBANDDEATH,	L", f�rje hal�la ut�n sz�letett",						// f�rj
	
	BORNBEFOREMOTHERBIRTH,	L", anyja sz�let�se el�tt sz�letett",					// gyerek
	BORNBEFOREFATHERBIRTH,	L", apja sz�let�se el�tt sz�letett",					// gyerek

	BORNAFTERMOTHERDEATH,	L", anyja hal�la ut�n sz�letett",						// gyerek
	BORNAFTERFATHERDEATH9,	L", apja hal�la ut�n t�bb mint 9 h�napra sz�letett",	// gyerek
	
	DIEDBEFOREMOTHERBIRTH,	L", anyja sz�let�se el�tt meghalt",						// gyerek
	DIEDBEFOREFATHERBIRTH,	L", apja sz�let�se el�tt meghalt",						// gyerek
};


// CCheckFamilyDates dialog
void clearW( WIFES* w );
IMPLEMENT_DYNAMIC(CCheckFamilyDates, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDates::CCheckFamilyDates(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckFamilyDates::IDD, pParent)
	, m_maxAgeHAtWedd(70)
	, m_minAgeHAtWedd(18)
	, m_maxAgeWAtWedd(60)
	, m_minAgeWAtWedd(14)
	, m_maxDiffFC(70)
	, m_minDiffFC(18)
	, m_minDiffMC(14)
	, m_maxDiffMC(60)
	, m_maxLifespan(110)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;
	m_recordset3	= new CSqliteDBRecordSet;
	m_recordset4	= new CSqliteDBRecordSet;

	m_rgb[0] = RGB( 255, 105, 180 );
	m_rgb[1] = RGB( 127, 255, 212 );
	m_rgb[2] = RGB( 255, 255, 0 );
	m_rgb[3] = RGB( 0, 191, 255 );
	m_rgb[4] = RGB( 173, 255, 47 );
	m_rgb[5] = RGB( 216,191,216 );
	m_rgb[6] = RGB( 255, 105,180 );
	m_rgb[7] = RGB( 255, 255, 224 );
	m_rgb[8] = RGB( 0,255,255 );
	m_rgb[9] = RGB( 211,211,211 );

	m_info = L"\
Az adatb�zisban a sz�let�s, az elhal�loz�s �s az esk�v�k d�tumait tartjuk nyilv�n. Ezek egy-egy csal�don bel�li \
ellentmod�sait keress�k, amik nyilv�nval�an valamilyen adathib�ra h�vj�k fel a figyelm�nket.\r\n\
\r\n\
Az al�bbi d�tumokat ellen�rizz�k:\r\n\
\r\n\
- az emberek �lettartama\r\n\
- h�zast�rsak kork�l�nbs�ge\r\n\
- f�rj �letkora az esk�v�n\r\n\
- feles�g �letkora az esk�v�n\r\n\
- apa �s gyermeke kork�l�nbs�ge\r\n\
- anya �s gyermeke kork�l�nbs�ge\r\n\
- gyerek anyja hal�la el�tt sz�letett\r\n\
- gyerek 9 h�nappal apja hal�la el�tt sz�letett\r\n\
\r\n\
A d�tum k�l�nbs�gekre hat�r�rt�keket �ll�thatunk be az ind�t�skor megjelen� param�ter ablakban.\n\r\
Term�szetesen csak azokat a d�tumokat ellen�rzi a program, amelyek meg vannak adva �s helyes form�tum�ak \
( 2021.02.07 - 2021.02. . 2021.02 - 2021 )\r\n\
\r\n\
Minden embern�l a r� vonatkoz� d�tum hib�t jelz� �zenetet list�zunk az utols� oszlopban.\
Csak azokat a csal�dokat list�zzuk, amelyekben valamely d�tumok k�z�tt �sszef�rhetetlens�get �llap�tott meg a program.\n\r\
A list�n a csal�dok z�ld sz�n� sorokal vannak elv�lasztva. A csal�d els� sor�ban az apa �ll k�k sz�nnel, �t k�veti az \
feles�g piros sz�nnel, majd a gyerekeik feket�vel. Ha t�bb feles�ge volt az ap�nak, akkor k�vetkezik a k�vetkez� \
feles�g a gyerekeivel, �s �gy tov�bb. Az ellentmond� d�tumok s�rga h�tt�r sz�nnel vannak jel�lve\r\n\
\r\n\
Egy sorra kattintva jobb eg�rgombban, egy leg�rd�l� men�r�l v�laszthatunk olyan funkci�kat, amelyekkel megn�zhetj�k a \
bementi ga.html f�jlt ill. szerkeszthetj�k, jav�thatjuk a hib�s adatokat ak�r a bementi html f�jlban, ak�r az adatb�zisban.\r\n\
\r\n\
";

	m_infoFamilies = L"\
A f�rjek sz�rk�k, a feles�gek s�rg�k, a gyerekek z�ld sz�n�ek.\
";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDates::~CCheckFamilyDates()
{
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
	DDX_Control(pDX, IDC_STATIC_KERESS, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
	//  DDX_Control(pDX, IDC_RICHEDIT, m_captionCtrl);
	DDX_Control(pDX, IDC_RICHEDIT, captionCtrl);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFamilyDates, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_KERESS, &CCheckFamilyDates::OnClickedKeress)
	ON_STN_CLICKED(IDC_NEXT, &CCheckFamilyDates::OnClickedNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckFamilyDates::OnCustomdrawList)
	ON_COMMAND(ID_INFO, &CCheckFamilyDates::OnInfo)
	ON_COMMAND(ID_INFO_FAMILIES, &CCheckFamilyDates::OnInfoFamilies)

// IDR_DROPDOWN_HTML funkci�k	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckFamilyDates::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckFamilyDates::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckFamilyDates::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckFamilyDates::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CCheckFamilyDates::OnDbEdit)
	ON_COMMAND(ID_EDIT_3GENERATIONS, &CCheckFamilyDates::On3Generations )
	ON_COMMAND(ID_EDIT_DATABASE, &CCheckFamilyDates::OnEditDatabase )
	ON_COMMAND(ID_HTML_CHILDREN, &CCheckFamilyDates::OnHtmlChildren)

	ON_COMMAND(ID_PARAMETERS, &CCheckFamilyDates::OnParameters)
	
	ON_COMMAND(ID_FILTER_FATHER9, &CCheckFamilyDates::OnBornAfterFatherDeath9)
	ON_COMMAND(ID_FILTER_UNFILTERED, &CCheckFamilyDates::OnFilterUnfiltered)
	ON_COMMAND(ID_FILTER_MOTHERAGE, &CCheckFamilyDates::OnFilterMotherage)
	ON_COMMAND(ID_FILTER_BEFOREFATHER, &CCheckFamilyDates::OnFilterBeforefather)
	ON_COMMAND(ID_FILTER_BIRTHDEATH, &CCheckFamilyDates::OnDeathBirth)
	ON_COMMAND(ID_FILTER_BIRTH, &CCheckFamilyDates::OnFilterBirth)
	ON_COMMAND(ID_FILTER_DEATH, &CCheckFamilyDates::OnFilterDeath)
	ON_COMMAND(ID_FILTER_WEDDING, &CCheckFamilyDates::OnFilterWedding)
	ON_COMMAND(ID_BORNAFTERHDEATH, &CCheckFamilyDates::OnBornAftreHusbandDeath)
	ON_COMMAND(ID_BORNBEFORE_MBIRTH, &CCheckFamilyDates::OnBornBeforeMotherBirth)
	ON_COMMAND(ID_DIEDBEFORE_MBIRTH, &CCheckFamilyDates::OnDiedBeforeMotherBirth)
	ON_COMMAND(ID_BORNAFTERMOTHERDEATH, &CCheckFamilyDates::OnBornaftermotherdeath)
	ON_COMMAND(ID_LIFESPAN, &CCheckFamilyDates::OnLifespan)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	SetWindowTextW(theApp.m_caption);

	captionCtrl.GetSelectionCharFormat(cf);
	cf.cbSize = sizeof(cf);
	cf.dwMask = CFM_BOLD | CFM_COLOR | CFM_SIZE;
	cf.dwEffects = CFE_BOLD;
	cf.crTextColor = LIGHTRED;
	cf.yHeight = 300;

	if( !m_all )
	{
		m_title = L"Csal�dok, amelyekben valamelyik d�tumban ellentmond�s van";
		CCheckFamilyDatesStart dlg;

		dlg.m_info = m_info;
		theApp.m_pszAppName = _tcsdup( L"Csal�don bel�li d�tumok vizsg�lata" );

		if( dlg.DoModal() == IDCANCEL )
		{
			CDialog::OnCancel();
			return false; 
		}
		m_maxLifespan		= dlg.m_maxLifespan;
		m_maxAgeHAtWedd		= dlg.m_maxAgeHAtWedd;
		m_minAgeHAtWedd		= dlg.m_minAgeHAtWedd;
		m_maxAgeWAtWedd		= dlg.m_maxAgeWAtWedd;
		m_minAgeWAtWedd		= dlg.m_minAgeWAtWedd;
		m_maxDiffFC			= dlg.m_maxDiffFC;
		m_minDiffFC			= dlg.m_minDiffFC;
		m_minDiffMC			= dlg.m_minDiffMC;
		m_maxDiffMC			= dlg.m_maxDiffMC;
	}
	else
	{
		m_title = L"Csal�dok"; 
		SetMenu(NULL);
		::DestroyMenu(GetMenu()->GetSafeHmenu());
		CMenu menu;
		menu.LoadMenuW(IDR_TABLE_FAMILIES );
		SetMenu(&menu);
	}



	CString caption;
	caption = L"Csal�dok: apa: apa �letkora esk�v�n ill. a gyerek sz�let�sekor  || anya: anya �letkora az esk�v�n ill. a gyerek sz�let�sekor"; 
	richText(caption);

	colorKeres.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );


	if( m_all )
		createColumnsAll();
	else
		createColumns();
	
	collectFamily();
	

	if( !vList.size() && !m_all )
	{
		AfxMessageBox( L"Nincsenek ellentmond� d�tumok\nvagy hat��r�rt�ken k�v�li d�tumok!", MB_ICONINFORMATION );
		OnCancel();
	}
	else
	{
		OnFilterUnfiltered();
		ShowWindow( SW_MAXIMIZE );
	}
	return TRUE; 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::richText( CString caption )
{
	captionCtrl.SetSel(0, -1);
	captionCtrl.Clear();
	captionCtrl.SetWindowTextW( caption );
	captionCtrl.SetSel(0, -1);
	captionCtrl.SetSelectionCharFormat(cf);
	captionCtrl.SetBackgroundColor(false, RGB(240, 240, 240));
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::createColumns()
{

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TYPE,		L"type",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	if( theApp.m_inputMode == GAHTML )
		m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	else
		m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( L_WEDDING,	L"esk�v�",		LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_INDEX,		L"ix",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"n�v",			LVCFMT_LEFT,	180,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"sz�let�s",	LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH,		L"hal�l",		LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_AGE,			L"kor",			LVCFMT_RIGHT,	 35,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_APA,			L"apa",			LVCFMT_RIGHT,	 35,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_ANYA,		L"anya",		LVCFMT_RIGHT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_YELLOW,		L"yellow",		LVCFMT_LEFT,	 10,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( L_MESSAGE,		L"hiba",		LVCFMT_LEFT,	500,-1,COL_TEXT);

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::createColumnsAll()
{

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( LL_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LL_TYPE,		L"type",		LVCFMT_RIGHT,	 30,-1,COL_NUM);
	if( theApp.m_inputMode == GAHTML )
		m_ListCtrl.InsertColumn( LL_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	else
		m_ListCtrl.InsertColumn( LL_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_HIDDEN);
	m_ListCtrl.InsertColumn( LL_WEDDING,	L"esk�v�",		LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_INDEX,		L"ix",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LL_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( LL_NAME,		L"n�v",			LVCFMT_LEFT,	180,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( LL_BIRTHP,		L"sz�let�s",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_BIRTH,		L"",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_DEATHP,		L"hal�l",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_DEATH,		L"",			LVCFMT_LEFT,	 70,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_AGE,		L"kor",			LVCFMT_RIGHT,	 35,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_APA,		L"apa",			LVCFMT_RIGHT,	 35,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_ANYA,		L"anya",		LVCFMT_RIGHT,	 40,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( LL_COMMENT,	L"le�r�s",		LVCFMT_LEFT,	950,-1,COL_TEXT);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////// K E R E S � S /////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message==WM_KEYDOWN)
	{
		if( pMsg->wParam == VK_RETURN )
		{
			keress(0);
			return true;			// mert az als� return-re debug m�dban hib�t jelez
		}
	}
	return CDialogEx::PreTranslateMessage(pMsg);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedKeress()
{
	keress( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedNext()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	theApp.keress( search, &m_ListCtrl, L_NAME, start, true);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckFamilyDates::collectFamily()
{
	wndP.Create( NULL, L"Csal�dok lek�rdez�se..." );
	wndP.GoModal();

	vList.clear();
	m_cnt = 1;

	if( m_all )
		m_command =L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, comment, birth_place, death_place FROM people WHERE sex_id=1 ORDER BY last_name, first_name";
	else
		m_command =L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, comment FROM people WHERE sex_id=1 ORDER BY last_name, first_name";
	if( !query( m_command ) ) return false;

	
	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	int z;
	UINT j;

	
	CString name;
	bool push;

	// f�rjek ciklusa
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
//	for( UINT i = 0; i < 1000; ++i, m_recordset->MoveNext() )
	{
		h.rowid = m_recordset->GetFieldString( X_ROWID );

		m_command.Format( L"SELECT rowid,* FROM marriages WHERE husband_id='%s' ORDER by whichWife", h.rowid );
		if( !query1( m_command ) ) break;
		h.numOfSpouses	= m_recordset1->RecordsCount();
		h.linenumber	= m_recordset->GetFieldString( X_LINENUMBER );
		h.name.Format( L"%s %s", m_recordset->GetFieldString( X_LAST_NAME), getFirstWord( m_recordset->GetFieldString( X_FIRST_NAME) ) );
		h.birth			= m_recordset->GetFieldString( X_BIRTH );
		h.death			= m_recordset->GetFieldString( X_DEATH );

		h.birthC = checkDate( h.birth );
		h.deathC = checkDate( h.death );
		h.message.Empty();
		h.age.Empty();
		h.yellow = 0;

		if( m_all )
		{

			h.birthP = m_recordset->GetFieldString( X_BIRTHP );
			h.deathP = m_recordset->GetFieldString( X_DEATHP );
			h.comment		= m_recordset->GetFieldString( X_COMMENT ); 
		}
	
		// h�zass�gok lek�rdez�se
		vWifes.clear();
		for( j = 0; j < m_recordset1->RecordsCount() ; ++j, m_recordset1->MoveNext() )
		{
			w.rowidH	= h.rowid;
			w.rowid		= m_recordset1->GetFieldString( DBM_WIFE_ID );

			w.wedding	= m_recordset1->GetFieldString( DBM_DATE );
			w.weddingC = checkDate( w.wedding );
			// feles�g lek�rdez�se
			if( m_all )
				m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, comment, birth_place, death_place FROM people WHERE rowid='%s'", w.rowid );
			else
				m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, comment FROM people WHERE rowid='%s'", w.rowid );
			if( !query3( m_command ) ) break;

			w.linenumber	= m_recordset3->GetFieldString( Y_LINENUMBER );
			w.name.Format( L"%s %s", m_recordset3->GetFieldString( Y_LAST_NAME), getFirstWord( m_recordset3->GetFieldString( Y_FIRST_NAME) ) );
			w.birth			= m_recordset3->GetFieldString( Y_BIRTH );
			w.death			= m_recordset3->GetFieldString( Y_DEATH );

			if( m_all )
			{
				w.birthP	= m_recordset3->GetFieldString( Y_BIRTHP );
				w.deathP	= m_recordset3->GetFieldString( Y_DEATHP );
				w.comment	= m_recordset3->GetFieldString( Y_COMMENT ); 
			}


			w.birthC = checkDate( w.birth );
			w.deathC = checkDate( w.death );
			w.age.Empty();

			w.motherOrder	= j+1;
			w.message.Empty();
			if (m_all)
				w.yellow = 1;
			else
				w.yellow = 0 ;
			vWifes.push_back( w );
		}
		if(  j < m_recordset1->RecordsCount() ) break;  // hiba t�rt�nt!!!
		if( !vWifes.size() )
		{
			clearW(&w);
			w.name = L"ismeretlen";
			vWifes.push_back( w );
		}
		//goto cont;

		//gyerekek lek�rdez�se
		if( m_all )
			m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, mother_id, parentIndex, comment, birth_place, death_place FROM people WHERE father_id ='%s' ORDER BY parentIndex", h.rowid );
		else
			m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, mother_id, parentIndex, comment FROM people WHERE father_id ='%s' ORDER BY parentIndex", h.rowid );
		if( !query2( m_command ) ) break;
		
		vChildren.clear();
		for( UINT k = 0; k < m_recordset2->RecordsCount() ; ++k, m_recordset2->MoveNext() )
		{
			c.name.Format( L"%s %s", m_recordset2->GetFieldString( Z_LAST_NAME), getFirstWord( m_recordset2->GetFieldString( Z_FIRST_NAME) ) );

			c.linenumber	= m_recordset2->GetFieldString( Z_LINENUMBER );
			c.rowid			= m_recordset2->GetFieldString( Z_ROWID );
			c.birth			= m_recordset2->GetFieldString( Z_BIRTH );
			c.death			= m_recordset2->GetFieldString( Z_DEATH );

			if( m_all )
			{
				c.birthP	= m_recordset2->GetFieldString( Z_BIRTHP );
				c.deathP	= m_recordset2->GetFieldString( Z_DEATHP );
				c.comment	= m_recordset2->GetFieldString( Z_COMMENT ); 
			}

			c.birthC = checkDate( c.birth );
			c.deathC = checkDate( c.death );

			c.father_id		= h.rowid;
			c.mother_id		= m_recordset2->GetFieldString( Z_MOTHER_ID );
			c.mother_index	= _wtoi( m_recordset2->GetFieldString( Z_MOTHERINDEX ) );
			c.message.Empty();

			c.age.Empty();
			c.yellow = 0;
			vChildren.push_back( c );
		}

		if( vWifes.at(0).name == L"ismeretlen" && !vChildren.size() ) goto cont;

		if( !m_all )
			checkFamily();
		else
			printFamilyAll();

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::checkFamily()
{
	WIFES		w;
	CHILDREN	c;
	UINT i;
	int	mx;
	int diff;
	int z;
	int ret;
	bool hiba = false;
	bool month9 = false;

	// sz�let�s-hal�l, �letkor
	if( ( ret = diffD( h.deathC, h.birthC, &diff ) ) != INT_MAX )
	{
		h.age.Format( L"%d", diff );
		if( ret < 0 )
		{
			h.message = err[DEATHBIRTH].msg;
			h.yellow |= 1 << L_BIRTH;
			h.yellow |= 1 << L_DEATH;
			h.yellow |= 1 << L_AGE;
			hiba = true;
		}
		if( diff > m_maxLifespan )
		{
			str.Format( L", %d �vesen halt meg", diff );
			h.message += str;
			h.yellow |= 1 << L_AGE;
			h.yellow |= 1 << L_BIRTH;
			h.yellow |= 1 << L_DEATH;
			hiba = true;
		}
	}


	for( i = 0; i < vWifes.size(); ++i )
	{
		w = vWifes.at(i);


		if( ( ret = diffD( w.deathC, w.birthC, &diff ) ) != INT_MAX )
		{
			vWifes.at(i).age.Format( L"%d", diff );
			if( ret < 0 )
			{
				w.message += err[DEATHBIRTH].msg;
				w.yellow |= 1 << L_BIRTH;
				w.yellow |= 1 << L_DEATH;
				w.yellow |= 1 << L_AGE;
				hiba = true;
			}
			if( diff > m_maxLifespan )
			{
				str.Format( L", %d �vesen halt meg", diff );
				w.yellow |= 1 << L_AGE;
				w.yellow |= 1 << L_BIRTH;
				w.yellow |= 1 << L_DEATH;
				w.message += str;
				hiba = true;
			}
		}

		if( ( ret = diffD( w.weddingC, w.birthC, &diff ) ) != INT_MAX )
		{
			vWifes.at(i).motherAge.Format( L"%d", diff );
			if( ret < 0 )
			{
				w.message += err[BORNAFTERWEDDING].msg;
				w.yellow |= 1 << L_WEDDING;
				w.yellow |= 1 << L_BIRTH;

				hiba = true;
			}
			else if( m_minAgeWAtWedd > diff || diff > m_maxAgeWAtWedd )
			{
				str.Format( L", h�zass�gk�t�skor %d �ves volt", diff ); 
				w.yellow |= 1 << L_WEDDING;
				w.yellow |= 1 << L_BIRTH;
				w.message += str;
				hiba = true;
			}
		}
		if( ( ret = diffD( w.deathC, h.birthC, &diff ) ) != INT_MAX )
		{
			if( ret < 0 )
			{
				w.message += err[DIEDBEFOREHUSBANDBIRTH].msg; 
				w.yellow |= 1 << L_DEATH;
				h.yellow |= 1 << L_BIRTH;
				hiba = true;
			}
		}
		if( ( ret = diffD( h.deathC, w.birthC, &diff ) ) != INT_MAX )
		{
			if( ret < 0 )
			{
				w.message += err[BORNAFTERHUSBANDDEATH].msg; 
				w.yellow |= 1 << L_BIRTH;
				h.yellow |= 1 << L_DEATH;
				hiba = true;
			}
		}
		if( ( ret = diffD( h.deathC, w.weddingC, &diff ) ) != INT_MAX )
		{
			if( ret < 0 )
			{
				h.message += err[DIEDBEFOREWEDDING].msg;
				w.yellow |= 1 << L_WEDDING;
				h.yellow |= 1 << L_DEATH;
				hiba = true;
			}
		}
		if( ( ret = diffD( w.deathC, w.weddingC, &diff ) ) != INT_MAX )
		{
			if( ret < 0 )
			{
				w.message += err[DIEDBEFOREWEDDING].msg; 
				w.yellow |= 1 << L_WEDDING;
				w.yellow |= 1 << L_DEATH;
				hiba = true;
			}
		}
		if( ( ret = diffD( w.weddingC, h.birthC, &diff ) ) != INT_MAX )
		{
			vWifes.at(i).fatherAge.Format( L"%d", diff );
			if( ret < 0 )
			{
				h.message += err[BORNAFTERWEDDING].msg;
				w.yellow |= 1 << L_WEDDING;
				h.yellow |= 1 << L_BIRTH;
				hiba = true;
			}
			else if( m_minAgeHAtWedd > diff || diff > m_maxAgeHAtWedd )
			{
				str.Format( L", h�zass�gk�t�skor %d �ves volt", diff ); 
				w.yellow |= 1 << L_WEDDING;
				h.yellow |= 1 << L_BIRTH;

				h.message += str;
				hiba = true;
			}
		}
		
		// gyerekek sz�let�si d�tum�nak �sszehasonl�t�sa anya sz�let�si d�tum�val �s annak h�zass�gk�t�si d�tum�val
		for( UINT j = 0; j < vChildren.size(); ++j )
		{
			c = vChildren.at(j);
			if( c.mother_index != i+1 ) continue;


			if( ( ret = diffD( c.deathC, c.birthC, &diff ) ) != INT_MAX )
			{
				vChildren.at(j).age.Format( L"%d", diff );
				if( ret < 0 )
				{
					c.message = err[DEATHBIRTH].msg;
					c.yellow |= 1 << L_BIRTH;
					c.yellow |= 1 << L_DEATH;
					hiba = true;
				}
				else if( diff > m_maxLifespan )
				{
					str.Format( L", %d �vesen halt meg", diff );
					c.yellow |= 1 << L_BIRTH;
					c.yellow |= 1 << L_DEATH;
					c.yellow |= 1 << L_AGE;
					c.message += str;
					hiba = true;
				}
			}

			if( ( ret = diffD( c.birthC, w.weddingC, &diff ) ) != INT_MAX )
			{
				if( ret < 0 )
				{
					c.message += err[BORNBEFOREWEDDING].msg;
					c.yellow |= 1 << L_BIRTH;
					w.yellow |= 1 << L_WEDDING;
					hiba = true;
				}
			}

			if( ( ret = diffD( c.deathC, w.weddingC, &diff ) ) != INT_MAX )
			{
				if( ret < 0 )
				{
					c.message += err[DIEDBEFOREWEDDING].msg;
					c.yellow |= 1 << L_DEATH;
					w.yellow |= 1 << L_WEDDING;
					hiba = true;
				}
			}

			if( ( ret = diffD( w.deathC, c.birthC, &diff ) ) != INT_MAX )
			{
				if( ret < 0 )
				{
					c.message += err[BORNAFTERMOTHERDEATH].msg;
					c.yellow |= 1 << L_BIRTH;
					w.yellow |= 1 << L_DEATH;
					hiba = true;
				}
			}
			if( ( ret = diffD( c.birthC, w.birthC, &diff ) ) != INT_MAX )
			{
				vChildren.at(j).motherAge.Format( L"%d", diff );					// anya �letkora a gyerek sz�let�sekor
				if( ret < 0 )
				{
					c.message += err[BORNBEFOREMOTHERBIRTH].msg;
					c.yellow |= 1 << L_BIRTH;
					w.yellow |= 1 << L_BIRTH;
					hiba = true;
				}
				else if( diff < m_minDiffMC || m_maxDiffMC < diff  )
				{
					str.Format( L", anyja %d �ves kor�ban sz�letett", diff ); 
					c.yellow |= 1 << L_BIRTH;
					w.yellow |= 1 << L_BIRTH;
					c.message += str;
					hiba = true;
				}
			}
			
			// gyerek sz�let�si d�tum �s apa hal�loz�si + 9 h�nap

			if( theApp.dateDiff( h.deathC, c.birthC, 9 ) )
			{
				c.message += err[BORNAFTERFATHERDEATH9].msg; 
				c.yellow |= 1 << L_BIRTH;
				h.yellow |= 1 << L_DEATH;

				month9 = true;
				hiba = true;
			}

			if( ( ret = diffD( c.birthC, h.birthC, &diff ) ) != INT_MAX )
			{
				vChildren.at(j).fatherAge.Format( L"%d", diff );					// apa �letkora a gyerek sz�let�sekor
				if( ret < 0 )
				{
					c.message += err[BORNBEFOREFATHERBIRTH].msg;
					c.yellow |= 1 << L_BIRTH;
					h.yellow |= 1 << L_BIRTH;
					hiba = true;
				}
				else if( ( diff < m_minDiffFC || m_maxDiffFC < diff ) && !month9 )
				{
					str.Format( L", apja %d �ves kor�ban sz�letett", diff ); 
					c.message += str;
					c.yellow |= 1 << L_BIRTH;
					h.yellow |= 1 << L_BIRTH;
					hiba = true;
				}
			}

			if( ( ret = diffD( c.deathC, w.birthC, &diff ) ) != INT_MAX )
			{
				if( ret < 0 )
				{
					c.message += err[DIEDBEFOREMOTHERBIRTH].msg;
					c.yellow |= 1 << L_DEATH;
					w.yellow |= 1 << L_BIRTH;
					hiba = true;
				}
			}
			if( ( ret = diffD( c.deathC, h.birthC, &diff ) ) != INT_MAX )
			{
				if( ret < 0 )
				{
					c.message += err[DIEDBEFOREFATHERBIRTH].msg;
					c.yellow |= 1 << L_DEATH;
					h.yellow |= 1 << L_BIRTH;
					hiba = true;
				}
			}
			vChildren.at(j).message	= c.message;
			vChildren.at(j).yellow	= c.yellow;
		}
		vWifes.at(i).message	= w.message;
		vWifes.at(i).yellow 	= w.yellow;
	}
	if( hiba )
		printFamily();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::printFamily()
{
	WIFES		w;
	CHILDREN	c;
	CString message;

	UINT j;

	m_filtered = false;
	message = L"";
	if( h.message.GetLength() > 2 ) message = h.message.Mid( 2 );
	message = L"";
	if( h.message.GetLength() > 2 ) message = h.message.Mid( 2 );  // a kezd� L" ," leszed�se
	str.Format( L"%d", m_cnt );
	push( str );
	push( L"1" );							// type
	push( h.linenumber );
	push( L"" );							// esk�v�
	push( L"" );							// index, ami szerint sz�nez
	push( h.rowid );
	push( h.name );
	push( h.birth );
	push( h.death );
	push( h.age );
	push( L"");								// apa
	push( h.motherAge );
	str.Format( L"%d", h.yellow );
	push( str );
	push( message );
	for( j = 0; j < vWifes.size(); ++j )
	{
		w = vWifes.at(j);
		message = L"";
		if( w.message.GetLength() > 2 ) 
			message = w.message.Mid( 2 );
		push( L"" );						// cnt
		push( L"2" );						// type
		push( w.linenumber );
		push( w.wedding );
		str.Format( L"%d", w.motherOrder );  // index, ami szerint sz�nez
		str.Format( L"%d", j+1 );
		push( str );
		push( w.rowid );
		push( w.name );
		push( w.birth );
		push( w.death );
		push( w.age );
		push( w.fatherAge );
		push( w.motherAge );
		str.Format( L"%d", w.yellow );
		push( str );
		push( message );
	
		for( UINT k = 0; k < vChildren.size(); ++k )
		{
			c = vChildren.at(k );
			if( c.mother_index != j + 1 ) continue;			// csak az any�hoz tartoz� gyerekekket list�zza!!!


			message = L"";
			if( c.message.GetLength() > 2 ) message = c.message.Mid( 2 );

			push( L"" );							// cnt
			push( L"3" );							// type
			push( c.linenumber );
			push( L"" );							// marriage
			str.Format( L"%d", c.mother_index );	// index, ami szerint szinez
			str.Format( L"%d", j+1 );
			push( str );
			push( c.rowid );
			push( c.name );
			push( c.birth );
			push( c.death );
			push( c.age );
			push( c.fatherAge );
			push( c.motherAge );
			str.Format( L"%d", c.yellow );
			push( str );
			push( message );
		}
	}
	emptyLine();
	++m_cnt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::printFamilyAll()
{
	WIFES		w;
	CHILDREN	c;
	CString comment;

	UINT j;
	
	str.Format( L"%d", m_cnt );
	push( str );
	push( L"1" );							// type
	push( h.linenumber );
	push( L"" );							// esk�v�
	push( L"" );							// index, ami szerint sz�nez
	push( h.rowid );
	push( h.name );
	push( h.birthP );
	push( h.birth );
	push( h.deathP );
	push( h.death );
	push( h.age );
	push( L"");								// apa
	push( h.motherAge );
	comment = h.comment;
	comment.Replace( '|','\'' );
	push( comment );

	for( j = 0; j < vWifes.size(); ++j )
	{
		w = vWifes.at(j);

		push( L"" );						// cnt
		push( L"2" );						// type
		push( w.linenumber );
		push( w.wedding );
		str.Format( L"%d", w.motherOrder );  // index, ami szerint sz�nez
		str.Format( L"%d", j+1 );
		push( str );
		push( w.rowid );
		push( w.name );
		push( w.birthP );
		push( w.birth );
		push( w.deathP );
		push( w.death );
		push( w.age );
		push( w.fatherAge );
		push( w.motherAge );
		comment = w.comment;
		comment.Replace( '|','\'' );
		push( comment );

		
		for( UINT k = 0; k < vChildren.size(); ++k )
		{
			c = vChildren.at(k );
			if( c.mother_index != j + 1 ) continue;			// csak az any�hoz tartoz� gyerekekket list�zza!!!

			push( L"" );							// cnt
			push( L"3" );							// type
			push( c.linenumber );
			push( L"" );							// marriage
			str.Format( L"%d", c.mother_index );	// index, ami szerint szinez
			str.Format( L"%d", j+1 );
			push( str );
			push( c.rowid );
			push( c.name );
			push( c.birthP );
			push( c.birth );
			push( c.deathP );
			push( c.death );
			push( c.age );
			push( c.fatherAge );
			push( c.motherAge );
			comment = c.comment;
			comment.Replace( '|','\'' );
			push( comment );
		}
	}
	emptyLineAll();
	++m_cnt;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::push( CString item )
{

	int tLen;
	TCHAR* sT;
	tLen	= item.GetLength()+1;
	sT		= new TCHAR[tLen];
	_tcscpy_s( sT, tLen, item.GetBuffer() );
	vList.push_back( sT );

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::emptyLine()
{
	for( UINT i = 0; i < L_NUMOFCOLUMNS; i++ ) push( L"" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::emptyLineAll()
{
	for( UINT i = 0; i < LL_NUMOFCOLUMNS; i++ ) push( L"" );
 }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	CString	who;
	UINT mask;
	int yellow;
	int index;

	*pResult = 0;

	switch( pLVCD->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		nItem	= pLVCD->nmcd.dwItemSpec;
		nCol	= pLVCD->iSubItem;
		mask    =  1 << nCol; 
		who = m_ListCtrl.GetItemText(nItem, L_TYPE);
		yellow	= _wtoi( m_ListCtrl.GetItemText( nItem, L_YELLOW ) );

		if( m_all )
		{
			if( _wtoi( vList.at( nItem * LL_NUMOFCOLUMNS + LL_TYPE ))  )
			{
				/*
				index = _wtoi( vList.at( nItem * LL_NUMOFCOLUMNS + LL_INDEX ) );
				pLVCD->clrTextBk = m_rgb[index];
				*/
				if( who == MAN )
					pLVCD->clrTextBk = GRAY;		// f�rj
				else if (who == WOMAN )
				{
					index = _wtoi(vList.at(nItem * LL_NUMOFCOLUMNS + LL_INDEX));
//					pLVCD->clrTextBk = m_rgb[index+1];
					pLVCD->clrTextBk = YELLOW;		// feles�g
				}
				else
					pLVCD->clrTextBk = RGB(0, 255, 205);	// gyerek

			}
		}
		else
		{
			if( nCol == L_NAME )
			{
				if( who == MAN )
					pLVCD->clrText	 = RGB( 0,0,255);		// f�rj
				else if (who == WOMAN)
					pLVCD->clrText	 = RGB( 255,0,0 );		// feles�g
			}
			else if( yellow & mask )
				pLVCD->clrTextBk = YELLOW;					// gyn�s d�tumok

			if( who.IsEmpty() )									// �res sor
				pLVCD->clrTextBk = RGB( 0,255,205);
		}


		*pResult = CDRF_DODEFAULT;
		break;
	}
}
/*
void CCheckFamilyDates::OnCustomdrawList(NMHDR *pNMHDR, LRESULT *pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>( pNMHDR );
	
	int nItem;
	int nCol;
	int index;
	int type;
	UINT mask;

	*pResult = 0;

	switch( pLVCD->nmcd.dwDrawStage )
	{
	case CDDS_PREPAINT:
		*pResult = CDRF_NOTIFYITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT:
		*pResult = CDRF_NOTIFYSUBITEMDRAW;
		break;
	case CDDS_ITEMPREPAINT|CDDS_SUBITEM:
		nItem	= pLVCD->nmcd.dwItemSpec;
		nCol	= pLVCD->iSubItem;
		mask	= 1 << nCol;
		if( m_all )
		{
			if( _wtoi( vList.at( nItem * LL_NUMOFCOLUMNS + LL_TYPE ))  )
			{
				index = _wtoi( vList.at( nItem * LL_NUMOFCOLUMNS + LL_INDEX ) );
				pLVCD->clrTextBk = m_rgb[index];
			}
		}
		else
		{
			if( m_filtered )
			{
				if( _wtoi( vListFiltered.at( nItem * L_NUMOFCOLUMNS + L_TYPE ))  )
				{
					index = _wtoi( vListFiltered.at( nItem * L_NUMOFCOLUMNS + L_INDEX ) );
					pLVCD->clrTextBk = m_rgb[index];
				}
			}
			else
			{
				if( _wtoi( vList.at( nItem * L_NUMOFCOLUMNS + L_TYPE ))  )
				{
					index = _wtoi( vList.at( nItem * L_NUMOFCOLUMNS + L_INDEX ) );
					pLVCD->clrTextBk = m_rgb[index];
				}
			}

		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnInfo()
{
	AfxMessageBox( m_info, MB_ICONINFORMATION );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnInfoFamilies()
{
	AfxMessageBox( m_infoFamilies, MB_ICONINFORMATION );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnParameters()
{
	CString str;
	str.Format( L"A lista az al�bbi param�ter �rt�kekkel k�sz�lt:\r\n\r\n\
Maxim�lis �lettartam:\t\t\t\t%3d �v\r\n\
Az apa �s gyermeke k�z�tti kork�l�nbs�g hat�rai:\t%2d-%3d\r\n\
Az anya �s gyermeke k�z�tti kork�l�nbs�g hat�rai:\t%2d-%3d\r\n\
A v�leg�ny �letkor�nak hat�rai az esk�v�n:\t\t%2d-%3d\r\n\
A menyasszony �letkor�nak hat�rai az esk�v�n:\t%2d-%3d",\
	m_maxLifespan,\
	m_minDiffFC,\
	m_maxDiffFC,\
	m_minDiffMC,\
	m_maxDiffMC,\
	m_minAgeHAtWedd,\
	m_maxAgeHAtWedd,\
	m_minAgeWAtWedd,\
	m_maxAgeWAtWedd\
);
	AfxMessageBox( str, MB_ICONINFORMATION );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckFamilyDates:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	if( theApp.m_inputMode == GEDCOM )
		MENU_IDR = IDR_DROPDOWN_FAMILIES;
	else if( theApp.m_inputMode == GAHTML )
		MENU_IDR = IDR_DROPDOWN_HTML;
	else if( theApp.m_inputMode == MANUAL )
		MENU_IDR = IDR_DROPDOWN_FAMILIES	;

	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);
		if(m_ListCtrl.GetNextItem(-1,LVNI_SELECTED) < 0 )
		{
			pPopup->EnableMenuItem(ID_HTML_EDIT, MF_BYCOMMAND | MF_GRAYED);
		}
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlEditLines()
{
	CString title;
	int selectedCount	= m_ListCtrl.GetSelectedCount();
	int nItem			= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( selectedCount == 1 )
		title.Format( L"%s a ga.html f�jlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijel�lt sor a ga.html f�jlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( theApp.m_htmlPathName, lineNumber );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlNotepadParents()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, L_ROWID );

	theApp.HtmlNotepadParents( rowid );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlFatherAndSiblings()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s sz�lei �s testv�rei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelatives dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::On3Generations()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );

	CRelatives dlg;
	dlg.m_rowid = rowid;

//	ShowWindow( SW_HIDE );
	dlg.DoModal();
//	ShowWindow( SW_RESTORE );

}	
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlChildren()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);

	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CEditHtmlLines dlg;
	dlg.m_title.Format( L"%s �s gyermekei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_CHILDREN";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnEditDatabase()
{
	int nItem	= m_ListCtrl.GetNextItem(-1, LVNI_SELECTED); 
	if( nItem == -1 ) return;

	CString name;
 	name = m_ListCtrl.GetItemText( nItem, L_NAME );

	CEditPeople dlg;

	dlg.m_rowid	= m_ListCtrl.GetItemText( nItem, L_ROWID );
	dlg.m_caption.Format( L"%s szerkeszt�se", name );

//	ShowWindow( SW_HIDE );
	dlg.DoModal();	
//	ShowWindow( SW_RESTORE );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::query( CString command )
{
	if( m_recordset->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::query1( CString command )
{
	if( m_recordset1->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::query3( CString command )
{
	if( m_recordset3->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::query4( CString command )
{
	if( m_recordset4->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset4->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Elfogadott form�tum:
// az �vnek 1000 �s az aktu�lis �v k�z� kell esni

// 1944
// 1944.12.
// 1944.12.17

// hib�s d�tum eset�n �res stringet ad vissza, j� d�trum eset�n a d�tumot
CString CCheckFamilyDates::checkDate( CString datum)
{
	CString ures(L"");

	datum.Trim();
	int length = datum.GetLength();
	if( length < 4 ) return ures;

	int	year = _wtoi( datum.Left( 4 ) );
	if( year < 1000 ) return ures;

// �v ellen�rz�se
	char	sTime[50];
	time_t	now = time(0);
	ctime_s( sTime, sizeof( sTime ), &now);
	CString st;
	st = sTime;
	int	yearCurr = _wtoi( st.Right( 6 ) );		// "\n" miatt 6 karaktert vesz le, ami konverzin�l ugyis lemarad
	if( year > yearCurr ) return ures;

	// �v ut�ni '.' ellen�rz�se
	if( length == 4 ) return datum;	// csak 2001 a d�tum

// h�nap ellen�rz�se a 2019.11 
	if( length <= 7 ) return ures;
	if( datum.GetAt(4) != '.' ) return ures;
	if( !iswdigit( datum.GetAt( 5 ) ) ) 
		return ures;
	if( !iswdigit( datum.GetAt( 6 ) ) ) 
		return ures;
	int	month = _wtoi( datum.Mid( 5, 2 ) );
	if( month < 1 || month > 12 ) return ures;
	if( length == 8  ) return datum;	// csak 2019.11.  a d�tum

// nap ellen�rz�se 2019.11.12
	if( datum.GetAt( 7 ) != '.' ) return ures;
	if( length < 10 ) return ures;

	if( !iswdigit( datum.GetAt( 8 ) ) ) 
		return ures;
	if( !iswdigit( datum.GetAt( 9 ) ) ) 
		return ures;
	int	day = _wtoi( datum.Mid( 8, 2 ) );
	if( day < 1 ) return ures;
	if( month == 2 && day > 29 ) return false;
	if(( month == 4 || month == 6 || month == 9 || month == 11 ) && day > 30 ) return ures;
	if( day > 31 ) return ures; 
	if( length > 10 ) return ures;  // d�tum ut�n m�g van valami
	return datum;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void clearW( WIFES* w )
{
	w->age.Empty();
	w->birth.Empty();
	w->birthC.Empty();
	w->birthP.Empty();
	w->comment.Empty();
	w->death.Empty();
	w->deathC.Empty();
	w->deathP.Empty();
	w->fatherAge.Empty();
	w->linenumber.Empty();
	w->message.Empty();
	w->motherAge.Empty();
	w->motherOrder = 0;
	w->name.Empty();
	w->rowid.Empty();
	w->rowidH.Empty();
	w->wedding.Empty();
	w->weddingC.Empty();
	w->yellow = 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// F I L T E R  //////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnFilterUnfiltered()
{
	richText(m_title);
	m_filtered = false;
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.DetachDataset();
	m_ListCtrl.SetItemCountEx( vList.size() + 1  );
	m_ListCtrl.AttachDataset( &vList );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnDeathBirth()
{
	filter( L"Sz�let�s�k el�tt meghaltak", L"<" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnBornAftreHusbandDeath()
{
	filter( L"F�rje hal�la ut�n sz�letett feles�gek csal�djai", L"f�rje hal�la" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnBornBeforeMotherBirth()
{
	str = err[BORNBEFOREMOTHERBIRTH].msg;
	filter( L"Anyja sz�let�se el�tt sz�lettek", str.Mid( 2 ) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnDiedBeforeMotherBirth()
{
	str = err[DIEDBEFOREMOTHERBIRTH].msg;
	filter( L"Anyjuk sz�let�se el�tt meghaltak", str.Mid( 2 ) );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnBornAfterFatherDeath9()
{
	filter( L"Apjuk hal�la ut�n t�bb mint 9 h�nappal sz�letettek", L"9 h" );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CCheckFamilyDates::OnFilterBeforemother()
//{
//	filter( L"Anyjuk sz�let�se el�tt sz�letettek vagy meghaltak", L"anyja sz�let�se" );
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnFilterBeforefather()
{
	filter( L"Apjuk sz�let�se el�tt sz�letettek vagy meghaltak", L"apja sz�let�se" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnBornaftermotherdeath()
{
	filter( L"Anyjuk hal�la ut�n sz�lettek", L"anyja hal�la ut�n" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnFilterBirth()
{
	filter( L"Sz�let�si d�tumokkal ellentmond�sban van", L"sz�let�se" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnFilterDeath()
{
	filter( L"Hal�loz�si d�tumokkal ellentmond�sban van", L"hal�la" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnFilterWedding()
{
	filter( L"Esk�v� d�tum�val ellentmond�sban van", L"esk�v�" );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnFilterMotherage()
{
	filter( L"Anyjuk X �ves kor�ban sz�lettek", L"�ves kor�ban" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::filter( CString caption, CString azon )
{
	vListFiltered.clear();
	int pos;

	CProgressWnd wndProgress(NULL, L"Folyik a keres�s.." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vList.size()/L_NUMOFCOLUMNS );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	for( int i = L_MESSAGE; i < vList.size() - L_NUMOFCOLUMNS - 1; i += L_NUMOFCOLUMNS )
	{
		str = vList.at(i);
		if( ( pos = str.Find( azon ) ) != -1 )
		{
			i = pushBlock( i, L_MESSAGE  );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	m_filtered = true;
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.DetachDataset();
	m_ListCtrl.SetItemCountEx( vListFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vListFiltered );
	richText(caption);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnLifespan()
{
/*
	CCheckParam dlg;

	dlg.m_title		= L"Azokat az csal�dokat list�zzuk, amelyekben van olyan ember, akinek az �letkora nagyobb az el��rtn�l.";
	dlg.m_function1 = L"�rd el� a kort!";
	dlg.m_value		= 100;

	if( dlg.DoModal() == IDCANCEL )
	{
		CDialog::OnCancel();
	}

	m_lifespan = dlg.m_value;
*/
	vListFiltered.clear();
	int pos;

	CProgressWnd wndProgress(NULL, L"Folyik a keres�s.." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, vList.size()/L_NUMOFCOLUMNS );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	int age;
	for( int i = L_AGE; i < vList.size() - L_NUMOFCOLUMNS - 1; i += L_NUMOFCOLUMNS )
	{
		age = _wtoi( vList.at(i) );
		if( age >= m_maxLifespan )
		{
			i = pushBlock( i, L_AGE );
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	m_filtered = true;
	m_ListCtrl.DeleteAllItems();
	m_ListCtrl.DetachDataset();
	m_ListCtrl.SetItemCountEx( vListFiltered.size() + 1  );
	m_ListCtrl.AttachDataset( &vListFiltered );
	str.Format( L"%d �vn�l id�sebb emeberek", m_maxLifespan );
	richText( str);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// i az L_REF oszlopra mutat
int CCheckFamilyDates::pushBlock( UINT i, int L_REF )
{
	int typeIndex = i  - L_REF + L_TYPE;  // ugyanannak a sornak az L_TYPE oszlop�nak indexe
	int z;

	while( typeIndex > 0 && typeIndex < vList.size() && (str = vList.at(typeIndex)) != L"1" )		// visszamegy az ap�ig
		typeIndex -= L_NUMOFCOLUMNS;
	int firstIndex = typeIndex - 1;			// a megtal�lt "f�rj" sor elej�re mutet

	while( typeIndex > 0 && typeIndex < vList.size() && ( str = vList.at( typeIndex ) ) != L"" )						// lemegy az �res sorig
		typeIndex += L_NUMOFCOLUMNS;
	int lastIndex = typeIndex -1 + L_NUMOFCOLUMNS;						// az �res sor utol� oszlop�ra mutat

	if( firstIndex < 0 || lastIndex > vList.size() -1 )
		z = 1;
	for( int j = firstIndex; j < lastIndex; ++j )
	{
		vListFiltered.push_back( vList.at(j) );
	}		
//	return(lastIndex - 1 );	// visszadja, a folytat�shoz az indexet, ami a k�vetkez� L_REF-re mutat
	return( lastIndex - L_NUMOFCOLUMNS + L_REF );
}



