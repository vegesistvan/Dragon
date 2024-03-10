// InputGedcom.cpp : implementation file
//
#include "pch.h"
#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "GEDCOMInput.h"
#include <algorithm>

#include "ProgressWnd.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnGedcomInput()
{
	CGEDCOMInput dlg;
	if (dlg.DoModal() == IDCANCEL) return;
	OnDisplayIndividuals();
	mainTitle();
	ChangeMenu();
}
void CDragonDlg::OnGedcomVindi()
{
	CGEDCOMInput ged;
	ged.listVindi();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnGedcomVFAM()
{
	CGEDCOMInput ged;
	ged.printFAM();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnGedcomINDIFAMS()
{
	CGEDCOMInput ged;
	ged.indiFams();
}
void CDragonDlg::OnListPeople()
{
	CGEDCOMInput ged;
	ged.listPeople();
}
void CDragonDlg::OnListFamilyByName()
{
	CGEDCOMInput ged;
	ged.listFamiliesByName();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

// A ga.html list�ban az egy ap�hoz tartoz� gyerekek sorrendj�t a 'people.orderFather' hat�rozza meg. Ennek hi�ny�ban az 
// adatb�zisban elfoglalt sorrendben lesznek a gyerekek.
// Sz�p lenne, ha a gyerekek sz�let�si sorrendben lenn�nek, de a sz�let�s d�tum�nak hi�ny�ban ez nem mindig biztos�that�.
// Az viszont k�vetelm�ny, hogy az egy any�hoz tartoz� gyerekek egym�s ut�n ker�ljenek list�z�sra a ga.html f�jlban, azaz 
// az apa t�bb h�zass�g�b�l sz�rmaz� gyerekek ne keveredjenek, az egy h�zass�gb�l val� gyerekek egym�s ut�n legyenek list�zva.
// Mivel az �jabb inform�ci�k alapj�n ez a sorrend v�ltozhat, k�v�natos hogy ne hagyatkozzunk az adatb�zisban val� sorrendhez,
// hanem 'people.orderFather' mez� korrekt kit�lt�s�vel, updatel�s�vel biztos�tsuk a helyes sorrendet. 
// 
// A ged f�jlokban a FAM rekordok alapj�n hat�rozhatjuk meg a gyerekek 'oredrFather' �rt�k�t. 
//
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CGEDCOMInput, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGEDCOMInput::CGEDCOMInput(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GEDCOM_INPUT, pParent)
{
	m_recordset = new CSqliteDBRecordSet;
	REFRESH = FALSE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGEDCOMInput::~CGEDCOMInput()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMMENT, colorComment);
	DDX_Text(pDX, IDC_GED, theApp.m_gedPathName);
	DDX_Control(pDX, IDC_STATIC_GED, colorGED);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGEDCOMInput, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGEDCOMInput::OnBnClickedOk)
	ON_STN_CLICKED(IDC_STATIC_GED, &CGEDCOMInput::OnClickedStaticGed)
END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGEDCOMInput::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	colorComment.SetTextColor(RED);
	colorGED.SetTextColor(theApp.m_colorClick);

	SetWindowTextW(L"GEDCOM f�jl beolvas�sa adatb�zisba");
	if (!_waccess(theApp.m_gedPathName, 0))
		GetDlgItem(IDC_GED)->SetWindowTextW(theApp.m_gedPathName);
	setDB();

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGEDCOMInput::setDB()
{
	GetDlgItem(IDC_DATABASE)->SetWindowTextW(theApp.m_dbPathName);
	m_command = L"SELECT count() FROM people";
	if (!query(m_command)) return FALSE;

	m_count = _wtoi(m_recordset->GetFieldString(0));
	if (m_count)
	{
		str.Format(L"Az adatb�zisban %d ember van nyilv�ntartva! T�r�lni fogom �ket!!", m_count );
		GetDlgItem(IDC_COMMENT)->SetWindowTextW( str );
	}
	else
	{
		GetDlgItem(IDC_COMMENT)->SetWindowTextW(L"Az adatb�zis �res" );
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGEDCOMInput::query(CString command)
{
	if (m_recordset->Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Az adatb�zis t�rl�se
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::emptyDatabase()
{
	GetDlgItem(IDOK)->EnableWindow(FALSE);
	GetDlgItem(IDCANCEL)->EnableWindow(FALSE);

	theApp.clearDatabase();
	setCreationTime(theApp.m_dbPathName);

	GetDlgItem(IDCANCEL)->EnableWindow(TRUE);
	GetDlgItem(IDOK)->EnableWindow(TRUE);
	str = L"Az adatb�zis �res.";
	GetDlgItem(IDC_COMMENT)->SetWindowTextW(str);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::OnClickedStaticGed()
{
	if (theApp.selectGedcom())
		GetDlgItem(IDC_GED)->SetWindowTextW(theApp.m_gedPathName);
//	setDB();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::OnBnClickedOk()
{
	GetDlgItem(IDC_GED)->GetWindowTextW(str);
	if (str.IsEmpty())
	{
		AfxMessageBox(L"A bementi f�jlt meg kell adni!");
		return;
	}
	CDialogEx::OnOK();
	gedcomInput();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CGEDCOMInput::gedcomInput()
{
	if (m_count)
		emptyDatabase();

	CProgressWnd wndP(NULL, L"GEDCOM f�jl beolvas�sa...");
	wndP.GoModal();

	if (!theApp.gedFileExists()) return false;

	CString drive;
	CString path;
	CString filename;
	CString	ext;

	splitFilespec(theApp.m_gedPathName, &drive, &path, &filename, &ext);

	theApp.m_inputCode = gedCHAR(theApp.m_gedPathName);
	if (theApp.m_inputCode == -1) theApp.m_inputCode = GetInputCode(theApp.m_gedPathName);

	file_ged.Open(theApp.m_gedPathName, CFile::modeRead);
	if (theApp.m_inputCode == UTF8BOM)	file_ged.Seek(3, SEEK_SET);


	m_blobFolder.Format(L"%s\\%s_BLOB", theApp.m_gedFolderPath, filename);
	if (_waccess(m_blobFolder, 0)) _wmkdir(m_blobFolder);

	wndP.DestroyWindow();



	if (!fillVectors()) return false;

	theApp.insertIntoFiles(theApp.m_gedPathName, GEDCOM_FILE);
	theApp.m_inputMode = GEDCOM;

	insertPeople();
	updateParents();
	insertMarriages();

	if( AfxMessageBox( L"Az adatb�zis felt�lt�se befez�d�tt.\nList�zzam a vektorokat?", MB_ICONQUESTION|MB_YESNO ) == IDYES )
		printVectors();

	return true;
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A v_indi vektorb�l minden embert a people t�bl�ba insert�l
void CGEDCOMInput::insertPeople()
{
	CString id;
	CString fields;
	CString values;
	CString filespec;
	CString filename;
	CString ext;
	int		pos;



	fields.Format(L"\
fileNumber,\
linenumber,\
tableNumber,\
sex_id,\
title,\
titolo,\
first_name,\
last_name,\
posterior,\
birth_place,\
birth_date,\
death_place,\
death_date,\
parentIndex,\
comment,\
source,\
occupation,\
orderFather,\
orderMother,\
indi\
");

	int parentIndex;
	CString lastname;
	CString firstname;





	CProgressWnd wndP(NULL, L"Emberek adatb�zisba t�lt�s...");
	wndP.GoModal();
	wndP.SetRange(0, v_indi.size());
	wndP.SetPos(0);
	wndP.SetStep(1);

	if (!theApp.execute(L"BEGIN")) return;
	for (UINT i = 0; i < v_indi.size(); ++i)
	{
		/*
				lastname	= v_indi.at(i).last_name;
				lastname	= lastname.MakeLower();
				kar			= towupper(lastname[0]);
				lastname	= kar + lastname.Mid(1);
		*/
		firstname = v_indi.at(i).first_name;
		lastname = v_indi.at(i).last_name;


//		if ((pos = lastname.Find('(')) != -1 )
//		{
//			lastname = lastname.Left(pos);
//			lastname.Trim();
//		}
		lastname.Trim();
		if (lastname.IsEmpty() )
			lastname = L"N";

		firstname.Trim();
		if (firstname.IsEmpty())
			firstname = L"N";


		id = v_indi.at(i).refI;
		parentIndex = v_indi.at(i).parentIndex;
		values.Format( L" 1, '%d', '%s', '%s','%s','%s', '%s','%s','%s','%s','%s','%s','%s','%d','%s','%s','%s','%d','%d', '%s'",
		v_indi.at(i).linenumber,\
		v_indi.at(i).tablenumber,\
		v_indi.at(i).sex,\
		v_indi.at(i).title,\
		v_indi.at(i).titolo,\
		firstname,\
		lastname,\
		v_indi.at(i).posterior,\
		v_indi.at(i).birth_place,\
		v_indi.at(i).birth_date,\
		v_indi.at(i).death_place,\
		v_indi.at(i).death_date,\
		parentIndex,\
		v_indi.at(i).comment,\
		v_indi.at(i).source,\
		v_indi.at(i).occupation,\
		v_indi.at(i).orderFather,\
		v_indi.at(i).orderMother,\
		v_indi.at(i).refI\
		);
		m_command.Format( L"INSERT INTO people (%s) VALUES (%s)", fields, values );
		if( !theApp.execute( m_command ) ) return;
		++theApp.m_cntPeople;

//		m_command = L"SELECT last_insert_rowid() FROM people";
//		if( ! theApp.query( m_command ) ) return;
//		v_indi.at(i).rowid = theApp.m_recordset->GetFieldString(0);

		str.Format(L"%d", i + 1);
		v_indi.at(i).rowid = str;


		CString https;
		int cnt = 1;
	
		for( UINT j = 0; j < vPhotos.size(); ++j )
		{
			if( vPhotos.at(j).refI == id )   // az emberhoz tartoz� k�pekhez beteszi a rowidP �s f�jlnevet.
			{
				str = vPhotos.at(j).link;
				ext.Empty();
				if( (pos = str.ReverseFind( '.' )) != -1 )
				{
					ext = str.Mid( pos + 1 ).MakeLower();
				}
				filename.Format( L"%s %s_%s_%d.%s", v_indi.at(i).last_name, v_indi.at(i).first_name, v_indi.at(i).rowid, cnt, ext );
				vPhotos.at(j).fileName	= filename;
				vPhotos.at(j).rowidP	= v_indi.at(i).rowid;
				if( cnt == 1 ) 
					vPhotos.at(j).primaryPic = 1;
				else
					vPhotos.at(j).primaryPic = 0;
				++cnt;
			}
		}

		
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();

	if( !theApp.execute( L"COMMIT" ) ) return;
	
	
	CString rowidP;
	if( !theApp.executeBlob( L"BEGIN" ) ) return;
	for( UINT j = 0; j < vPhotos.size(); ++j )
	{
		str = vPhotos.at(j).link;
		if( str.Left(5) == L"https" ) // neten tal�lhat� k�p van megadva ( a f�jln�v semmit mond�, ez�rt mi adunk neki
		{
			filespec.Format( L"%s\\%s", m_blobFolder, vPhotos.at(j).fileName  );   // el�sz�r beolvassa ebbe a f�jlba, majd insert�lja a k�pet
			insertHttps( vPhotos.at(j).rowidP, filespec, vPhotos.at(j).link, j+1 );
		}
		else if( str.Left(1) == L"." )		// a ged folderhez k�pest relat�v �tvonallal van megadva a k�p
		{
			filename = str.Mid(1);
			filespec.Format( L"%s%s", theApp.m_gedFolderPath, filename );
			insertFile( vPhotos.at(j).rowidP, filespec,  j );
		}
		else if( str.GetAt(1) == ':' )			// abszul�t �tvonallal van megadva 
		{
			filespec = vPhotos.at(j).link;
			insertFile( vPhotos.at(j).rowidP, filespec,  j );
		}

	}
	if( !theApp.executeBlob( L"COMMIT" ) ) return;

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A people t�bl�ban l�v� emberek father_id �s mother_id �rt�keit update-eli a v_fam �s v_chil alapj�n
void CGEDCOMInput::updateParents()
{
	CString refF;
	CString xref_child;

	CString rowid_father(L"");
	CString rowid_mother(L"");
	CString rowid_child(L"");

	CProgressWnd wndP(NULL, L"Sz�l�k �s gyerekek �sszekapcsol�sa...");
	wndP.GoModal();
	wndP.SetRange(0, v_fam.size());
	wndP.SetPos(0);
	wndP.SetStep(1);

	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < v_fam.size(); ++i )
	{
		refF		= v_fam.at(i).refF;
		rowid_father = getRowid( v_fam.at(i).refH );
		rowid_mother = getRowid( v_fam.at(i).refW );
		rowid_mother.Trim();
		for( UINT j = 0; j < v_chil.size(); ++j )
		{
			if( v_chil.at(j).refF == refF )
			{
				xref_child = v_chil.at(j).refC;
				for( UINT k = 0; k < v_indi.size(); ++k )
				{
					if( v_indi.at(k).refI == xref_child )
					{
						rowid_child = v_indi.at(k).rowid;
						m_command.Format( L"UPDATE people SET father_id='%s', mother_id='%s' WHERE rowid = '%s'", rowid_father, rowid_mother, rowid_child );
						if( !theApp.execute( m_command ) ) return;
						break;
					}
				}
			}
		}
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	if( !theApp.execute( L"COMMIT" ) ) return;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// A marriages t�bl�ba insert�lja a v_fam-b�l a h�zass�gokat
// orderHusband �s orderWife a leveg�ben l�g
void CGEDCOMInput::insertMarriages()
{
	CString spouse1_id;
	CString spouse2_id;
	CString place;
	CString date;
	int orderHusband;
	int orderWife;

	CString fields;
	CString values;

	fields.Format( L"\
fileNumber,\
place,\
date,\
spouse1_id,\
spouse2_id,\
orderHusband,\
orderWife\
");

	CProgressWnd wndP(NULL, L"H�zast�rsak �sszekapcsol�sa...");
	wndP.GoModal();
	wndP.SetRange(0, v_fam.size());
	wndP.SetPos(0);
	wndP.SetStep(1);

	if( !theApp.execute( L"BEGIN" ) ) return;
	for( UINT i = 0; i < v_fam.size(); ++i )
	{
		spouse1_id	= getRowid( v_fam.at(i).refH );
		spouse2_id	= getRowid( v_fam.at(i).refW );
		place		= v_fam.at(i).place;
		place.Replace( '\'', '|' );
		date		= date2date( v_fam.at(i).date );
		orderHusband = v_fam.at(i).marriageH;				// f�rj h�zass�g�nak sorsz�ma
		orderWife	 = v_fam.at(i).marriageW;				// feles�g h�zass�g�nak sorsz�ma

		values.Format( L" 1, '%s','%s', '%s','%s','%d','%d'",
		place,\
		date,\
		spouse1_id,\
		spouse2_id,
		orderHusband,\
		orderWife\
		);
		m_command.Format( L"INSERT INTO marriages (%s) VALUES (%s)", fields, values );
		if( !theApp.execute( m_command ) ) return;

		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}
	wndP.DestroyWindow();
	if( !theApp.execute( L"COMMIT" ) ) return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::insertHttps( CString rowid, CString fileSpec, CString url, int primary )
{
	BLOBSTAT stat;
	HRESULT hr;
	bool	error = false;
	LPCTSTR Url		= url;
	LPCTSTR File	= fileSpec;

	hr = URLDownloadToFile (0, Url, File, 0, 0);
	switch (hr)
	{
		case S_OK:
			break;
		case E_OUTOFMEMORY:
			AfxMessageBox( L"Out of memory error" );
			error = true;
			break;
		case INET_E_DOWNLOAD_FAILURE:
			AfxMessageBox( L"Cannot access server data" );
			error = true;
			break;
		default:
			AfxMessageBox( L"Unknown error" );
			error = true;
			break;
	}

	if( !error )
	{
		theApp.selectFile( fileSpec, &stat );
		int rowidI = theApp.blobDB->blobInsert( "pictures", "picture", &stat );

		if( rowidI )
		{
			m_command.Format( L"UPDATE pictures SET id='%s', filename='%s', ext='%s', size='%d', table_id=1, title='%s', primaryPic = 1  WHERE rowid='%d'", rowid,  stat.filename, stat.ext, stat.size, stat.filename,  rowidI );
			if( !theApp.executeBlob( m_command ) ) return;
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGEDCOMInput::insertFile( CString rowid, CString fileSpec, int primary )
{
	BLOBSTAT stat;

	if( _waccess( fileSpec, 0 ) ) return;

	theApp.selectFile( fileSpec, &stat );
	int rowidI = theApp.blobDB->blobInsert( "pictures", "picture", &stat );

	if( rowidI )
	{
		m_command.Format( L"UPDATE pictures SET id='%s', filename='%s', ext='%s', size='%d', table_id=1, title='%s', primaryPic = 1  WHERE rowid='%d'", rowid,  stat.filename, stat.ext, stat.size, stat.filename,  rowidI );
		if( !theApp.executeBlob( m_command ) ) return;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
