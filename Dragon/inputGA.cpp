#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"
#include "SelectFamilyName.h"
#include "SelectTableName.h"
#include "SelectLineNumber.h"
#include "ConnectCsaladTorzs.h"
#include "ProgressWnd.h"
#include "checkNames.h"
#include "checkDateFormat.h"
#include "checkParents.h"
#include "GetString.h"
#include <Windows.h>
#include "utilities_dragon.h"



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CDragonDlg::OnInputGA()
//{
//	CInputGA dlg;
//	if (dlg.DoModal() == IDCANCEL) return;
//
//	mainTitle();
//	ChangeMenu();
//}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Egy leszármazotti string felbontása több lépésben történik, elõször kisebb substringekre, majd azokat elemeire szedjük szét

// I. szint
// 'folyt' levétel
// [...család] és [..család õse]levétel
// G generációs karakter levétele
// descendantString és vMarriageStrings( marriageString ) szétválasztása
//
// II. szint: rövidebb substringek

// marriageString:		marriagePlaceDate, spouseString, parentsString, spouseSpousesString-re
// parentsString:		fatherString, motherString

// A descendantString, spouseString, fatherString, motherString és spouseSpouseString azonos szerkezetûek,
// ezeket a splitPeopleString bontja fel az alábbi substringekre:
// nameString, birthString, deathString és commentString-re


// A descNameString és a fatherNameString némileg különbözik a többitõl, ezeket saját függvényük bontja fel,
// a spouseNameString, motherNameString és spouseSpouseNameString felbontását a splitSpouseNameString végzi.


// III. szint: substringek elemekere
// descNameString:		title, titolo, last_name, first_name, comment
// spouseNameString:	title, titolo, last_name, first_name, comment
// fatherNameString:	title, titolo, last_name, first_name, comment
// motherNameString:	title, titolo, last_name, first_name, comment
// spouseSpousesString:	title, titolo, last_name, first_name, comment


// A birthString, deathString és marriagePlaceDateString azonos szerkezetûek, ezeket a splitPlaceDate bontja fel 'place' és 'date' elemekre
// birthString:			place, date
// deathString:			place, date
// marriageString:		place, date


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////// S P L I T  //////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CInputGA, CDialogEx)
CInputGA::CInputGA(CWnd* pParent /*=NULL*/)
	: CDialogEx(CInputGA::IDD, pParent)
{

// people tábla menetett oszlopai	
m_fieldsP = L"\
fileNumber,\
familyNumber,\
tableNumber,\
lineNumber,\
source,\
united,\
spouse,\
spouseparent,\
spousespouse,\
generation,\
sex_id,\
peer,\
title,\
titolo,\
first_name,\
last_name,\
posterior,\
other_names,\
birth_place,\
birth_date,\
death_place,\
death_date,\
comment,\
tableAncestry,\
tableRoman,\
arm,\
sources,\
occupation,\
csalad,\
gap,\
folyt,\
father_id,\
mother_id,\
parentIndex,\
whichHusband,\
whichWife,\
whichChildFather, \
whichChildMother, \
numOfSpouses,\
numOfChildren\
";

// a "marriages' tábla oszlopai
m_fieldsM = L"\
tableNumber,\
lineNumber,\
husband_id,\
wife_id,\
whichHusband,\
whichWife,\
place,\
date,\
source\
";

// a 'tables' tábla oszlopai
m_fieldsT = L"\
fileNumber,\
familyNumber,\
lineNumber,\
tableHeader,\
percent,\
familyName,\
alias,\
titolo,\
tableRoman,\
comment,\
torzs,\
arm,\
joint,\
folyt,\
csalad,\
peer,\
ancestor\
";



m_generationPrev = ' ';
_contractions = theApp.contractions();

REFRESH = FALSE;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CInputGA::~CInputGA(void)
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMENT, colorComment);
	DDX_Control(pDX, IDC_STATIC_GA, colorHtml);
	DDX_Control(pDX, IDC_MAPPA, m_mappaCtrl);
	DDX_Control(pDX, IDC_DATABASE, m_databaseCtrl);
	DDX_Control(pDX, IDC_HTML, m_htmlCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CInputGA, CDialogEx)
	ON_STN_CLICKED(IDC_STATIC_GA, &CInputGA::OnClickedStaticHtml)
	ON_BN_CLICKED(IDC_BUTTON_FILE, &CInputGA::OnClickedButtonFile)
	ON_BN_CLICKED(IDC_BUTTON_FAMILY, &CInputGA::OnClickedButtonFamily)
	ON_BN_CLICKED(IDC_BUTTON_TABLE, &CInputGA::OnClickedButtonTable)
	ON_BN_CLICKED(IDC_BUTTON_LINE, &CInputGA::OnClickedInputGahtmlFromline)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputGA::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	colorComment.SetTextColor(RED);
	colorHtml.SetTextColor(theApp.m_colorClick);

	SetWindowTextW(L"GA.html fájl beolvasása adatbázisba");

	m_mappaCtrl.SetWindowTextW(theApp.m_dbFolderPath);
	m_databaseCtrl.SetWindowTextW(theApp.m_dbFileName);
	m_htmlCtrl.SetWindowTextW(theApp.m_htmlFileName);

	m_command = L"SELECT count() FROM people";
	if (!query(m_command)) return FALSE;

	m_cnt= _wtoi(m_recordset.GetFieldString(0));
	if (m_cnt )
		str.Format(L"Az adatbázisban %d ember van nyilvántartva! Törölni fogom az adatbázist!!", m_cnt);
	else
		str = L"Az adatbázis üres.";

	GetDlgItem(IDC_COMMENT)->SetWindowTextW(str);

	if (theApp.m_htmlFileName.IsEmpty())
	{
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_FAMILY)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_TABLE)->EnableWindow(false);
		GetDlgItem(IDC_BUTTON_LINE)->EnableWindow(false);
	}

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// GA.html fájl kiválasztása
void CInputGA::OnClickedStaticHtml()
{
	while (true)
	{
		if( !theApp.selectHtml() ) break;
			
		if (theApp.m_htmlFolderPath != theApp.m_dbFolderPath)
		{
			str.Format(L"Csak a\n%s\nmappából választhatsz GA.htm fájlt!", theApp.m_dbFolderPath);
			AfxMessageBox(str, MB_ICONEXCLAMATION);
		}
		else
		{
			m_htmlCtrl.SetWindowTextW(theApp.m_htmlFileName);
			GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_FAMILY)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_TABLE)->EnableWindow(true);
			GetDlgItem(IDC_BUTTON_LINE)->EnableWindow(true);
			break;
		}
	}
/*
	if (!theApp.m_htmlFileName.IsEmpty())
	{
		GetDlgItem(IDC_BUTTON_FILE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_FAMILY)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_TABLE)->EnableWindow(true);
		GetDlgItem(IDC_BUTTON_LINE)->EnableWindow(true);

	}
*/
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::OnClickedButtonFile()
{
	m_rollToLine = 0;
	m_rollToLine = 0;
	m_rollToFamily = 0;
	m_rollToTable = 0;
//	m_fileNumber = 1;

	inputFile();
	CDialogEx::OnOK();
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::OnClickedButtonFamily()
{
	CSelectFamilyName dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	m_num = _wtoi(dlg.m_num);
	m_rollToLine = 0;
	m_rollToTable = 0;
	m_rollToFamily = _wtoi(dlg.m_familyNumber);
	m_familyName = dlg.m_familyName;
//	m_fileNumber = 1;

	inputFile();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::OnClickedButtonTable()
{
	CSelectTableName dlg;
	if (dlg.DoModal() == IDCANCEL) return;
	CDialogEx::OnOK();

	m_num = _wtoi(dlg.m_num);
	m_rollToLine = 0;
	m_rollToTable = dlg.m_tableNumberSelected;
	m_rollToFamily = 0;
	m_tableName = dlg.m_tableNameSelected;
//	m_fileNumber = 1;

	inputFile();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::OnClickedInputGahtmlFromline()
{
//	CGetString dlg;
	SelectLineNumber dlg;
	if (dlg.DoModal() == IDCANCEL) return;

	m_num = _wtoi(dlg.m_numberOfLines);
	m_rollToLine = _wtoi( dlg.m_lineNumber);
	m_rollToTable = 0;
	m_rollToFamily = 0;


/*
	int lineNumber;
	lineNumber = theApp.GetProfileInt(L"dragon", L"lineNumber", 0);
	dlg.m_string.Format(L"%d", lineNumber);
	dlg.m_caption.Format(L"Add meg a %s fájlban lévõ sor sorszámát amitõl kezdve be akarod olvasni a fájlt!", theApp.m_htmlFileName);
	if (dlg.DoModal() == IDCANCEL) return;
	lineNumber = _wtoi(dlg.m_string);
	theApp.WriteProfileInt(L"dragon", L"lineNumber", lineNumber);

	m_rollToLineFrom = lineNumber;
	m_rollToLine = lineNumber;
	m_rollToTable = 0;
	m_rollToFamily = 0;
	m_fileNumber = 1;
*/
	inputFile();
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CInputGA::inputFile()
{
	FOLYT sFolyt;			// családonként gyûjti a folyt sorokat, hogy a kapcsolódó gyerek szüleit meg lehessen adni
	GENERATIONS	gen;		// a generáció, az apa és max 10 feleség azonoosítója, amit a v_genertion-ben õriz késõbbi felhasználásra
	CString		caption;
	int z;

	CDialogEx::OnOK();

	if (m_cnt)
	{
		if (!theApp.clearDatabase()) return false;
		setCreationTime(theApp.m_dbPathName);
	}

	--m_num;
	if (m_num < 0) m_num = 0;

	m_textFilePath.Format(L"%s\\descendants_%s", theApp.m_workingDirectory, theApp.m_dbFileTitle);
	if (_waccess(m_textFilePath, 0))
		_wmkdir(m_textFilePath);
	CString textFilespec;
	textFilespec.Format(L"%s\\input_%s.txt", m_textFilePath, theApp.m_htmlFileName);
	if( TEST)
	{
		if (textF != NULL)
			fclose(textF);
		if (!openFileSpec(&textF, textFilespec, L"w+"))
			return NULL;
	}

		
	if (m_rollToLine)
		caption.Format(L"A %s fájl beolvasása a %d sortól", theApp.m_htmlFileName, m_rollToLine);
	else if (m_rollToFamily)
		caption.Format(L"A %s fájl %s (%d) családjának beolvasása", theApp.m_htmlFileName, m_familyName, m_rollToFamily);
	else if (m_rollToTable)
		caption.Format(L"A %s fájl %s (%d) táblájának beolvasása", theApp.m_htmlFileName, m_tableName, m_rollToTable);
	else
		caption.Format(L"A %s fájl beolvasása", theApp.m_htmlFileName);


	int position;
	
	m_command.Format( L"SELECT last_insert_rowid() FROM people" );
	if( !query( m_command ) ) return false;
	m_rowid = _wtoi( m_recordset.GetFieldString(0) );  // m_rowid az utoljára insertált people rowid-ja

	v_tableHeader.clear();

	theApp.m_cntPeople		= 0;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlPathName );		// a bemeneti fájl kódrendszere: UTF8 vagy ANSI
	CStdioFile file( theApp.m_htmlPathName, CFile::modeRead);		// input ga.html fájl
	int fileLength = (int)file.GetLength();
	
	if( !rollFile( &file ) ) return false;		// eltekeri a fájlt a kívánt pozicióba, 
												// ami beállítja az m_lineNumber, m_tableNumber, ill. m_familyNUmber értékeket.

	m_tableNumber1 = 0;

	CProgressWnd wndP(NULL, caption );
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);


	CString cLine;
	bool kilepett = false;
	m_tableHeader.tableHeader.Empty();  // a nem üres tableHeader jelzi majd, hogy be kell tenni a vektorba

	if( !theApp.execute( L"BEGIN" ) ) return false;			// Ha nme itt lenne, hanem az insertEntries-ben, akkor nagyon lassú lenne!!!
	while(file.ReadString(cLine)) 
	{
		++m_lineNumber;
		if ( m_rollToLine && m_lineNumber >= m_rollToLine + m_num) break;

/*
#ifndef _DEBUG
		str.Format(L"%d", m_lineNumber);
		wndP.SetText(str);
		AfxMessageBox(str);
#endif
*/
		if (TEST)
		{
			fwprintf(textF, L"%d\n", m_lineNumber);  // hiba estén megállapítható a sor, ahol a baj történt
			fflush(textF);
		}

		if (m_lineNumber == 8126)
			z = 3;

		cLine.Trim();
		if( cLine.IsEmpty() ) continue;

		if (cLine.Left(4) == L"<!--") continue;		// comment

		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM )
			cLine = Utf8ToAnsi( cLine );
		cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
		
		if(  cLine.Left( 2 ) == L"%%" )			// %% és %%% tábla fejléc feldolgozása
		{
			m_genCnt = 0; // az <ol> és <\ol> apalján számolja a generációt a táblán belül. Jelenleg nem használja semmire. 
			if( m_rollToTable && m_tableNumber >= m_rollToTable + m_num ) break;
		
			if (cLine.Left(3) == L"%%%")
			{
				vFolyt.clear();				// folyt sorok egy családra vonatkoznak
				if (m_rollToFamily && m_familyNumber >= m_rollToFamily + m_num) break; // megyünk a következõ %%%-ra
			}

			if( !m_tableHeader.tableHeader.IsEmpty() )
				v_tableHeader.push_back( m_tableHeader );		// elõzõ m_tableHeader megõrzése
		
			splitTableHeader( cLine );				// 
			++m_tableNumber1;						// 1-tõl egyesével nõ a tableNumber, föhggetlenül attól, hoyg hanyadikat olvassuk be!!

			v_generations.clear();					// új táblánál újrakezdi a generációkat
			m_tableAncestry = TRUE;					// a tábla fejlécet követõ elsõ embere lesz a tábla õse
			
		}
		else if( cLine.Left(2) == L"% " )			// '% name' név változást jelöl!!
		{
			str = cLine.Mid(2);
			str = getFirstWord(str );
			str.Replace(',', ' ');			// % Aba, aaaaa
			str.Trim();
			m_familyName = str;
		}
		else			// descendant line
		{
			if( cLine.GetAt(0) != '<' )
			{
				str.Format( L"A %d. sor hibás!\n%s", m_lineNumber, cLine );
				AfxMessageBox( str );
				kilepett = true;
				break;
//				return false;
			}

			cLine = cleanHtmlTags( m_lineNumber, cLine );
			if( cLine.IsEmpty() ) continue;

			splitLine( cLine );		// sor felbontása d, s, strukturákba és v_marriages vektorba
			insertEntries();		// a strukturába és vektorokba összegyûjtött emberek insertálja a people és a marriges táblákba

// itt már ismerjük a rowid-ket !! v_generations feltöltése rowid-kkal
			v_generations.at(v_generations.size() - 1).descendant_id = d.rowid;
			for (UINT i = 0; i < v_marriages.size(); ++i)
			{
				if( i <= MAXNUMOFSPOUSES )
					v_generations.at( v_generations.size() -1 ).spouse_id[i] = v_marriages.at(i).spouse_id;			// beteszi az aktuális házastársak rowid-jeit!!! nincs még rowid!!
			}

			if (!d.folyt.IsEmpty())
			{
				sFolyt.folyt = d.folyt;
				sFolyt.rowidD = d.rowid;
				sFolyt.sex_id = d.sex_id;
				sFolyt.peer = d.peer;
				
				for (UINT i = 0; i < v_marriages.size(); ++i)
				{
					if (i <= MAXNUMOFSPOUSES)
						sFolyt.rowidS[i] = v_marriages.at(i).spouse_id;			// beteszi az aktuális házastársak rowid-jeit!!! nincs még rowid!!
				}
				vFolyt.push_back(sFolyt);
			}
			else
			{
				for (UINT i = 0; i < MAXNUMOFSPOUSES; ++i)
					sFolyt.rowidS[i].Empty();
			}

			wndP.SetPos( (int)file.GetPosition() );
			wndP.StepIt();
			wndP.PeekAndPump();
			if (wndP.Cancelled())
			{
				kilepett = true;
				break;
			}
		}
	}
	if( !theApp.execute( L"COMMIT" ) ) return false;
	wndP.DestroyWindow();
	file.Close();
	if( kilepett ) return false;
	
	theApp.insertIntoFiles(theApp.m_htmlFileName, HTML_FILE);

	v_tableHeader.push_back( m_tableHeader );
	insertTableHeader();
	theApp.insertVersion(GAHTML);

	connectCsalad();
	peerHeritage();
	
	CCheckNames dlg;
	dlg.DoModal();

	CCheckParents dlgP;
	dlgP.DoModal();

	str.Format( L"Az adatok beolvasása befejezõdött a\n\n%s fájlból a\n%s adatbázisba.\n\nVégezd el az összes öszevonás elõtti ellenõrzést\nés amit lehet, javítsd a ga.html fájlban!", theApp.m_htmlFileName, theApp.m_dbFileName );
	AfxMessageBox( str, MB_ICONINFORMATION );

	theApp.m_inputMode = GAHTML;
	theApp.m_dbFileExists = true;
	
	if (TEST)
	{
		fseek(textF, 0, SEEK_END);
		int length = ftell(textF);
		fclose(textF);
		if (length)
			theApp.showFile(textFilespec);
	}
	
	CDialogEx::OnOK();
	return true;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A bemeneti ga-html fájlt az elõírt pozicióba tekeri.
// m_rollToLine:	a kívánt sor száma
// m_rollToTable:	a kívánt tábla azonosító száma
// m_rollToFamily:	a kívánt család azoosító száma
int CInputGA::rollFile( CStdioFile* file )
{
	m_familyNumber	= 0;
	m_tableNumber	= 0;
	m_lineNumber	= 0;

	if( !m_rollToLine && !m_rollToTable && !m_rollToFamily ) return 1;
	
	CString	str;
	CString cLine;
	LONGLONG pos = 0;

	CProgressWnd wndProgress(NULL,L"html fájl tekerése a kívánt pozícióba ..." ); 
	wndProgress.GoModal();
	
	if( m_rollToFamily )
	{
		while(file->ReadString(cLine))
		{
			++m_lineNumber;
			if( cLine.Left(2) == L"%%" )
			{
				splitTableHeader( cLine );		// beállítja: m_familyNumber, m_table_number-t és mindent
				if( m_rollToFamily == m_tableHeader.familyNumber )
				{
					file->Seek( pos, 0 );	// visszamegy a tábla elejére ( az elõzõ sorra )
					--m_tableNumber;
					--m_familyNumber;
					--m_lineNumber;
					return 1;
				}
			}
			pos = file->GetPosition();		// a beolvasott sor poziciója
		}
		str.Format( L"Nincs %d család a fájlban!", m_rollToFamily );
	}


	if( m_rollToTable )
	{
		while(file->ReadString(cLine))
		{
			++m_lineNumber;
			if( cLine.Left(2) == L"%%" )
			{
				splitTableHeader( cLine );
				if( m_rollToTable == m_tableNumber )
				{
					file->Seek( pos, 0 );	// visszamegy a tábla elejére ( az elõzõ sorra )
					--m_tableNumber;
					--m_lineNumber;
					return 1;
				}
			}
			pos = file->GetPosition();		// a beolvasott sor poziciója
		}
		str.Format( L"Nincs %d tábla a fájlban!", m_rollToTable );
	}

	else if( m_rollToLine )
	{
		while(file->ReadString(cLine))
		{
			++m_lineNumber;
			if( cLine.Left(2) == L"%%" )
			{
				splitTableHeader( cLine );
			}
			if( m_lineNumber + 1 == m_rollToLine )		// csak az elõzõ sorig pörög
				return 1;
		}
		str.Format( L"A fájlban nincs %d sor!", m_rollToLine );
	}
	wndProgress.DestroyWindow();
	AfxMessageBox( str );
	return 0;

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::connectFolyt()
{
	CString rowidParent1;
	CString rowidParent2;
	CString sexidParent1;

	CString rowidFather;
	CString rowidMother;

	CString familyNumber;
	CString folyt;

	CString husband_id;
	CString wife_id;
	
	CString rowid;
	int	parentIndex;
	int z;

	std::vector<CString> v_rowid;

	CProgressWnd wndP(NULL, L"Folytatás táblák összekötése (folyt XXX) ...");
	wndP.GoModal();

	/// lekérdezi azokat a szülõket ( apa vagy anya), akiknél elágazás van, tehát gyerekeik egy másik,folyt-ban megadott táblában vannak,
	/// annak a táblának elsõ-generációs leszármazottai, amit a tableRoman == folyt értékkel jelölünk. (lásd processDescendtSubstring )
	m_command.Format(L"SELECT rowid, familyNumber, folyt, last_name, first_name, sex_id FROM people WHERE folyt!=''");
	if (!query(m_command)) return;
	wndP.SetRange(0, m_recordset.RecordsCount());
	wndP.SetPos(0);
	wndP.SetStep(1);


	if (!theApp.execute(L"BEGIN")) return;
	// minden folytatás pontra megkeresi az ember házastársait, majd a gyerekeik father_id, mother_id mezõibe UPDATE-eli ezeket.
	for (UINT i = 0; i < m_recordset.RecordsCount(); ++i, m_recordset.MoveNext())
	{
		familyNumber = m_recordset.GetFieldString(1);
		folyt = m_recordset.GetFieldString(2);
		sexidParent1 = m_recordset.GetFieldString(5);

		
		// apa rowid-ja
		rowidParent1	= m_recordset.GetFieldString(0);
		if (rowidParent1 == L"23525")
			z = 1;
		if (sexidParent1 == MAN )
			rowidFather = rowidParent1;
		else
			rowidMother = rowidParent1;

		// anya rowid meghatározása
		// apa feleségeinek lekérdezése és rowid-jük gyûjtése a v_rowid vektorban
		m_command.Format(L"SELECT husband_id, wife_id FROM marriages WHERE husband_id ='%s' OR wife_id='%s'", rowidParent1, rowidParent1 );
		if (!query2(m_command)) return;

		v_rowid.clear();
		for (UINT i = 0; i < m_recordset2.RecordsCount(); ++i, m_recordset2.MoveNext())
		{
			husband_id = m_recordset2.GetFieldString(0);
			wife_id = m_recordset2.GetFieldString(1);
			if (husband_id == rowidParent1)
				rowidParent2 = wife_id;
			else
				rowidParent2 = husband_id;
			v_rowid.push_back(rowidParent2);
		}


		// A folyt tábla elsõ generációs gyerekei közül a tableRoman == folyt t kérdezi le
		m_command.Format(L"SELECT rowid, parentIndex FROM people WHERE familyNumber='%s' AND tableRoman='%s'", familyNumber, folyt);
		if (!query3(m_command)) return;

		for (UINT i = 0; i < m_recordset3.RecordsCount(); ++i, m_recordset3.MoveNext())
		{
			rowid = m_recordset3.GetFieldString(0);
			parentIndex = _wtoi(m_recordset3.GetFieldString(1)) - 1;
			if (parentIndex < v_rowid.size())
			{
				if (sexidParent1 == MAN )
					rowidMother = v_rowid.at(parentIndex);
				else
					rowidFather = v_rowid.at(parentIndex);
			}
			m_command.Format(L"UPDATE people SET father_id = '%s', mother_id='%s' WHERE rowid = '%s'", rowidFather, rowidMother, rowid);
			if (!theApp.execute(m_command)) return;

		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	if (!theApp.execute(L"COMMIT")) return;
	wndP.DestroyWindow();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::connectCsalad()
{
	CConnectCsalad conn;
	conn.connectCsalad( TRUE );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::peerHeritage()
{
	CString peer;
	CString rowid;
	CString father_id;
	int z;

	CProgressWnd wndP(NULL, L"Fõnemesi rangok örökítése... ");
	wndP.GoModal();


	theApp.execute(L"BEGIN");
	// az apák örökítik fõnemesi címüket gyerekeikre,ha nekik maguknak nincs.
	m_command.Format(L"SELECT rowid FROM people WHERE sex_id = '1' ORDER BY rowid");
	if (!theApp.query(m_command))return;

	wndP.SetRange(0, theApp.m_recordset->RecordsCount() );
	wndP.SetPos(0);
	wndP.SetStep(1);
	for ( int i = 0; i < theApp.m_recordset->RecordsCount(); ++i)
	{
		father_id = theApp.m_recordset->GetFieldString(0);
	
		// apa újboli lekérdezése, hogy a cím örökösei is tovább örökítsék a címet!!
		m_command.Format(L"SELECT peer FROM people WHERE rowid = '%s'", father_id );
		if (!theApp.query1(m_command))return;

		if (father_id == L"16")
			z = 1;

		// az öröklött fõnemesi címek rövidítve lesznek
		peer = theApp.m_recordset1->GetFieldString(0);  // apa fõnemesi címe
		if (!peer.IsEmpty())
		{
			if (iswalpha(peer[0]) )
			{
				if (peer == L"báró") peer = L"br";
				else if (peer == L"gróf") peer = L"gr";
				else if (peer == L"herceg")peer = L"hg";;
				// csak azok a gyerekek öröklik a fõnemnesi címet, akiknek saját joguk alapján nincs
				m_command.Format(L"UPDATE people SET peer='%s' WHERE father_id ='%s' AND peer=''", peer, father_id);
				if (!theApp.execute(m_command))return;
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
		theApp.m_recordset->MoveNext();
	}
	wndP.DestroyWindow();
	theApp.execute(L"COMMIT");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputGA::query( CString command )
{
	if( m_recordset.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputGA::query1( CString command )
{
	if( m_recordset1.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset1.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputGA::query2( CString command )
{
	if( m_recordset2.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CInputGA::query3( CString command )
{
	if( m_recordset3.Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset3.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A soremlés és más kiszûrése
CString CInputGA::cleanHtmlTags(int lineNumber, CString cLine)
{
	CString str;
	CString str1;
	CString str2;
	CString tag;
	int	pos;
	int pos1 = 0;
	int pos2 = 0;


	setGenCnt(cLine);

	while ((pos1 = cLine.Find('<', pos1)) != -1)
	{
		str1 = cLine.Left(pos1);
		if ((pos2 = cLine.Find('>', pos1)) == -1)
		{
			str.Format(L"A '<' karakter után nincs '>' karakter!!\nL%d %s", lineNumber, cLine);
			AfxMessageBox(str, MB_ICONERROR);
			return L"";
		}
		str2 = cLine.Mid(pos2 + 1);
		cLine = str1 + str2;
	}

	tag = getUntil(cLine, ';');
	if ((pos = cLine.Find(';')) != -1)
		if (pos != cLine.GetLength())
			cLine = dropUntil(cLine, ';');

	str.Empty();
	if (!cLine.IsEmpty())
		if (cLine.GetAt(0) != '%')
			str.Format(L"%s %s", tag.Left(1), cLine);
		else
			str = cLine;
	return str;
}////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Csak kisérlet: a táblán belüli generációs sorszám meghatározása a behúzásokból
int CInputGA::setGenCnt(CString line)
{
	int pos;
	int start = 0;

	while ((pos = line.Find(L"<ol>", start)) != -1)
	{
		start = pos + 1;
		++m_genCnt;
	}
	
	start = 0;
	while ((pos = line.Find(L"</ol>", start)) != -1)
	{
		start = pos + 1;
		--m_genCnt;
	}
	return m_genCnt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Név és az azt követõ leírás szétválasztása
void CInputGA::splitNameComment()
{
	int pos;
	CString str1;
	CString str2;
	CStringArray A;
	int n;

	m_name.Empty();
	m_comment.Empty();
	// a nevet veszzõ választhatja el az azt követõ nagybetûvel kezdõdõ leírástól
	if ((pos = m_nameSubstr.Find(',')) != -1)
	{
		str1 = m_nameSubstr.Left(pos);
		str2 = m_nameSubstr.Mid(pos);   // vesszõvel együtt szedi le!
		// Ha avesszõ elõtti és utáni szó is nagybetûvel kezdõdik
		if (iswupper(str1.GetAt( str1.GetLength() -1 )) && iswupper(m_nameSubstr.GetAt(pos + 2)))
		{
			m_name = str1;
			m_comment = str2;
			return;
		}
	}

	// Nincs vesszõ, vagy van, de az nem nagybetûs szavakat választ el
	// akkor a név az utolsó keresztnévig tart vagy az ezt követõ utótagig, amit nem keresztnév követ

	n = wordList(&A, m_nameSubstr, ' ', false);
	n = getSuffix(&A, 1);
	m_name = packWords(&A, 0, n);
//	if (m_name.IsEmpty())
//		m_name = L"N N";
//		n = 1;

}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
// a fõnemnesi címek 3 helyen vannak:
// 1. title
// 2. posterior
// 3. comment 2.,4.,6. szavában
CString CInputGA::setPeer(PEOPLE* p)
{
	CString str;
	CString peer;
	int pos;

	if (p->birth_date == L"1656.04.26")
		pos = 1;

	if (!p->comment.IsEmpty())
	{
		peer = getPeerFromComment(p->comment);
	}
	if (peer.IsEmpty())
	{
		peer = extractPeer(p->title);
	}
	if (peer.IsEmpty())
	{
		str = p->posterior;
		if (str == L"gr" || str == L"õrgr" || str == L"br" || str == L"hg" || str == L"gróf" || str == L"õrgróf" || str == L"báró" || str == L"herceg")
		{
			peer = str;
			p->posterior.Empty();
		}
		str = p->comment;
		if (str == L"gr" || str == L"õrgr" || str == L"br" || str == L"hg" || str == L"gróf" || str == L"õrgróf" || str == L"báró" || str == L"herceg")
		{
			peer = str;
			p->posterior.Empty();
		}
	}
	return peer;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//typedef struct
//{
//	CString place;
//	CString date;
//	CString comment;
//}PLACE_DATE_BLOCK;

// Called from:	splitDesendantsubstring
//				splitMarriagesVector
//				splitPeopesString

// [place][date][comment] blokkot felbontja elemeire.
// Ha a place után ',' van, akkor az utána álló szám már comment
// Ha talál numerikus szót, akkor az elõtte lévõ string a place, az utána lévõ a comment
// Ha nem talál numerikus szót, akkor a place-ben adja vissza az egész stringet
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::processPlaceDateComment(CString placeDateComment, PLACE_DATE_BLOCK* ns)
{
	ns->place.Empty();
	ns->date.Empty();
	ns->comment.Empty();

	if (placeDateComment.IsEmpty()) return;

	CStringArray A;
	CString word;
	CString date;
	CString datum;
	CString place;
	CString comment;
	int i;
	int pos;
	int	ret;

	int n = splitCString(placeDateComment, ' ', false, &A);


	// megkeresi az elsõ dátumot szót
	for (i = 0; i < n; ++i)
	{
		ret = isDate(&A, i, &datum);
		datum.Remove(',');
		if (ret)  // van datum!!
		{
			place = packWords(&A, 0, i);			// dátum elõtt 'place'  (ha van)
			if (place == L"kb") place.Empty();

			if (place.Right(1) == ',')
			{
				place = place.Left(place.GetLength() - 1);
				if (place == L"kb") place.Empty();
				ns->comment = datum;
				ns->comment += L" ";
			}
			else
			{
				ns->date = datum;
			}
			ns->place = place;
			if ((i + ret) < n)
			{
				if (A[i + ret] == L"éves" || A[i + ret] == L"napos")	// x éves || x napos
				{
					ns->date.Empty();								// date nem lesz
					--i;											// comment: x éves || x napos
				}
				ns->comment += packWords(&A, i + ret, n - (i + ret));
			}
			return;
		}
	}


	// Nem talált numerikus szót a placeDateComment stringben. Csak place[, comment] van!!!
	// Ha van comment, akkor vesszõnek kell lenni a place és comment között!!
	if ((pos = placeDateComment.Find(',')) != -1)
	{
		ns->place = placeDateComment.Left(pos);
		ns->comment = placeDateComment.Mid(pos + 2);   // 2: mert , és szóköz is van!
	}
	else
		ns->place = placeDateComment;   // nem talált vesszõt, az egész sor place
}
//////////////////////////////////////////////////////////////////////////////////////////////
// nameSubStr-bõl elõállított 
// visszaadja a posterior kezdetének indexét és a posterior-t a posteriorba teszi
// vissza: 'ret' szóból áll a név, megfosztva az esetleges peer-tõl és posterior-tól
int CInputGA::getSuffix(CStringArray* A, int k )
{

	int i;
	int j;
	int n = A->GetCount();
	int ret = n;
	CString word;
	CString res;
	CString sex;
	bool	comma = false;
	TCHAR ch;

 	m_comment.Empty();
	m_posterior.Empty();
	m_peer.Empty();
	if (!n) return 0;
	// gr Vicenti Salvi Mária
	if (isPeer(A->GetAt(0)))
	{
		m_peer = A->GetAt(0);
		++k;
	}


	for (i = k; i < n; ++i)  // a keresztnév keresése a k. szótól
	{
		word = A->GetAt(i);
		sex = theApp.isFirstName(word);  // elteker az elsõ keresztnévig
	
		if( !sex.IsEmpty())				// keresztnév
				break;
	}
  	if (i + 1 < n)		// talát keresztnevet és utána még van valami
	{
		// elteker az ezt követõ elsõ nem keresztnévig ( kivétel a Márkus és Batizfalvi típusú név, amin átmegy  )
		for (++i; i < n; ++i)
		{
			if (A->GetAt(i) != L"és")			// Márkus és Batizfalvi ....
			{
				word = A->GetAt(i);
				sex = theApp.isFirstName(word);  // elteker az ezt követõ elsõ nem keresztnévig
				if( sex.IsEmpty() )              // nem keresztnév 
					break;
			}
		}
		ret = i;	// az utolsó keresznév utáni szó indexe

		if ((ch = getLastCharacter( A->GetAt( i-1 ))) == ',')
			comma = true;		// a keresztnév után , következik!!

		if (i < n) // az utolsó keresztnév után talált valamit, ami nem keresztnév
		{
			if (!m_birthSubstr.IsEmpty() && !m_deathSubstr.IsEmpty())
				m_posterior = packWords(A, i, n - i);		// akkor csak utótag lehet
			else
			{
				m_comment = packWords(A, i, n - i);		// ez akkor mind leírás, amiben lehet fõnemesi cím is!!!	


				if (comma)
					m_comment.Format(L", %s", (CString)m_comment);
			}

			word = A->GetAt(i);								// keresztnév utáni szó
			if (isPeer(word))								// ez vagy peer
			{
				m_peer = getPeerFromComment( word );		// csak azért, hogy a gróf-gr stb legyen
				++i;
			}
			if (isPeer(m_comment))
			{
				m_peer = m_comment;
				m_comment.Empty();
			}

			if (m_posterior == m_peer)
				m_posterior.Empty();

/*
			m_posterior = packWords(A, i, n - i);
			// számtól kezdve nem posterior, hanem comment
			for (j = i+1; j < n; ++j)
			{
				word = A->GetAt(j);
				if (isdigit(word.GetAt(0)) )
				{
					m_posterior = packWords(A, i,  (j - i ) );
//					m_comment = packWords(A, j, n - j );		// vagy posterior
					break;
				}
			}
*/			
		}
	}
	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////////
int CInputGA::getFirstName(CStringArray* A, int i, CString* firstname, CString* sex)
{
	CString word;
	int cnt = 0;
	CString ret;
	int n = A->GetCount();

	for (i; i < n; ++i) // összeszedi a keresztneveket
	{
		word = A->GetAt(i);
		//		word.Remove(',');
		if ((ret = theApp.isFirstName(word)) != L"" )
		{
			*firstname += word;
			*firstname += L" ";
			*sex = ret;
			++cnt;
		}
		else
			break;
	}
	*firstname->TrimRight();
	return cnt;
}
