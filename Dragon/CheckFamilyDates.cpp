// CCheckFamilyDates.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "CheckFamilyDates.h"
#include "utilities.h"
#include "html_EditLines.h"
#include "Relations.h"
#include "FamilyDatesStart.h"

// SELECT  FROM people father
enum
{
	X_ROWID = 0,
	X_LINENUMBER,
	X_FIRST_NAME,
	X_LAST_NAME,
	X_BIRTH,
	X_DEATH,
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
};

// m_ListCtrl oszlopok
enum
{
	L_CNT = 0,
	L_TYPE,
	L_LINENUMBER,
	L_MARRIAGE,
	L_INDEX,
	L_ROWID,
	L_NAME,
	L_BIRTH,
	L_DEATH,
	L_AGE,
	L_DIFFH,
	L_DIFFW,
	L_COMMENT,
	L_NUMOFCOLUMNS
};



// CCheckFamilyDates dialog

IMPLEMENT_DYNAMIC(CCheckFamilyDates, CDialogEx)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CCheckFamilyDates::CCheckFamilyDates(CWnd* pParent /*=NULL*/)
	: CDialogEx(CCheckFamilyDates::IDD, pParent)
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

	m_colorWife = RGB( 255,0,222 );

	m_info = L"\
Az adatbázisban a születés, az elhalálozás és az esküvõk dátumait tartjuk nyilván. Ezek egy-egy családon belüli \
ellentmodásait keressük, amik nyilvánvalóan valamilyen adathibára hívják fel a figyelmünket.\r\n\
\r\n\
Az alábbi dátumokat ellenõrizzük:\r\n\
\r\n\
- az emberek élettartama\r\n\
- házastársak korkülönbsége\r\n\
- férj életkora az esküvõn\r\n\
- feleség életkora az esküvõn\r\n\
- apa és gyermeke korkülönbsége\r\n\
- anya és gyermeke korkülönbsége\r\n\
- gyerek anyja halála elõtt született\r\n\
- gyerek 9 hónappal apja halála elõtt született\r\n\
\r\n\
A dátum különbségekre határértékeket állíthatunk be az indításkor megjelenõ paraméter ablakban.\n\r\
Ebben az ablakban azt is kijelölhetjük, hogy mely családokat vizsgáljunk: gyerektelen családokat, vagy \
olyanokat, amelyekben a férjnek csak egy felesége volt, vagy olyanokat, amelyekben több felesége is volt.\n\r\
Ezt a választást azért ajánlja fel a program, mert az összes család túl nagy, nehezen kezelhetõ listát eredményezhet, \
másrészt az így megkülönbözetet családokban talált dátum hibák sokszor külünbözõ fajta adathibákra utalhatnak.\r\n\
Természetesen csak azokat a dátumokat ellenõrzi a program, amelyek meg vannak adva és helyes formátumúak \
( 2021.02.07 - 2021.02. . 2021.02 - 2021 )\r\n\
\r\n\
Minden embernél a rá vonatkozó dátum hibát jelzõ üzenetet listázunk az utolsó oszlopban.\r\n\
Csak azokat a családokat listázzuk, amelyekben valamely dátumok között összeférhetetlenséget állapított meg a program.\n\r\
A listán az elsõ sorban az apa áll halvány piros színnel, õt követi az elsõ felesége a gyerekeivel egyforma színnel, \
majd a további feleségek a gyerekeikkel más-más színnel. A családokat üres  sor választja el.\r\n\
\r\n\
Egy sorra kattintva jobb egérgombban, egy legördülõ menürõl választhatunk olyan funkciókat, amelyekkel megnézhetjük a \
bementi ga.html fájlt ill. szerkeszthetjük, javíthatjuk a hibás adatokat akár a bementi html fájlban, akár az adatbázisban.\r\n\
\r\n\
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
	DDX_Control(pDX, IDC_KERES, colorKeres);
	DDX_Control(pDX, IDC_NEXT, colorNext);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CCheckFamilyDates, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_STN_CLICKED(IDC_KERES, &CCheckFamilyDates::OnClickedKeres)
	ON_STN_CLICKED(IDC_NEXT, &CCheckFamilyDates::OnClickedNext)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_LIST, &CCheckFamilyDates::OnCustomdrawList)
	ON_COMMAND(ID_INFO, &CCheckFamilyDates::OnInfo)

// IDR_DROPDOWN_HTML funkciók	
	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CCheckFamilyDates::OnHtmlEditLines)
	ON_COMMAND(ID_HTML_NOTEPAD, &CCheckFamilyDates::OnHtmlNotepad)
	ON_COMMAND(ID_HTML_NOTEPAD_PARENTS, &CCheckFamilyDates::OnHtmlNotepadParents)
	ON_COMMAND(ID_HTML_FATHERANDSIBLINGS, &CCheckFamilyDates::OnHtmlFatherAndSiblings)
	ON_COMMAND(ID_DB_EDIT, &CCheckFamilyDates::OnDbEdit)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CCheckFamilyDates::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_ADD( IDC_CAPTION,	ES_BORDER,	ES_BORDER,	ES_BORDER,		ES_KEEPSIZE,	0 );
	EASYSIZE_INIT();

	CFamilyDatesStart dlg;

	dlg.m_info = m_info;
	theApp.m_pszAppName = _tcsdup( L"Családon belüli dátumok vizsgálata" );

	if( dlg.DoModal() == IDCANCEL )
	{
		CDialog::OnCancel();
		return false; 
	}
	m_type				= dlg.m_type;
	m_maxLifespan		= dlg.m_maxLifespan;
	m_maxAgeHAtWedd		= dlg.m_maxAgeHAtWedd;
	m_minAgeHAtWedd		= dlg.m_minAgeHAtWedd;
	m_maxAgeWAtWedd		= dlg.m_maxAgeWAtWedd;
	m_minAgeWAtWedd		= dlg.m_minAgeWAtWedd;
	m_maxDiffFC			= dlg.m_maxDiffFC;
	m_minDiffFC			= dlg.m_minDiffFC;
	m_minDiffMC			= dlg.m_minDiffMC;
	m_maxDiffMC			= dlg.m_maxDiffMC;


	CString title;
	switch( m_type )
	{
	case 0:
		title = L"Gyermektelen családok, amelyekben valamelyik dátumban ellentmondás van";
		break;
	case 1:
		title = L"Férfiak családjai, akiknek csak egy feleségük volt, és amelyekben valamelyik dátumban ellentmondás van";
		break;
	case 2:
		title = L"Családok, amelyekben a férjnek több házassága volt, és a dátumok valamelyike ellentmondásos"; 
		break;
	}

	CString caption;
	caption.Format( L"max élet=%d, nõk életkora esküvõn: (%d-%d), férfiak életkora esküvõn: (%d-%d), gyermek és anya korkülönbsége( %d-%d ), gyermek és apa korkülönbsége( %d-%d)", \
	m_maxLifespan, m_minAgeWAtWedd, m_maxAgeWAtWedd, m_minAgeHAtWedd, m_maxAgeHAtWedd, m_minDiffMC, m_maxDiffMC, m_minDiffFC, m_maxDiffFC );

	caption = L"dh: feleség-férj ill. gyerek-apa korkülönbsége || dw: anya-gyerek korkülönbsége"; 
	caption = L"dh: apa életkora esküvõn ill. a gyerek születésekor  || dw: anya életkora az esküvõn ill. a gyerek születésekor"; 
	GetDlgItem( IDC_CAPTION )->SetWindowTextW( caption );


	colorKeres.SetTextColor( theApp.m_colorClick );
	colorNext.SetTextColor( theApp.m_colorClick );


	SetWindowTextW( title );

//	CString fileName = L"childrenFAmily";
//	m_filespec = theApp.openTextFile( &textF, fileName, L"w+" );

//	str.Format( L"Az anya %d éves kora után született gyerekeket fogadj el a program helyesnek.\n\n", m_maxDiffMC ); 
//	fwprintf( textF, str );

	createColumns();
	collectFamily();

	m_ListCtrl.SetItemCountEx( vList.size() + 1  );
	m_ListCtrl.AttachDataset( &vList );


//	fclose( textF );
	ShowWindow( SW_MAXIMIZE );
//	theApp.showFile( m_filespec );
	return TRUE; 
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::createColumns()
{

	m_ListCtrl.KeepSortOrder(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );

	m_ListCtrl.InsertColumn( L_CNT,			L"cnt",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_TYPE,		L"type",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_LINENUMBER,	L"line#",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_MARRIAGE,	L"esküvõ",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_INDEX,		L"ix",			LVCFMT_RIGHT,	 30,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_ROWID,		L"rowid",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( L_NAME,		L"név",			LVCFMT_LEFT,	200,-1,COL_TEXT );
	m_ListCtrl.InsertColumn( L_BIRTH,		L"születés",	LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DEATH,		L"halál",		LVCFMT_LEFT,	 80,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_AGE,			L"kor",			LVCFMT_RIGHT,	 35,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DIFFH,		L"dH",			LVCFMT_RIGHT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_DIFFW,		L"dW",			LVCFMT_RIGHT,	 30,-1,COL_TEXT);
	m_ListCtrl.InsertColumn( L_COMMENT,		L"hiba",		LVCFMT_LEFT,	500,-1,COL_TEXT);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedKeres()
{
	keress( 0 );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnClickedNext()
{
int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	if( nItem == -0 )
	{
		AfxMessageBox( L"Nincs kijelölve sor!" );
		return;
	}
	keress( nItem + 1 );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::keress( int start )
{
	CString	search;
	GetDlgItem( IDC_SEARCH )->GetWindowText( search );
	if( search.IsEmpty() )
	{
		AfxMessageBox( L"Meg kell adni a keresendõ stringet!");
		return;
	}

	CProgressWnd wndProgress(NULL, L"Folyik a keresés.." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0, m_ListCtrl.GetItemCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);



	int		itemCnt	= m_ListCtrl.GetItemCount();
	int		length	= search.GetLength();
	int		nItem;
	int		topIndex = m_ListCtrl.GetTopIndex();
	CString	str;

	theApp.unselectAll( &m_ListCtrl );

	for( nItem = start; nItem < itemCnt-1; ++nItem )
	{
		str = m_ListCtrl.GetItemText( nItem, L_NAME );
		str = str.Left(length);						// az aktuális search string hosszával azonos hosszúság leválasztása
		if( str == search )	break;
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	if( nItem < itemCnt-1 )			// megtalálta a keresett embert,. aki az nItem-1 sorban van
	{
		m_ListCtrl.EnsureVisible( nItem, FALSE );

		if( nItem > topIndex )   // lefele megy, fel kell hozni a tábla tetejére a megtalált sort
		{
			int countPP = m_ListCtrl.GetCountPerPage();
			int nItemEV	= nItem - 1 + countPP;			// alaphelyzet: a kijelölt sor az ablak tetején

			if( nItemEV > itemCnt - 1 )					// már nem lehet az ablak tetejére hozni, mert nincs annyi adat
				nItemEV = itemCnt - 1;

			m_ListCtrl.EnsureVisible( nItemEV, FALSE );
		}
		m_ListCtrl.SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
		Invalidate( false );
	}
	else
	{
		str.Format( L"%s nevû embert nem találtam!", search );
		AfxMessageBox( str );
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////44
BOOL CCheckFamilyDates::PreTranslateMessage(MSG* pMsg)
{
	int x=(int)pMsg->wParam;

    if( pMsg->message==WM_KEYDOWN)
    {
		switch( x )
		{
		case VK_RETURN:
			GetDlgItem( IDC_SEARCH )->GetWindowTextW( str );
			if( str.GetLength() ) 
			OnClickedKeres();
			break;
		case VK_ESCAPE:
			CDialogEx::OnCancel();
		}
	}
	return CWnd::PreTranslateMessage(pMsg);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CCheckFamilyDates::collectFamily()
{
	wndP.Create( NULL, L"Családok lekérdezése..." );
	wndP.GoModal();

	vList.clear();
	m_cnt = 1;

	m_command =L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date FROM people ORDER BY last_name, first_name";
	if( !query( m_command ) ) return false;

	
	wndP.SetRange( 0, m_recordset->RecordsCount() );
	wndP.SetPos(0 );
	wndP.SetStep(1 );

	int z;
	UINT j;

	
	CString name;
	bool push;

	// férjek cikluse
	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{
		h.rowid = m_recordset->GetFieldString( X_ROWID );

		m_command.Format( L"SELECT rowid,* FROM marriages WHERE spouse1_id='%s' ORDER by orderWife", h.rowid );
		if( !query1( m_command ) ) break;
		if( m_type == 1 && m_recordset1->RecordsCount() != 1 )
			goto cont;				// csak 1 feleségüeket akarunk, de ez nem az
		else if( m_type == 2 && m_recordset1->RecordsCount() < 2 )
			goto cont;				// csak több feleségûeket akarunk, de ez nem az

		h.numOfSpouses	= m_recordset1->RecordsCount();
		h.linenumber	= m_recordset->GetFieldString( X_LINENUMBER );
		h.name.Format( L"%s %s", m_recordset->GetFieldString( X_LAST_NAME), getFirstWord( m_recordset->GetFieldString( X_FIRST_NAME) ) );
		h.birth			= m_recordset->GetFieldString( X_BIRTH );
		h.death			= m_recordset->GetFieldString( X_DEATH );

		h.birthJo = checkDate( h.birth );
		h.deathJo = checkDate( h.death );
		h.message.Empty();
		h.age.Empty();
	
		// házasságok lekérdezése
		vWifes.clear();
		for( j = 0; j < m_recordset1->RecordsCount() ; ++j, m_recordset1->MoveNext() )
		{
			w.rowidH	= h.rowid;
			w.rowid		= m_recordset1->GetFieldString( MARRIAGES_SPOUSE2_ID );

			w.marriage	= m_recordset1->GetFieldString( MARRIAGES_DATE );
			w.marriageJo = checkDate( w.marriage );
			// feleség lekérdezése
			m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date FROM people WHERE rowid='%s'", w.rowid );
			if( !query3( m_command ) ) break;

			w.linenumber	= m_recordset3->GetFieldString( Y_LINENUMBER );
			w.name.Format( L"%s %s", m_recordset3->GetFieldString( Y_LAST_NAME), getFirstWord( m_recordset3->GetFieldString( Y_FIRST_NAME) ) );
			w.birth			= m_recordset3->GetFieldString( Y_BIRTH );
			w.death			= m_recordset3->GetFieldString( Y_DEATH );
	
			w.birthJo = checkDate( w.birth );
			w.deathJo = checkDate( w.death );
			w.age.Empty();

			w.motherOrder	= j+1;
			w.message.Empty();
			vWifes.push_back( w );
		}
		if( j < m_recordset1->RecordsCount() ) break;

		//gyerekek lekérdezése
		m_command.Format( L"SELECT rowid, linenumber, first_name, last_name, birth_date, death_date, mother_id, parentIndexCalc FROM people WHERE father_id ='%s' ORDER BY parentIndexCalc", h.rowid );
		if( !query2( m_command ) ) break;
		
		// ha csak gyerektelenket akarunk, de ennek van, akkor kihagyjuk
		if( m_type == 0 && m_recordset2->RecordsCount() ) goto cont;


		vChildren.clear();
		for( UINT k = 0; k < m_recordset2->RecordsCount() ; ++k, m_recordset2->MoveNext() )
		{
			c.name.Format( L"%s %s", m_recordset2->GetFieldString( Z_LAST_NAME), getFirstWord( m_recordset2->GetFieldString( Z_FIRST_NAME) ) );

			c.linenumber	= m_recordset2->GetFieldString( Z_LINENUMBER );
			c.rowid			= m_recordset2->GetFieldString( Z_ROWID );
			c.birth			= m_recordset2->GetFieldString( Z_BIRTH );
			c.death			= m_recordset2->GetFieldString( Z_DEATH );

			c.birthJo = checkDate( c.birth );
			c.deathJo = checkDate( c.death );

			c.father_id		= h.rowid;
			c.mother_id		= m_recordset2->GetFieldString( Z_MOTHER_ID );
			c.mother_index	= _wtoi( m_recordset2->GetFieldString( Z_MOTHERINDEX ) );
			c.message.Empty();
			c.age.Empty();

			vChildren.push_back( c );
		}
		checkFamily1();

cont:	wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::checkFamily1()
{
	WIFES		w;
	CHILDREN	c;
	int	mx;
	int age;
	int z;
	int ret;
//	_int64 diff;
	bool hiba = false;


	// születés-halál, életkor
	if( ( ret = diffD( h.death, h.birth, &age ) ) != INT_MAX )
	{
		h.age.Format( L"%d", age );
		if( ret < 0 )
		{
			h.message = L" , születés > halál";
			hiba = true;
		}
		if( age > m_maxLifespan )
		{
			str.Format( L" ,%d évesen halt meg", age );
			h.message += str;
			hiba = true;
		}
	}


	for( UINT i = 0; i < vWifes.size(); ++i )
	{
		w = vWifes.at(i);

		if( ( ret = diffD( w.death, w.birth, &age ) ) != INT_MAX )
		{
			vWifes.at(i).age.Format( L"%d", age );
			if( ret < 0 )
			{
				vWifes.at(i).message = L" , születés > halál";
				hiba = true;
			}
			if( age > m_maxLifespan )
			{
				str.Format( L" ,%d évesen halt meg", age );
				vWifes.at(i).message += str;
				hiba = true;
			}
		}
/*
		// férj és feleség korkülönbsége
		if( ( ret = diffD( h.birth, w.birth, &age ) ) != INT_MAX )
		{
			vWifes.at(i).diffH.Format( L"%d", age );
		}
*/
		if( ( ret = diffD( w.marriage, w.birth, &age ) ) != INT_MAX )
		{
			vWifes.at(i).diffW.Format( L"%d", age );
			if( ret < 0 )
			{
				str.Format( L", házasságkötés után született!" ); 
				vWifes.at(i).message = str;
				hiba = true;
			}
			if( m_minAgeWAtWedd > age || age > m_maxAgeWAtWedd )
			{
				str.Format( L", házasságkötéskor %d éves volt", age ); 
				vWifes.at(i).message += str;
				hiba = true;
			}
		}
		if( ( ret = diffD( w.marriage, w.death, &age ) ) != INT_MAX )
		{
				if( ret > 0 )
				{
					str.Format( L", házasságkötés elõtt meghalt!" ); 
					vWifes.at(i).message = str;
					hiba = true;
				}
		}
		if( ( ret = diffD( w.marriage, h.birth, &age ) ) != INT_MAX )
		{
				vWifes.at(i).diffH.Format( L"%d", age );
			if( ret < 0 )
			{
				str.Format( L", férj házasságkötés után született!!" ); 
				vWifes.at(i).message = str;
				hiba = true;
			}
			else
			{
				if( m_minAgeHAtWedd > age || age > m_maxAgeHAtWedd )
				{
					str.Format( L", férj házasságkötéskor %d éves volt", age ); 
					vWifes.at(i).message = str;
					hiba = true;
				}
			}
		}
		if( ( ret = diffD( w.death, h.birth, &age ) ) != INT_MAX )
		{
			if( ret < 0 )
			{
				str.Format( L", férj a feleség halála után született!" ); 
				vWifes.at(i).message = str;
				hiba = true;
			}
		}
		if( ( ret = diffD( h.death, w.birth, &age ) ) != INT_MAX )
		{
			if( ret < 0 )
			{
				str.Format( L", feleség  a férj halála után született!" ); 
				vWifes.at(i).message = str;
				hiba = true;
			}
		}

	}
	

	// gyerekek születési dátumának összehasonlítása anya születési dátumával és annak házasságkötási dátumával
	for( UINT i = 0; i < vChildren.size(); ++i )
	{
		c = vChildren.at(i);
		if( ( ret = diffD( c.death, c.birth, &age ) ) != INT_MAX )
		{
			vChildren.at(i).age.Format( L"%d", age );
			if( ret < 0 )
			{
				vChildren.at(i).message = L" , születés > halál";
				hiba = true;
			}
			if( age > m_maxLifespan )
			{
				str.Format( L" ,%d évesen halt meg", age );
				vChildren.at(i).message += str;
				hiba = true;
			}
		}

		mx = c.mother_index -1;
		// kritikus hibák, utána return;!!!
		if( !vWifes.size() ) break;

		if(  mx + 1 > vWifes.size() )
		{
			vChildren.at(i).message.Format( L", Anya index nagyobb, mint amire a gyerek hivatkozik: %d, %d", vWifes.size(), c.mother_index );
			hiba = true;
			break;
		}
		if( mx < 0 )
		{
			vChildren.at(i).message.Format( L", Anya index %d", mx+1 );
			hiba = true;
			break;
		}
		// a mother indexxel kijelölt anyának dátumaival egyeztet
		w = vWifes.at(mx);



		if( ( ret = diffD( w.marriage, c.birth, &age ) ) != INT_MAX )
		{
			if( ret > 0 )
			{
				str.Format( L", házasságkötés elõtt született" ); 
				vChildren.at(i).message = str;
				hiba = true;
			}
		}

		if( ( ret = diffD( w.death, c.birth, &age ) ) != INT_MAX )
		{
			if( ret < 0 )
			{
				str.Format( L", anyja halála után született" ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
		}
		if( ( ret = diffD( c.birth, w.birth, &age ) ) != INT_MAX )
		{
			vChildren.at(i).diffW.Format( L"%d", age );					// anya életkora a gyerek születésekor
			if( ret < 0 )
			{
				str.Format( L", anyja születése elõtt született", age ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
			
			if( age < m_minDiffMC || m_maxDiffMC < age  )
			{
				str.Format( L", anyja %d éves korában született", age ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
		}
		if( ( ret = diffD( c.birth, h.birth, &age ) ) != INT_MAX )
		{
			vChildren.at(i).diffH.Format( L"%d", age );					// apa életkora a gyerek születésekor
			if( ret < 0 )
			{
				str.Format( L", apja születése elõtt született", age ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
			if( age < m_minDiffFC || m_maxDiffFC < age )
			{
				str.Format( L", apja %d éves korában született", age ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
		}
		// gyerek születési dátum és apa halálozási + 9 hónap

		if( theApp.dateDiff( h.death, c.birth, 9 ) )
		{
			str = L", apja halála után több mint 9 hónapra született"; 
			vChildren.at(i).message += str;
			hiba = true;
		}
	}

	if( hiba )	printFamily();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::checkFamily()
{
	WIFES		w;
	CHILDREN	c;
	int	mx;
	int age;
	int z;
	int ret;
	_int64 diff;
	bool hiba = false;;
	
	CString dateC;
	CString dateW;
	CString dateM;
	CString msg;

	_int64 birthH;
	_int64 deathH;
	_int64 deathH9;
	_int64 birthW;
	_int64 deathW;

	_int64 marriage;
	_int64 birthC;
	_int64 deathC;

	birthH = 0;
	if( checkDate( h.birth ) )
	{
		h.birth = roundDate( h.birth );
		birthH	= theApp.getDateI( h.birth, 0 );
	}
	deathH = 0;
	if( checkDate( h.death ) )
	{
		h.death = roundDate( h.death );
		deathH	= theApp.getDateI( h.death, 0 );
	}

	h.diffH = L"";
	if( deathH && birthH )
	{
		if(deathH < birthH )
		{
			h.message += L", halála < születése";
			hiba = true;
		}
		diff = theApp.dateDiff( h.death, h.birth );
		h.age.Format( L"%d", diff );
		if( (int)diff > m_maxLifespan )
		{
			str.Format( L", %d évesen halt meg", diff );
			h.message += str;
			hiba = true;
		}
	}

	// feleségek idõ-adatainak ellenõrzése ......................................................................
	for( UINT i = 0; i < vWifes.size(); ++i )
	{
		w = vWifes.at(i);

		vWifes.at(i).diffH = L"";
		vWifes.at(i).diffW = L"";
		birthW = 0;
		if( checkDate( w.birth ) )
		{
			w.birth = roundDate( w.birth );
			birthW	= theApp.getDateI( w.birth, 0 );
		}
		
		deathW = 0;
		if( checkDate( w.death ) )
		{
			w.death = roundDate( w.death );
			deathW	= theApp.getDateI( w.death, 0 );
		}
		
		if( deathW && birthW )
		{
			if(  deathW < birthW )
			{
				vWifes.at(i).message += L", halála < születése";
				hiba = true;
			}
			diff = theApp.dateDiff( w.death, w.birth );
			vWifes.at(i).age.Format( L"%d", diff );
			if( diff > m_maxLifespan )
			{
				str.Format( L", %d évesen halt meg", diff );
				vWifes.at(i).message += str;
				hiba = true;
			}
		}
/*
		if( birthH && birthW )
		{
			diff = theApp.dateDiff( w.birth, h.birth );
			vWifes.at(i).diffH.Format( L"%d", diff );			// férj és feleség korkülönbsége
			diff = abs( diff );
		
			if( diff > m_maxDiffBetweenHW )
			{
				str.Format( L", férj és feleség korkülönbsége nagyobb mint %d", m_maxDiffBetweenHW );
				vWifes.at(i).message += str;
				hiba = true;
			}
		}
*/
		marriage = 0;
		if( checkDate( w.marriage ) )
		{
			w.marriage = roundDate( w.marriage );
			marriage	= theApp.getDateI( w.marriage, 0 );

			if( birthW && marriage )
			{
				if( w.birth > w.marriage )
				{
					str.Format( L", házasságkötés után született!!" ); 
					vWifes.at(i).message = str;
					hiba = true;
				}
				else
				{
					diff = theApp.dateDiff( w.marriage, w.birth );
					if( diff < m_minAgeWAtWedd )
					{
						str.Format( L", házasságkötéskor %d éves volt", diff ); 
						vWifes.at(i).message += str;
						hiba = true;
					}
					if( diff > m_maxAgeWAtWedd )
					{
						str.Format( L", házasságkötéskor %d éves volt", diff ); 
						h.message += str;
						hiba = true;
					}
				}
			}


			if( birthH && marriage )
			{
				if( w.marriage < h.birth )
				{
					str.Format( L", férj házasságkötés után született!!" ); 
					vWifes.at(i).message = str;
					hiba = true;
				}
				else
				{
					diff = theApp.dateDiff( w.marriage, h.birth );
					if( diff < m_minAgeHAtWedd )
					{
						str.Format( L", férj házasságkötéskor %d éves volt", diff ); 
						vWifes.at(i).message = str;
						hiba = true;
					}
					if( diff > m_maxAgeHAtWedd )
					{
						str.Format( L", férj házasságkötéskor %d éves volt", diff ); 
						vWifes.at(i).message = str;
						hiba = true;
					}
				}
			}
		}
	}
	// gyerekek idõ adatainak ellenõrzése .............................................................................
	for( UINT i = 0; i < vChildren.size(); ++i )
	{
		c = vChildren.at(i);


		birthC = 0;
		if( checkDate( c.birth ) )
		{
			c.birth = roundDate( c.birth );
			birthC	= theApp.getDateI( c.birth, 0 );
		}
		deathC = 0;
		if( checkDate( c.death ) )
		{
			c.death = roundDate( c.death );
			deathC	= theApp.getDateI( c.death, 0 );
		}
		
		if( deathC && birthC )
		{
			if( deathC < birthC )
			{
				vChildren.at(i).message += L", halála < születése";
				hiba = true;
			}
			diff = theApp.dateDiff( c.death, c.birth );
			vChildren.at(i).age.Format( L"%d", diff );
			if( diff > m_maxLifespan )
			{
				str.Format( L", %d évesen halt meg", diff );
				vChildren.at(i).message += str;
				hiba = true;
			}
		}
	}


	// gyerekek születési dátumának összehasonlítása anya születési dátumával és annak házasságkötási dátumával
	for( UINT i = 0; i < vChildren.size(); ++i )
	{
		c = vChildren.at(i);

		vChildren.at(i).diffW = L"";
		birthC = 0;
		if( checkDate( c.birth ) )
		{
			c.birth = roundDate( c.birth );
			birthC	= theApp.getDateI( c.birth, 0 );
		}
		deathC = 0;
		if( checkDate( c.death ) )
		{
			c.death = roundDate( c.death );
			deathC	= theApp.getDateI( c.death, 0 );
		}

		mx = c.mother_index -1;

		// kritikus hibák, utána return;!!!
		if( !vWifes.size() ) break;

		if(  mx + 1 > vWifes.size() )
		{
			vChildren.at(i).message.Format( L"Anya index nagyobb mint %d", vWifes.size() );
			hiba = true;
			break;
		}
		if( mx < 0 )
		{
			vChildren.at(i).message.Format( L"Anya index %d", mx+1 );
			hiba = true;
			break;
		}

		// a mother indexxel kijelölt anyának dátumaival egyeztet
		w = vWifes.at(mx);
		birthW = 0;
		if( checkDate( w.birth ) )
		{
			w.birth = roundDate( w.birth );
			birthW	= theApp.getDateI( w.birth, 0 );
		}
		deathW = 0;
		if( checkDate( w.death ) )
		{
			w.death = roundDate( w.death );
			deathW	= theApp.getDateI( w.death, 0 );
		}
		marriage = 0;
		if( checkDate( w.marriage ) )
		{
			w.marriage	= roundDate( w.marriage );
			marriage	= theApp.getDateI( w.marriage, 0 );
		}


		if( marriage && birthC && birthC < marriage )
		{
			vChildren.at(i).message += L", házasságkötés elõtt született";
			hiba = true;
		}


		// gyerek születési dátum és anya születési dátum 
		if( birthC && birthW )
		{
			diff = theApp.dateDiff( c.birth, w.birth );
			vChildren.at(i).diffW.Format( L"%d", diff );		// anya életkora a gyerek születésekor
			if( diff < m_minDiffMC || m_maxDiffMC < diff )
			{
				str.Format( L", anyja %d éves korában született", diff ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
		}

		// gyerek születési dátum és anya halálozási dátum
		if( birthC && deathW )
		{
			diff = theApp.dateDiff( c.birth, w.death );
			if( diff > 0 )
			{
				str.Format( L",  anyja halála után született", diff ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
		}


		// gyerekek születési dátumának összehasonlítása apa születési dátumával
		if( birthC && birthH )
		{
			diff = theApp.dateDiff( c.birth, h.birth );
			vChildren.at(i).diffH.Format( L"%d", diff );			// apa életkora a gyerek születésekor
			if( diff < m_minDiffFC || m_maxDiffFC < diff )
			{
				str.Format( L", apja %d éves korában született", diff ); 
				vChildren.at(i).message += str;
				hiba = true;
			}
		}
		
		// gyerek születési dátum és apa halálozási + 9 hónap
		if( birthC &&  deathH )
		{
			
			deathH9	= theApp.getDateI( h.death, 9 );
			if( deathH9 < birthC )
			{
				str = L", apja halála után több mint 9 hónapra született"; 
				vChildren.at(i).message += str;
				hiba = true;
			}
		}
	}
	if( hiba )	printFamily();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::printFamily()
{
	WIFES		w;
	CHILDREN	c;
	CString message;
	UINT j;
	
	message = L"";
	if( h.message.GetLength() > 2 ) message = h.message.Mid( 2 );
	/*
	fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s %s\n", h.linenumber, L"", h.numOfSpouses, h.rowid, h.name, h.birth, h.death, message );

	for( UINT j = 0; j < vWifes.size(); ++j )
	{
		w = vWifes.at(j);
		message = L"";
		if( w.message.GetLength() > 2 ) message = w.message.Mid( 2 );

		fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s %s\n", w.linenumber, w.marriage, w.motherOrder, w.rowid, w.name, w.birth, w.death, message );
	}
	for( UINT k = 0; k < vChildren.size(); ++k )
	{
		c = vChildren.at(k );
		message = L"";
		if( c.message.GetLength() > 2 ) message = c.message.Mid( 2 );
		fwprintf( textF, L"%8s %10s %2d %8s %-25s %10s %10s %s", c.linenumber, L"", c.mother_index, c.rowid, c.name, c.birth, c.death, message );
		fwprintf( textF, L"\n" );
	}
	fwprintf( textF, L"\n" );
	fflush( textF );
	*/

	message = L"";
	if( h.message.GetLength() > 2 ) message = h.message.Mid( 2 );  // a kezdõ L" ," leszedése
	str.Format( L"%d", m_cnt );
	push( str );
	push( L"1" );							// type
	push( h.linenumber );
	push( L"" );							// esküvõ
	push( L"" );							// index
	push( h.rowid );
	push( h.name );
	push( h.birth );
	push( h.death );
	push( h.age );
	push( L"");								// diffH
	push( h.diffW );
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
		push( w.marriage );
		str.Format( L"%d", w.motherOrder );
		push( str );
		push( w.rowid );
		push( w.name );
		push( w.birth );
		push( w.death );
		push( w.age );
		push( w.diffH );
		push( w.diffW );
		push( message );
//	}
	
	for( UINT k = 0; k < vChildren.size(); ++k )
	{
		c = vChildren.at(k );
		if( c.mother_index != j + 1 ) continue;


		message = L"";
		if( c.message.GetLength() > 2 ) message = c.message.Mid( 2 );


		push( L"" );							// cnt
		push( L"3" );							// type
		push( c.linenumber );
		push( L"" );							// marriage
		str.Format( L"%d", c.mother_index );
		push( str );
		push( c.rowid );
		push( c.name );
		push( c.birth );
		push( c.death );
		push( c.age );
		push( c.diffH );
		push( c.diffW );
		push( message );
	}
	}
	emptyLine();
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
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
		if( _wtoi( vList.at( nItem * L_NUMOFCOLUMNS + L_LINENUMBER ))  )
		{
			index = _wtoi( vList.at( nItem * L_NUMOFCOLUMNS + L_INDEX ) );
			pLVCD->clrTextBk = m_rgb[index];
/*
			type = _wtoi( vList.at( nItem * L_NUMOFCOLUMNS + L_TYPE ) );
			if( nCol == L_NAME && type == 2 ) 
				pLVCD->clrTextBk = m_colorWife;
*/
		}
		*pResult = CDRF_DODEFAULT;
		break;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnInfo()
{

	AfxMessageBox( m_info, MB_ICONINFORMATION );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CCheckFamilyDates:: OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;


	if(Menu.LoadMenu( IDR_DROPDOWN_HTML ))
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
		title.Format( L"%s a ga.html fájlban (%s. sor)", m_ListCtrl.GetItemText( nItem, L_NAME ), m_ListCtrl.GetItemText( nItem, L_LINENUMBER )  );
	else
		title.Format( L"%d kijelölt ember a ga.html fájlban", selectedCount );

	theApp.htmlEditLines( &m_ListCtrl, L_LINENUMBER, title );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem,	L_LINENUMBER );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
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
	CHtmlEditLines dlg;
	dlg.m_title.Format( L"%s szülei és testvérei", m_ListCtrl.GetItemText( nItem, L_NAME ) );
	dlg.m_type	= L"F_SIBLINGS";
	dlg.m_rowid = rowid;
	dlg.DoModal();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CCheckFamilyDates::OnDbEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	CString rowid = m_ListCtrl.GetItemText( nItem, 	L_ROWID );
	CRelations dlg;
	dlg.m_rowid = rowid;
	dlg.DoModal();
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