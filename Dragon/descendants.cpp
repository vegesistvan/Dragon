// GaDescendants2.cpp : implementation file
//
// Ha n�k lesz�rmazottait is list�zzuk, akkor n�mely esetben olyan hatalmas lesz�rmazotti lista keletkezne, ami 
// olyan hat�st kelt, hogy lefagyna a program. Lehet, hogy le is fagy, vagyis a html f�jl �sszet�rt sorokat tartalmaz,
// kezelhetetlen az eg�sz





#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "descendants.h"
#include <winuser.h>
#include "ProgressWnd.h"
#include "ColorButton.h"
#include <wingdi.h>
#include "Table_tables.h"
#include "Table_people_columns.h"
#include "Relatives.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////// C R E A T E   G A . H T M L /////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CTablePeople::OnDescendantsAll()          // GEDCOM vagy MANUAL adatb�zisn�l lehet k�rni az �sszes �s list�j�t
{
	theApp.selectAll(&m_ListCtrl);

	if (theApp.m_inputMode == GEDCOM)
		m_ListCtrl.SetSortOrder(2, G_LAST_NAME + 1, G_FIRST_NAME + 1);
	else if (theApp.m_inputMode == MANUAL)
		m_ListCtrl.SetSortOrder(2, N_LAST_NAME + 1, N_FIRST_NAME + 1);
	m_orderix = G_LAST_NAME;

	OnDescendeantsPeople();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnDescendeantsPeople()    // Kijel�lt emberek lesz�rmazottai
{											
	int nItem;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if (!pos)
	{
		theApp.message(L"Kijel?lt ?s?k list?ja", L"\nNincs kijel�lve senki!");
		return;
	}

	theApp.v_rowid.clear();
	theApp.v_tableNumbers.clear();
	CString rowid;

	while (pos)
	{
		nItem = m_ListCtrl.GetNextSelectedItem(pos);
		rowid = m_ListCtrl.GetItemText(nItem, G_ROWID);
		theApp.v_rowid.push_back(rowid);
	}
	CDescendants dlg;
	dlg.DoModal();
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTableTables::OnDescendantsTable()  // Kijel�lt t�bl�k list�ja
{
	int nItem;

	POSITION pos = m_ListCtrl.GetFirstSelectedItemPosition();
	if ( !pos)
	{
		AfxMessageBox(L"Nincs kijel�lve senki!");
		return;
	}
	
	theApp.v_rowid.clear();
	theApp.v_tableNumbers.clear();
	CString rowid;

	while (pos)
	{
		nItem = m_ListCtrl.GetNextSelectedItem(pos);
		rowid = m_ListCtrl.GetItemText(nItem, 1);
		theApp.v_tableNumbers.push_back(rowid);
	}
	CDescendants dlg;
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CRelatives::OnDescendants()
{
	theApp.v_rowid.clear();
	theApp.v_rowid.push_back(m_rowid);

	CDescendants dlg;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CDescendants, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendants::CDescendants(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDescendants::IDD, pParent)
	,m_checkConnect(true)		// t�bl�kat �sszek�sse-e
	,m_checkMother(false)			// n�k lesz�rmazottait list�zza-e

//	, m_check_titolo(false)
	, m_checkCapital(false)

	,m_comboOtherName( 1 )			// m�s n�v bold
	,m_comboSpec( 0 )			// speci�lis karakterek (*+=) bold
	,m_comboComment( 2 )		// comment bold
	,m_comboKiemeltAttrib( 3 )		// lesz�rmazott kiemelt csal�dneve
	,m_comboFontSize( 3 )

	, m_radioCode(true)			// ANSI vagy UTF8 k�drendszer
	, m_radioNumbering(TUP)		// milyen sz�moz�si rendszer legyen (0,1,2) SZLUHA/VIL/TUP
	
	,m_tableNumber(L"")		// tablenumber, ha a t�bl�zat lesz�rmazotti list�j�t k�rj�k
	, m_editWidth(L"k?perny?")
	, m_printed(FALSE)
	, m_checkDeleteRepeated(FALSE)
	, m_checkFolyt(FALSE)
	, m_editGenMax(_T(""))
	, m_orderBirth(FALSE)
	, m_radioOne(true)
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CDescendants::~CDescendants()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

	DDX_Check(pDX, IDC_CHECK_CONNECT, m_checkConnect);
	DDX_Check(pDX, IDC_CHECK_WOMAN, m_checkMother);

	DDX_Check(pDX, IDC_CHECK_CAPITAL, m_checkCapital);


	DDX_Control(pDX, IDC_COMBO_COMMENT, m_combo_comment);
	DDX_Control(pDX, IDC_COMBO_SPEC, m_combo_spec);
	//	DDX_Control(pDX, IDC_COMBO_KIEMELT, m_combo_KiemeltAttrib);
	DDX_Control(pDX, IDC_COMBO_FONTSIZE, m_combo_FontSize);

	DDX_Control(pDX, IDC_STATIC_BACKGROUND, colorBgrnd);
	DDX_Control(pDX, IDC_BUTTON_BGNCOLOR, colorBgn);

	DDX_Radio(pDX, IDC_ANSI, m_radioCode);

	DDX_Control(pDX, IDC_COMBO_NAME, m_combo_OtherName);
	DDX_Control(pDX, IDC_COMBO_DESCATTRIB, m_combo_DAttrib);
	DDX_Control(pDX, IDC_RADIO_NOFAMILYNAME, m_RadioFamily);
	DDX_Text(pDX, IDC_EDIT_WIDTH, m_editWidth);
	DDX_Check(pDX, IDC_CHECK_REPEATED, m_checkDeleteRepeated);
	DDX_Check(pDX, IDC_CHECK_FOLYT, m_checkFolyt);
	DDX_Control(pDX, IDC_STATIC_NAME, colorName);
	DDX_Control(pDX, IDC_STATIC_NUM, colorNum);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_CODE, colorCode);
	DDX_Control(pDX, IDC_STATIC_ATTRIB, colorAttrib);
	DDX_Control(pDX, IDC_STATIC_FAMILY_INLINE, colorFamilyInline);
	DDX_Control(pDX, IDC_STATIC_FAMILYNAME_NO, colorFamilynameNo);
	DDX_Control(pDX, IDC_STATIC_FAMILYNAME_UP, colorFamilynameUp);
	DDX_Text(pDX, IDC_GENMAX, m_editGenMax);
	DDX_Radio(pDX, IDC_RADIO_ORDER, m_orderBirth);
	DDX_Control(pDX, IDC_GROUP_ORDER_TEXT, colorOrderText);

	DDX_Radio(pDX, IDC_RADIO_ONE, m_radioOne);
	DDX_Control(pDX, IDC_STATIC_OUTPUT, colorOutput);
	DDX_Control(pDX, IDC_STATIC_PRINT, colorPrint);
	DDX_Control(pDX, IDC_STATIC_CONTENT, colorContent);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CDescendants, CDialogEx)
	ON_WM_CTLCOLOR()

	ON_COMMAND(IDC_TUPIGNY, &CDescendants::OnTupigny)
	ON_COMMAND(IDC_VILLERS, &CDescendants::OnVillers)
	
	ON_BN_CLICKED(IDC_CHECK_WOMAN, &CDescendants::OnClickedCheckWoman)
	ON_BN_CLICKED(IDC_CHECK_CONNECT, &CDescendants::OnClickedCheckConnect)
	ON_BN_CLICKED(IDC_SZLUHA, &CDescendants::OnClickedSzluha)

	ON_BN_CLICKED(IDOK, &CDescendants::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_CAPITAL, &CDescendants::OnClickedCheckCapital)
	ON_STN_CLICKED(IDC_STATIC_BACKGROUND, &CDescendants::OnClickedStaticBackground)
	ON_BN_CLICKED(IDC_RADIO_NOFAMILYNAME, &CDescendants::OnClickedRadioNofamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAME, &CDescendants::OnRadioFamilyname)
	ON_COMMAND(IDC_RADIO_FAMILYNAMEUP, &CDescendants::OnRadioFamilynameup)
	ON_BN_CLICKED(IDC_CHECK_FOLYT, &CDescendants::OnClickedCheckFolyt)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_DEFAULT, &CDescendants::OnClickedDefault)
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDescendants::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if (theApp.m_inputMode == GAHTML && theApp.m_uniteVersion.IsEmpty() )  // nem egyes�tett adatb�zis
	{
		if (AfxMessageBox(L"Nem egyes�tett adatb�zis!!! Folytatja?", MB_ICONQUESTION | MB_YESNO) == IDNO)
		{
			CDialogEx::OnCancel();
		}
	}

	m_descendantsPath.Format( L"%s\\descendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle );
	if (_waccess(m_descendantsPath, 0))
		_wmkdir(m_descendantsPath);
	parameters();
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::treeTables()  // list�zand� t�bl�k a theapp.v_tableNumbers vektorban
{
	CString file;
	CString title;

	CString familyName;
	CString tableHeader;
	CString tableRoman;
	CString father_id;
	CString rowid;

	if (m_oneFile)
	{
		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(0));
		if (!query(m_command)) return;
		familyName = m_recordset.GetFieldString(0);
		tableHeader = m_recordset.GetFieldString(1);
		tableRoman = m_recordset.GetFieldString(2);
		title.Format(L"%s %s t�bla", familyName, tableRoman);
		title.Trim();

		if (theApp.v_tableNumbers.size() > 1)
			file.Format(L"%s �s tov�bbi %d t�bla", title, theApp.v_tableNumbers.size() - 1);
		else
			file.Format(L"%s_t�bla", title );
		openHtml(file, title, m_colorBgrnd);
	}
	
	CProgressWnd wndProgress(NULL,L"Lesz�rmazotti t�bl�k k�sz�t�se folyik...." ); 
	wndProgress.GoModal();
	wndProgress.SetRange( 0, theApp.v_tableNumbers.size() );
	wndProgress.SetPos( 0 );

	for( UINT i = 0; i < theApp.v_tableNumbers.size(); ++i )
	{
		m_tableNumber = theApp.v_tableNumbers.at(i);

		m_command.Format(L"SELECT familyName, tableHeader, tableRoman FROM tables WHERE rowid ='%s'", theApp.v_tableNumbers.at(i));
		if (!query(m_command)) return;
		familyName = m_recordset.GetFieldString(0);
		tableHeader = m_recordset.GetFieldString(1);
		tableRoman = m_recordset.GetFieldString(2);

		if (!m_oneFile)
		{
			title.Format(L"%s %s t�bla", familyName, tableRoman);
			title.Trim();
			title.Format(L"%s t�bla lesz�rmazotti list�ja", (CString)title);

			file.Format(L"%s_t�bla", tableHeader);
			openHtml(file, title, m_colorBgrnd);
		}
		
		str.Format( L"<b>%s</b>\n", tableHeader );
		print( str );

		m_command.Format( L"SELECT rowid, last_name, sex_id, father_id, peer FROM people WHERE tableNumber = '%s' AND tableAncestry == 1 ", m_tableNumber );
		if( ! query( m_command ) ) return;

		if( !m_recordset.RecordsCount() )
		{
			str.Format( L"%s t�bl�ban nincs �s!!", m_tableNumber );
			AfxMessageBox( str );
			continue;
//			OnCancel();
//			return;
		}
		rowid = m_recordset.GetFieldString(0);
		father_id	= m_recordset.GetFieldString( 3 );

		desc.childrenPrinted	= 0;
		desc.numOfSpouses		= 0;
		desc.parentIndexLast	= 0;


		if( father_id.IsEmpty() || !father_id.Compare( L"0" ) )  // ha nincs apa, akkor mag�t az �st teszi be a vDesc-be
		{
			m_sex_id = m_recordset.GetFieldString( 2 );

			desc.sex_id				= m_sex_id;
			desc.rowid				= rowid;     //m_rowid1;
			desc.hidden				= false;
			desc.numOfChildren		= getNumberOfChildren( rowid, m_sex_id );

		}
		else		// ha van apja az �snek, akkor az ap�t hidden-k�nt elteszi, hogy ha t�bb gyereke van, akkor mind list�zza 
		{
			m_command.Format( L"SELECT rowid, last_name, sex_id, generation FROM people WHERE father_id='%s'", father_id );
			if( ! query1( m_command ) ) return;

			m_sex_id = MAN;

			desc.sex_id				= MAN;
			desc.rowid				= father_id;
			desc.hidden				= true;
			desc.numOfChildren		= getNumberOfChildren( father_id, m_sex_id );
		}

		
		
		descendants();

		if( theApp.v_tableNumbers.size() > 1 )
		{
			for( int i =0; i < m_indent; ++i )
				fwprintf( fl, L"%s", m_tag2 );
			fwprintf( fl, L"<br><br><br>" );
		}
		if (!m_oneFile)
		{
			closeHtml();
		}

		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	theApp.v_tableNumbers.clear();
	if (m_oneFile)
		closeHtml();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::treePeople()  // ha v_rowid.size() nemnulla, benne a list�zand� �s�k rowid-i //GEDCOM �s MANUAL adatb�zisn�l az �s�k list�ja
{
	CString file;
	CString title;
	CString tableheader;
	CString rowid;

	for( UINT i = 0; i < theApp.v_rowid.size(); ++i )
	{
		rowid = theApp.v_rowid.at(i);
		queryP(rowid);								// m_sex_id-t �s m_numOfSpouses-t hat�rozza meg

		if (theApp.m_inputMode == GEDCOM)
			m_tableNumber = L"1";
		else
		{
			m_command.Format(L"SELECT tablenumber FROM people WHERE rowid = '%s'", rowid);
			if (!theApp.query(m_command)) return;
			m_tableNumber = theApp.m_recordset->GetFieldString(0);
		}
		m_command.Format(L"SELECT tableHeader FROM tables WHERE rowid = '%s'", m_tableNumber);
		if (!theApp.query(m_command)) return;
		tableheader = theApp.m_recordset->GetFieldString(0);


		if (!m_oneFile || !i )
		{
			if( theApp.v_rowid.size() > 1 )
				title.Format(L"%s lesz�rmazotti list�ja (%d/%d)", m_name, i +1, theApp.v_rowid.size() );
			else
				title.Format(L"%s lesz�rmazotti list�ja", m_name );
#ifndef _DEBUG
//			wndProgress.SetText( title );
#endif
			file.Format(L"%s lesz�rmazotti list�ja", m_name);
			openHtml(file, title, m_colorBgrnd);
		}
//		fwprintf(fl, L"%s\n\n", tableheader);

		str.Format( L"%s\n\n", tableheader);
		print(str);
	//	wchar_t* alma = L"alma";
	//	desc.rowid = L"alma";
	//	desc.rowid				= rowid;
		desc.sex_id				= m_sex_id;
		desc.numOfSpouses		= m_numOfSpouses;
		desc.numOfChildren		= getNumberOfChildren( rowid, m_sex_id );
		desc.childrenPrinted	= 0;
		desc.parentIndexLast	= 0;
		desc.hidden				= false;
		desc.rowid = rowid;

		descendants();


		if( theApp.v_rowid.size() > 1 )
		{
			for( int i =0; i < m_indent; ++i )
				fwprintf( fl, L"%s", m_tag2 );
			fwprintf( fl, L"<br><br><br>" );
		}

		if (!m_oneFile)
			closeHtml();
	}
	theApp.v_rowid.clear();
	if (m_oneFile)
		closeHtml();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A kinyomtatand� lesz�rmazot azonos�t�it a <DESCENDANTS> vDesc vektorba t�ltj�k fel, 
// �s annak felhaszn�l�s�val nyomtatjuk ki �ket.
// Mindaddig abban maradnak, am�g az �sszes gyereke kinyomtat�sra ker�l.Akkor t�r�lj�k �t a vektorb�l
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::descendants()
{
	CString rowid;
	int generationsMax = 55;
	m_indent = 0;
	m_genPrev = 0;
	m_gen = 0;
	m_genPeer = 0;
	m_db = 0;

	vRowid.clear();
	vDesc.clear();
	vDesc.push_back( desc );
		
	vSerial.clear();
	vSerial.push_back(1);

	if( m_radioNumbering == SZLUHA ) // orderd list
	{
		m_tag1 = L"<ol>\n";
		m_tag2 = L"</ol>\n";
	}
	else				// unordered list
	{
		m_tag1 = L"<ul>\n";
		m_tag2 = L"</ul>\n";
	}

	CProgressWnd wndP(NULL,L"Lesz�rmazotti t�bl�k k�sz�t�se folyik...." ); 
	wndP.GoModal();
	wndP.SetRange( 0, 10000 );
	wndP.SetPos( 0 );

	int cnt = 0;
	// az els� ember a treePeople, treeTables-ben ker�l bet�lt�sre 
	while( true )
	{
		if (cnt == 10000)	cnt = 0;
		wndP.SetPos(cnt);
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
		++cnt;

		if( !vDesc.at( m_gen ).hidden )
		{
/*
			if( m_gen > generationsMax )
			{
				for( int i =0; i < m_indent; ++i )
					fwprintf( fl, L"%s\n", m_tag2 );
				str.Format( L"<br><br><font color='red'>%d-n?l t?bb gener?ci?t ?rt ki, m?r kezelhetetlen, ez?rt abbahagyom!!!</font>", generationsMax );
				print( str );
				break;
			}
*/
			printGAline();	// a vDesc tetej�n l�v� lesz�rmazottat kinyomtatjuk  m_genPrev-et be�ll�tja

		}

		
		if( vDesc.at( m_gen ).sex_id == WOMAN && !m_checkMother )		// ha n� a lesz�rmazott �s annak a gyerekeit nem akarjuk list�zni
		{
			vDesc.at( m_gen ).numOfChildren		= 0;
			vDesc.at( m_gen ).childrenPrinted	= 0;
		}

		if( vDesc.at( m_gen ).numOfChildren == vDesc.at( m_gen ).childrenPrinted )	// m?r minden gyerek?t ki?rta
		{
			//visszamegy az el�z� gener�ci�ra, amelyiknak m�g van ki nem �rt gyereke
			while( m_gen >= 0 && vDesc.at( m_gen ).numOfChildren == vDesc.at( m_gen ).childrenPrinted )
			{
				vDesc.pop_back();		// visszamegy az el�z� gener�ci�ra
				--m_gen;
			}
		}
		if( m_gen < 0 ) break;			// nincs tov�bb!!!

		// a kinyomtatott ember k�vetkez�, m�g ki nem nyomtatott gyerek�t keresi
		rowid = getNextChildRowid();
		if( !rowid.IsEmpty() )			// van m�g gyerek
		{
			queryP( rowid );		// lek�rdezi a gyereket �s be�ll�tja m_sex_id-t �s m_numOfSpouses-t
			desc.rowid				= rowid;
			desc.sex_id				= m_sex_id;
			desc.numOfSpouses		= m_numOfSpouses;				// a gyerek apja feles?geinek sz?m?t meg?rzi 
			desc.childrenPrinted	= 0;
			desc.parentIndexLast	= 0;
			desc.hidden				= false;
			desc.numOfChildren		= getNumberOfChildren( rowid, m_sex_id );


			vDesc.at( m_gen ).childrenPrinted += 1;							// a most kinyomtatand� testv�rek sz�ma n� egyet

			vDesc.push_back( desc );
			++m_gen;

			// Tupigny-h�z el��ll�tja a gener�ci�n bel�l egyes�vel n�vekv� sorsz�mokat
			if( m_gen < vSerial.size() ) ++vSerial.at( m_gen );			// m�r l�tez� gener�ci�, 1-el n�veli a sorsz�mot	
			else	vSerial.push_back( 1 );								// �j gener�ci�: sorsz�m 1-el kezd�dik 
		}
	}
	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::queryP( CString rowid )
{
	m_command.Format( L"SELECT generation, sex_id, posterior, last_name, first_name, tableNumber  FROM people WHERE rowid ='%s'", rowid );
	if( !query( m_command ) ) return;


	m_sex_id = m_recordset.GetFieldString( 1 );
	m_posterior = m_recordset.GetFieldString(2);
	CString last_name = m_recordset.GetFieldString(3);
	if (!last_name.IsEmpty() && !iswupper(last_name.GetAt(0)))
		last_name.Empty();
	last_name.Replace('|', ' ');
	last_name.Replace('/', ' ');
	last_name.Replace('?', ' ');

	CString first_name = m_recordset.GetFieldString(4);
	first_name.Replace('/', ' ');
	first_name.Replace('(', ' ');
	first_name.Replace(')', ' ');
	first_name.Replace('*', ' ');
	first_name.Replace('|', ' ');
	first_name.Replace('"', ' ');
	first_name.Replace('?', ' ');
	first_name.Trim();

	m_tableNumber = m_recordset.GetFieldString(5);


	m_name.Format(L"%s %s", last_name, first_name );
	m_name.Trim();

	if( m_sex_id == MAN )
		m_command.Format( L"SELECT count() FROM marriages WHERE husband_id = '%s'", rowid );
	else
		m_command.Format( L"SELECT count() FROM marriages WHERE wife_id = '%s'", rowid );
	if( !query4( m_command ) ) return;
	m_numOfSpouses = _wtoi( m_recordset4.GetFieldString( 0 ) );
	

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int CDescendants::getNumberOfChildren( CString rowid, CString sex_id  )
{
	if( sex_id == MAN )
	{
		if( m_checkConnect )
			m_command.Format( L"SELECT count() FROM people WHERE father_id='%s'", rowid );
		else
			m_command.Format( L"SELECT count() FROM people WHERE father_id='%s' AND tableNumber='%s'", rowid, m_tableNumber );
	}
	else
	{
		if( m_checkConnect )
			m_command.Format( L"SELECT count() FROM people WHERE mother_id='%s'", rowid );
		else
			m_command.Format( L"SELECT count() FROM people WHERE mother_id='%s' AND tableNumber='%s'", rowid, m_tableNumber );
	}
	if (!query(m_command)) return 0;
		
	return( _wtoi( m_recordset.GetFieldString(0) ) );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CDescendants::getNextChildRowid()
{
	CString parent_id	= vDesc.at( m_gen ).rowid;
	UINT	ixChild		= vDesc.at( m_gen ).childrenPrinted + 1;	// lesz?rmazott ixChild-adik gyerek?t keresi  
	CString	sex_id		= vDesc.at( m_gen ).sex_id;

	if (sex_id == MAN)
	{
		if (m_checkConnect  )	// ha �sszekapcsol�s van, akkor t�bl�t�l f�ggetlen�l keres
		{
			if (m_orderBirth)
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' ORDER BY lineNumber", parent_id); // ?gy lesznek a gyerekek sorrendben!!
		}
		else		// ha nincs �sszekapcsol�s, akkor csak t�bl�n bel�l keres
		{
			if( m_orderBirth )
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY birth_date", parent_id, m_tableNumber);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE father_id='%s' AND tableNumber='%s' ORDER BY lineNumber", parent_id, m_tableNumber);
		}
	}
	else
	{
		if (m_checkConnect )
		{
			if (m_orderBirth)
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY birth_date", parent_id);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' ORDER BY lineNumber", parent_id);
		}
		else
		{
			if (m_orderBirth)
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY birth_date", parent_id, m_tableNumber);
			else
				m_command.Format(L"SELECT rowid FROM people WHERE mother_id='%s' AND tableNumber='%s' ORDER BY lineNumber", parent_id, m_tableNumber);
		}
	}

	if( !query( m_command ) ) return L"";
	if( m_recordset.RecordsCount() < ixChild )
	{
		str.Format( L"%s ap�nak csak %d gyereke van, te pedig a %d.-kat k�red!!", parent_id, m_recordset.RecordsCount(), ixChild );
		AfxMessageBox( str );
		return L"";
	}

	m_recordset.MoveTo( ixChild - 1 );
	return( m_recordset.GetFieldString( 0 ) );
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::OnPaint()
{
	CPaintDC dc(this); 

	COLORREF	color = RGB(0, 144, 216);
	COLORREF color2;

	color = LIGHTBLUE;
	color2 = GOLD;
	color2 = RGB(255, 0, 0);

	colorFrame(&dc, IDC_GROUPBOX_DESC, color);
	colorFrame(&dc, IDC_GROUP_CONTENT, color);
//	colorFrame(&dc, IDC_GROUPBOX_DESC_NO, color2);
//	colorFrame(&dc, IDC_GROUPBOX_DESC_INLINE, color2);
//	colorFrame(&dc, IDC_GROUPBOX_DESC_UP, color2);


	colorFrame(&dc, IDC_GROUP_ORDER, color);
	colorFrame(&dc, IDC_GROUPBOX_ATTRIB, color);
	colorFrame(&dc, IDC_GROUPBOX_CODE, color);
	colorFrame(&dc, IDC_GROUPBOX_NUM, color);
	colorFrame(&dc, IDC_GROUPBOX_PRINT, color);
	colorFrame(&dc, IDC_GROUPBOX_OUTPUT, color);
	

//	pW->ClientToScreen(&rc);
//	this->ScreenToClient(&rc);

//	dc.FillSolidRect(&rc, RGB(255, 0, 0)); // ez nem j�, mert az eg�szet kisz�nezi, a benne l�v� cotrolok meg feh�rek maradnak
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDescendants::colorFrame(CPaintDC* dc, int IDC, COLORREF color )
{
	CRect rc;

	HPEN penLine = CreatePen(PS_SOLID, 0, color );
	dc->SelectObject(penLine);

	CWnd* pW = this->GetDlgItem(IDC);
	pW->GetWindowRect(&rc);
	this->ScreenToClient(&rc);


	rc.TopLeft().x		-= 1;	// a sz�rke keret mellett kell rajzolni, mert k�l�nben a keret fel�l�rja a sz�nes n�gyzetet
	rc.BottomRight().x	+= 1;	// ez szebb, mint az al�bbi vonalas megold�s, de kifeh�r�tia n�gysz�g�n bel�l �s a sz�vegek h�tt�rsz�ne sz�rke marad.
	rc.TopLeft().y		+= 5;	// Ez�rt m�gis szebb a vonalas rajzol�s

	COLORREF colour = ::GetSysColor(COLOR_3DFACE);
	HBRUSH hbr = CreateSolidBrush( colour );
	dc->SelectObject( hbr );	// hogy a h�tt�r olyan sz�rke legyen, mint a benne l�v� control-ok (sz�veg, gomb )

	dc->Rectangle(&rc);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HBRUSH CDescendants::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	int IDC = pWnd->GetDlgCtrlID();
	
//	CTLCOLOR_BTN Button control
//	CTLCOLOR_DLG Dialog box
//	CTLCOLOR_EDIT Edit control
//	CTLCOLOR_LISTBOX List - box control
//	CTLCOLOR_MSGBOX Message box
//	CTLCOLOR_SCROLLBAR Scroll - bar control
//	CTLCOLOR_STATIC Static control

/*
	if (nCtlColor == CTLCOLOR_STATIC)
	{
		if (IDC == IDC_RADIO_FAMILYNAME || IDC== IDC_RADIO_NOFAMILYNAME || IDC == IDC_RADIO_FAMILYNAMEUP)
		{
//			pDC->SetBkMode(TRANSPARENT);
//			pDC->SetTextColor( LIGHTBLUE );
//			pDC->SetBkColor( RED );
//			hbr = CreateSolidBrush( RGB( 199,228,243 ));
			hbr = CreateSolidBrush( RGB( 250, 235, 235 ));
			hbr = CreateSolidBrush(RGB(255, 169, 169));
		}
	}
*/	
	return hbr;
	
}

