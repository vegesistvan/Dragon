// GedcomOut.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "GEDCOMOut.h"
#include "afxdialogex.h"
#include <algorithm>
#include "ProgressWnd.h"
#include "utilities.h"
#include "Table_people_columns.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CTablePeople::OnGedcomOutput()
{
	if (theApp.m_inputMode == MANUAL)
		m_ListCtrl.SetSortOrder(2, N_LAST_NAME + 1, N_FIRST_NAME + 1);
	if (theApp.m_inputMode == GEDCOM)
		m_ListCtrl.SetSortOrder(2, G_LAST_NAME + 1, G_FIRST_NAME + 1);
	if (theApp.m_inputMode == GAHTML)
		m_ListCtrl.SetSortOrder(2, H_LAST_NAME + 1, H_FIRST_NAME + 1);

	CGedcomOut dlg;
	dlg.m_filter = m_filterText;
	dlg.p_ListCtrl = &m_ListCtrl;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNAMIC(CGedcomOut, CDialogEx)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGedcomOut::CGedcomOut(CWnd* pParent /*=NULL*/)
	: CDialogEx(CGedcomOut::IDD, pParent)
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CGedcomOut::~CGedcomOut()
{
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CGedcomOut, CDialogEx)
	ON_BN_CLICKED(IDOK, &CGedcomOut::OnBnClickedOk)
END_MESSAGE_MAP()

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CGedcomOut::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	if( m_filter.IsEmpty() ) m_filter = theApp.m_dbFileTitle;
	m_fileName.Format( L"%s.ged", m_filter );
	GetDlgItem( IDC_FILENAME )->SetWindowText( m_fileName );


	m_gedFolderPath.Format( L"%s\\ged", theApp.m_dbFolderPath );
	GetDlgItem( IDC_PATH )->SetWindowText( m_gedFolderPath );
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::OnBnClickedOk()
{
	CProgressWnd wndProgress(NULL, L"GEDCOM fájl készítse..." ); 
	wndProgress.GoModal();


	GetDlgItem( IDC_FILENAME )->GetWindowText( m_fileName );

	if( m_fileName.IsEmpty() ) return;

	if( m_fileName.Find( L".ged" ) == -1 ) m_fileName += L".ged";

	m_gedFolderPath.Format( L"%s\\ged", theApp.m_dbFolderPath );
	if( _waccess( m_gedFolderPath, 0 ) ) _wmkdir( m_gedFolderPath );

	m_relFolder.Format( L"%s_BLOB", m_fileName );

	m_blobFolder.Format( L"%s\\%s", m_gedFolderPath, m_relFolder );
	if( _waccess( m_blobFolder, 0 ) ) _wmkdir( m_blobFolder);
		

	CString fileSpec = theApp.openTextFile( &fl, m_fileName, L"w+" );

	vPhotos.clear();
	fillINDI( p_ListCtrl );
	fillFAM();
	list();

	copyBlobs();

	// ged fájl készítése
	m_gedPathName.Format( L"%s\\%s", m_gedFolderPath, m_fileName );
	if( !openFileSpec( &fg, m_gedPathName, L"w+" ) ) return;
	
	printHeader();
	printINDI();
	printFAM();

	str = L"0 TRLR\n" ;
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );


	wndProgress.DestroyWindow();
	fclose( fg );

	if( theApp.NOTEPAD )
	{
		theApp.notepad( m_gedPathName, L"-alwaysOnTop -nosession" );
		theApp.notepad( fileSpec, L"-alwaysOnTop" );
	}
	else
	{
		str.Format( L"%s\nelkészült!", m_gedPathName );
		AfxMessageBox( str );
	}
	
	CDialogEx::OnOK();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::fillINDI( CListCtrlEx* p_ListCtrl )
{
	CString id;
	CString rowidP;
	CString rowidB;
	int rowid;
	int sex;
	INDI_S vIndi;
	CString givn;
	CString surn;

	int z;
	
	vINDI.clear();
	vIndi.famc_id = nincs;
//	vIndi.fams_id = nincs;
	vIndi.title.Empty();
	vIndi.titolo.Empty();

	for( UINT i = 0; i < p_ListCtrl->GetItemCount(); ++i )
	{
		if( theApp.m_inputMode == MANUAL ) 
		{
			rowidP = p_ListCtrl->GetItemText( i, N_ROWID );

			givn = p_ListCtrl->GetItemText( i, N_FIRST_NAME );
			if( givn.IsEmpty() ) givn = "N";
			vIndi.givn			= givn;

			surn = p_ListCtrl->GetItemText( i, N_LAST_NAME );
			if( surn.IsEmpty() ) surn = "N";
			vIndi.surn = surn;
			if (surn == L"Vingenfeld" && givn == L"Ferenc" )
				z = 1;

			vIndi.birthDate		= p_ListCtrl->GetItemText( i, N_BIRTH_DATE );
			vIndi.birthPlace	= p_ListCtrl->GetItemText( i, N_BIRTH_PLACE );
			vIndi.deathDate		= p_ListCtrl->GetItemText( i, N_DEATH_DATE );
			vIndi.deathPlace	= p_ListCtrl->GetItemText( i, N_DEATH_PLACE );
			str	= p_ListCtrl->GetItemText( i, N_COMMENT );
			vIndi.comment = str;
			vIndi.rowidF		= _wtoi( p_ListCtrl->GetItemText( i, N_FATHER_ID ) );
			vIndi.rowidM		= _wtoi( p_ListCtrl->GetItemText( i, N_MOTHER_ID ) );

			if( _wtoi(p_ListCtrl->GetItemText( i, N_SEX )) == MAN )
				vIndi.sex = 'M';
			else
				vIndi.sex = 'F';
		}
		else if( theApp.m_inputMode == GEDCOM ) 
		{
			rowidP = p_ListCtrl->GetItemText( i, G_ROWID );

			vIndi.title			= p_ListCtrl->GetItemText( i, G_TITLE );
			vIndi.titolo		= p_ListCtrl->GetItemText( i, G_TITOLO );

			givn = p_ListCtrl->GetItemText( i, N_BIRTH_DATE );
			if( givn.IsEmpty() ) givn = "N";
			vIndi.givn			= givn;

			surn = p_ListCtrl->GetItemText( i, G_LAST_NAME );
			if( surn.IsEmpty() ) surn = "N";
			vIndi.surn = surn;

			vIndi.birthDate		= p_ListCtrl->GetItemText( i, G_BIRTH_DATE );
			vIndi.deathDate		= p_ListCtrl->GetItemText( i, G_DEATH_DATE );
			vIndi.birthPlace	= p_ListCtrl->GetItemText( i, G_BIRTH_PLACE );
			vIndi.deathPlace	= p_ListCtrl->GetItemText( i, G_DEATH_PLACE );
			vIndi.comment		= p_ListCtrl->GetItemText( i, G_COMMENT );
			vIndi.rowidF = _wtoi(p_ListCtrl->GetItemText(i, G_FATHER_ID));
			vIndi.rowidM = _wtoi(p_ListCtrl->GetItemText(i, G_MOTHER_ID));


			if( _wtoi( p_ListCtrl->GetItemText( i, G_SEX ) ) == MAN )
				vIndi.sex = 'M';
			else
				vIndi.sex = 'F';
		}

		else if( theApp.m_inputMode == GAHTML ) 
		{
			rowidP = p_ListCtrl->GetItemText( i, H_ROWID );

			vIndi.title			= p_ListCtrl->GetItemText( i, H_TITLE );
			vIndi.titolo		= p_ListCtrl->GetItemText( i, H_TITOLO );


			givn = p_ListCtrl->GetItemText( i, H_FIRST_NAME );
			if( givn.IsEmpty() ) givn = "N";
			vIndi.givn			= givn;

			surn = p_ListCtrl->GetItemText( i, H_LAST_NAME );
			if( surn.IsEmpty() ) surn = "N";
			vIndi.surn = surn;

			vIndi.birthDate		= p_ListCtrl->GetItemText( i, H_BIRTH_DATE );
			vIndi.deathDate		= p_ListCtrl->GetItemText( i, H_DEATH_DATE );
			vIndi.birthPlace	= p_ListCtrl->GetItemText( i, H_BIRTH_PLACE );
			vIndi.deathPlace	= p_ListCtrl->GetItemText( i, H_DEATH_PLACE );
			vIndi.comment		= p_ListCtrl->GetItemText( i, H_COMMENT );
			vIndi.rowidF = _wtoi(p_ListCtrl->GetItemText(i, H_FATHER_ID));
			vIndi.rowidM = _wtoi(p_ListCtrl->GetItemText(i, H_MOTHER_ID));


			if( _wtoi( p_ListCtrl->GetItemText( i, H_SEX ) ) == MAN )
				vIndi.sex = 'M';
			else
				vIndi.sex = 'F';
		}
		vIndi.rowid = _wtoi( rowidP );
		vINDI.push_back( vIndi );
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::fillFAM()
{
	FAM_S vFam;
	CHIL_S vChil;

	int spouse1_id;
	int spouse2_id;
	int rowidH;
	int rowidW;
	int child_id;

	UINT i;
	UINT j;


	CString command = L"SELECT rowid, * FROM marriages";
	if( !theApp.query( command ) ) return;
	
	vFAM.clear();
	
	for( i = 0; i < theApp.m_recordset->RecordsCount(); ++i, theApp.m_recordset->MoveNext() )
	{
		rowidH	= _wtoi(  theApp.m_recordset->GetFieldString(  MARRIAGES_SPOUSE1_ID ) );
		rowidW	= _wtoi(  theApp.m_recordset->GetFieldString(  MARRIAGES_SPOUSE2_ID ) );

		if( rowidH == 648 && rowidW == 651 )
			int p = 2;


		vFam.husb_id = nincs;
		vFam.wife_id = nincs;
		for( j = 0; j < vINDI.size(); ++j )				// férj azonosító számának megkeresése
		{
			if( vINDI.at(j).rowid == rowidH )
			{
				vFam.husb_id = j;
				vFam.rowidH = rowidH;
				break;
			}
		}
		if( j < vINDI.size() )
		{
			for( j = 0; j < vINDI.size(); ++j )
			{
				if( vINDI.at(j).rowid == rowidW )
				{
					vFam.wife_id = j;
					vFam.rowidW = rowidW;
					vFAM.push_back( vFam );
					break;
				}
			}
		}
		vFam.place = theApp.m_recordset->GetFieldString(MARRIAGES_PLACE);
		vFam.date = theApp.m_recordset->GetFieldString(MARRIAGES_DATE);
		
	}

// Azoknak az emebereknek is kellen családod létreozni, akiknek nem ismerjük a feleségét/férjét, vagyis
// nincsenek a marriges táblában, de vannak gyerekei.

	for( i = 0; i < vINDI.size(); ++i )
	{
		int rowidF = vINDI.at(i).rowidF;
		if( rowidF != 0 )				// ennek at embernek van apja!
		{
			for( j = 0; j < vFAM.size(); ++j )		// az apa szerepel-e a FAM vektorban?
			{
				if( rowidF == vFAM.at(j).rowidH )
					break;							// szerepel már a családok között, nem kell foglalkozni vele!!
			}
			if( j == vFAM.size() )					// nem szerepelt az apa a családok között!!
			{
				for( j = 0; j < vINDI.size(); ++j )
				{
					if( rowidF == vINDI.at(j).rowid )
					{
						vFam.husb_id	= j;
						vFam.wife_id	= nincs;
						vFam.rowidH		= rowidF;
						vFam.rowidW		= 0;
						vFAM.push_back( vFam );
						break;
					}
				}
			}
		}

		int rowidM = vINDI.at(i).rowidM;
		if( rowidM != 0 )				// ennek at embernek van anyja!
		{
			for( j = 0; j < vFAM.size(); ++j )		// az anya szerepel-e a FAM vektorban?
			{
				if( rowidM == vFAM.at(j).rowidW )
					break;							// szerepel már a családok között, nem kell foglalkozni vele!!
			}
			if( j == vFAM.size() )					// nem szerepelt az anya a családok között!!
			{
				for( j = 0; j < vINDI.size(); ++j )
				{
					if( rowidM == vINDI.at(j).rowid )
					{
						vFam.husb_id	= nincs;
						vFam.wife_id	= j;
						vFam.rowidH		= 0;
						vFam.rowidW		= rowidM;
						vFAM.push_back( vFam );
						break;
					}
				}
			}
		}
	}

// vCHIL vektor elkészítése
	int rowidF;
	int rowidM;
	int rowidC;
	vCHIL.clear();
	
	// Apja és anyja is ismert
	for( i = 0; i < vFAM.size(); ++i )
	{
		rowidF = 0;
		if( vFAM.at(i).husb_id != nincs )
			rowidF = vINDI.at( vFAM.at(i).husb_id ).rowid;
		rowidM = 0;
		if( vFAM.at(i).wife_id != nincs )
			rowidM = vINDI.at( vFAM.at(i).wife_id ).rowid;

		if( rowidF == 648 && rowidM == 651 )
			int z = 0;
		if( rowidF != 0 && rowidM != 0 )  // Apja és anyja is ismert
		{
			command.Format( L"SELECT rowid FROM people WHERE father_id='%d' AND mother_id = '%d'", rowidF, rowidM );
		}
		else if( rowidF != 0 && rowidM == 0 )  // apa ismmert, anya nem
		{
			command.Format( L"SELECT rowid FROM people WHERE father_id='%d'", rowidF );
		}
		else									// apa nem ismeret, anya ismert
		{
			command.Format( L"SELECT rowid FROM people WHERE mother_id='%d'", rowidM );
		}
		if( !theApp.query1( command ) ) return;
		// a házaspár minden gyerekét beteszi a vCHIH_be

		for( j = 0; j < theApp.m_recordset1->RecordsCount(); ++j, theApp.m_recordset1->MoveNext() )
		{
			vChil.fam_id	= i;
			rowidC = _wtoi( theApp.m_recordset1->GetFieldString( 0 ) );
			for( UINT k = 0; k < vINDI.size(); ++k )
			{
				if( vINDI.at(k).rowid == rowidC )
				{
					vChil.chil_id	= k;
					vCHIL.push_back( vChil );
					break;
				}
			}
		}
	}

	// INDI rekordokba beteszi, hogy melyik családban fordul elõ gyerekként
	for( UINT i = 0; i < vCHIL.size(); ++i )
	{
		vINDI.at( vCHIL.at(i).chil_id ).famc_id = vCHIL.at(i).fam_id;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::copyBlobs()
{
	CString fileName;
	CString ext;
	CString fileSpec;

	CString rowidB;
	CString rowidP;
	CString id;
	int		errno_t;
	_int64 blob_size;
	void* block;
	int nItem;
	FILE* fl;
	PHOTOS photo;

	m_command.Format( L"SELECT rowid,* FROM pictures ORDER BY id" );
	if( !theApp.queryBlob( m_command ) ) return;

	vPhotos.clear();
	for( UINT i = 0; i < theApp.m_recordsetBlob->RecordsCount(); ++i )
	{
		rowidB		= theApp.m_recordsetBlob->GetFieldString( PIC_ROWID );
		rowidP		= theApp.m_recordsetBlob->GetFieldString( PIC_ID );
		ext			= theApp.m_recordsetBlob->GetFieldString( PIC_EXT );
		fileName.Format( L"%s_%s_%d.%s", theApp.m_recordsetBlob->GetFieldString( PIC_FILENAME ), rowidP, i+1, ext );;
		fileSpec.Format( L"%s\\%s", m_blobFolder, fileName );

		block = theApp.blobDB->blobRead( "pictures", "picture", rowidB, &blob_size );
		if( block == NULL ) continue;

		if( !openFileSpec( &fl, fileSpec, L"wb" ) ) continue;
		if( fwrite( block, blob_size, 1, fl ) != 1 )
		{
			AfxMessageBox( L"fwrite failed!" );
			fclose( fl );
			continue;
		}
		fclose( fl );
		photo.rowidP	= rowidP;
		photo.link		= fileName;
		vPhotos.push_back( photo );
		theApp.m_recordsetBlob->MoveNext();
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::printHeader()
{
	str = L"0 HEAD\n" ;
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );
	

	str = L"1 GEDC\n" ;
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );

	str = L"2 VERS 5.5.1\n" ;
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );

	str = "2 FORM LINEAGE-LINKED\n";
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );

	str = "1 CHAR UTF-8\n";
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );

	str = L"1 SOUR FA\n" ;
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );

	str = L"2 CORP fa - Véges István\n" ;
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );

	
	str.Format( L"1 DATE %s\n", dateConv( theApp.getPresentDate() ) );
	str = UnicodeToUtf8( str ); 
	fwprintf( fg, str );

}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::printINDI()
{
	int famc_id;
	int fams_id;
	CString title;
	CString titolo;
	CString birthDate;
	CString birthPlace;
	CString deathDate;
	CString deathPlace;
	CString comment;
	CString file;

	for( UINT i = 0; i < vINDI.size(); ++i )
	{
		str.Format( L"0 @I%05d@ INDI\n", i+1 );
		str = UnicodeToUtf8( str ); 
		fwprintf( fg, str );

		titolo = vINDI.at(i).titolo;
		{
			str.Format( L"1 NAME %s\n", titolo );
			str = UnicodeToUtf8( str ); 
			fwprintf( fg, str );
		}

		title = vINDI.at(i).title;
		if( !title.IsEmpty() )
		{
			str.Format( L"2 NPFX %s\n", title );
			str = UnicodeToUtf8( str ); 
			fwprintf( fg, str );
		}



		str.Format( L"2 GIVN %s\n", vINDI.at(i).givn );
		str = UnicodeToUtf8( str ); 
		fwprintf( fg, str );

		str.Format( L"2 SURN %s\n", vINDI.at(i).surn );
		str = UnicodeToUtf8( str ); 
		fwprintf( fg, str );

		str.Format( L"1 SEX %c\n", vINDI.at(i).sex );
		str = UnicodeToUtf8( str ); 
		fwprintf( fg, str );

		birthDate = vINDI.at(i).birthDate;
		birthPlace = vINDI.at(i).birthPlace;
		if( !birthDate.IsEmpty() || !birthPlace.IsEmpty() )
		{
			str.Format( L"1 BIRT\n" );
			str = UnicodeToUtf8( str ); 
			fwprintf( fg, str );
			if( !birthDate.IsEmpty() )
			{
				birthDate = dateConv( birthDate );
				str.Format( L"2 DATE %s\n", birthDate );
				str = UnicodeToUtf8( str ); 
				fwprintf( fg, str );
			}
			if( !birthPlace.IsEmpty() )
			{
				str.Format( L"2 PLAC %s\n", birthPlace );
				str = UnicodeToUtf8( str ); 
				fwprintf( fg, str );
			}
		}

		deathDate = vINDI.at(i).deathDate;
		deathPlace = vINDI.at(i).deathPlace;
		if( !deathDate.IsEmpty() || !deathPlace.IsEmpty() )
		{
			str.Format( L"1 DEAT\n" );
			str = UnicodeToUtf8( str ); 
			fwprintf( fg, str );
			if( !deathDate.IsEmpty() )
			{
				deathDate = dateConv( deathDate );
				str.Format( L"2 DATE %s\n", deathDate );
				str = UnicodeToUtf8( str ); 
				fwprintf( fg, str );
			}
			if( !deathPlace.IsEmpty() )
			{
				str.Format( L"2 PLAC %s\n", deathPlace );
				str = UnicodeToUtf8( str ); 
				fwprintf( fg, str );
			}
		}

		comment	= vINDI.at(i).comment;
		if( !comment.IsEmpty() )
		{
			str.Format( L"2 NOTE %s\n",comment );
			str = UnicodeToUtf8( str ); 
			fwprintf( fg, str );
		}

		famc_id = vINDI.at(i).famc_id;
		if( famc_id != nincs )
		{
			str.Format( L"1 FAMC @F%05d@\n", famc_id + 1 );
			str = UnicodeToUtf8( str ); 
			fwprintf( fg, str );
		}

		for( UINT j = 0; j < vFAM.size(); ++j )
		{
			if( vFAM.at(j).husb_id == i || vFAM.at(j).wife_id == i  )
			{
				str.Format( L"1 FAMS @F%05d@\n", j + 1 );
				str = UnicodeToUtf8( str ); 
				fwprintf( fg, str );
			}
		}


		for( UINT j = 0; j < vPhotos.size(); ++j )
		{
			if( _wtoi(vPhotos.at(j).rowidP ) == vINDI.at(i).rowid )
			{
				str.Format( L"1 FILE .\\%s\\%s\n", m_relFolder, vPhotos.at(j).link );
				str = UnicodeToUtf8( str ); 
				fwprintf( fg, str );
			}
		}
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::printFAM()
{
	int cnt = 1;
	int husbIndex;
	int wifeIndex;
	CString place;
	CString date;
	for( UINT i = 0; i < vFAM.size(); ++i )
	{
		husbIndex = vFAM.at(i).husb_id;
		wifeIndex = vFAM.at(i).wife_id;
		place = vFAM.at(i).place;
		date = vFAM.at(i).date;

		if( husbIndex == nincs && wifeIndex == nincs ) continue;  // most hagyja ki azokat a 'családokat', ahol nincs gyerek

		str.Format( L"0 @F%05d@ FAM\n", cnt );
		str = UnicodeToUtf8( str ); 
		fwprintf( fg, str );

		str.Format( L"1 HUSB @I%05d@\n", husbIndex + 1 );
		str = UnicodeToUtf8( str ); 
		fwprintf( fg, str );

		if( wifeIndex != nincs )
		{
			str.Format( L"1 WIFE @I%05d@\n", wifeIndex + 1 );
			str = UnicodeToUtf8( str ); 
			fwprintf( fg, str );
		}

		for( UINT j = 0; j < vCHIL.size(); ++j )
		{
			if( vCHIL.at(j).fam_id == i )
			{
				str.Format( L"1 CHIL @I%05d@\n", vCHIL.at(j).chil_id + 1);
				str = UnicodeToUtf8( str ); 
				fwprintf( fg, str );
			}
		}

		if (!date.IsEmpty())
		{
			str.Format(L"1 DATE %s\n", date );
			str = UnicodeToUtf8(str);
			fwprintf(fg, str);
		}
		if (!place.IsEmpty())
		{
			str.Format(L"1 PLAC %s\n", place);
			str = UnicodeToUtf8(str);
			fwprintf(fg, str);
		}


		++cnt;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// input: YYYY.MM.DD
// output: DD monthes YYYY
CString CGedcomOut::dateConv( CString input )
{
	CString date = input;
	CStringArray monthes;

	monthes.Add( L"JAN" );
	monthes.Add( L"FEB" );
	monthes.Add( L"MAR" );
	monthes.Add( L"APR" );
	monthes.Add( L"MAY" );
	monthes.Add( L"JUN" );
	monthes.Add( L"JUL" );
	monthes.Add( L"AUG" );
	monthes.Add( L"SEP" );
	monthes.Add( L"OCT" );
	monthes.Add( L"NOV" );
	monthes.Add( L"DEC" );

	CString year;
	CString day;
	int month;

	input.Trim();
	if( input.GetLength() == 10 )
	{
		year = input.Left( 4 );
		day = input.Mid( 8 );
		month = _wtoi( input.Mid( 5, 2 ) );
		date.Format( L"%s %s %s", day, monthes.GetAt( month - 1 ), year );
	}
	return date;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CGedcomOut::list()
{
	int famc_id;
	CString famc_idS;
	CString name;


	fwprintf( fl, L"\n\nINDI rekordok a gyerek-házastárs family id-val:\n\n" );
	fwprintf( fl, L"%-5s %-8s %-30s %-8s %-8s\n", L"rowid", L"INDI_ID", L"name", L"FAMC_ID", L"FAMS_ID" );
	for( UINT i = 0; i < vINDI.size(); ++i )
	{
		name.Format( L"%s %s", vINDI.at(i).surn, vINDI.at(i).givn );
		famc_id = vINDI.at(i).famc_id;
		if( famc_id == nincs )
			famc_idS.Empty();
		else
			famc_idS.Format( L"@F%05d@", famc_id+1 );
		fwprintf( fl, L"%5d @I%05d@ %-30s %8s ", vINDI.at(i).rowid, i+1, name, famc_idS );

		for( UINT j = 0; j < vFAM.size(); ++j )
		{
			if( vFAM.at(j).husb_id == i || vFAM.at(j).wife_id == i )
			{
				fwprintf( fl, L"@F%05d@ ", j + 1 );
			}
		}

		fwprintf( fl, L"\n" );
	}

	UINT husbIndex;
	UINT wifeIndex;
	CString husband;
	CString wife;


	fwprintf( fl, L"\n\nFAM rekordok:\n\n" );
	fwprintf( fl, L"%-8s %-8s %-8s %-30s %-30s %-20s %-10s\n", L"FAM_ID", L"HUSB_ID", L"WIFE_ID", L"férj", L"feleség", L"PLAC", L"DATE");
	for( UINT i = 0; i < vFAM.size(); ++i )
	{
		wife.Empty();
		husbIndex = vFAM.at(i).husb_id;
		wifeIndex = vFAM.at(i).wife_id;

		if( 0 <= husbIndex && husbIndex < vINDI.size() )
			husband.Format( L"%s %s", vINDI.at( husbIndex ).surn, vINDI.at( husbIndex ).givn );
		else
		{
			husbIndex = -1;
			husband.Empty();
		}
		
		if( 0 <= wifeIndex && wifeIndex < vINDI.size() )
			wife.Format( L"%s %s", vINDI.at( wifeIndex ).surn, vINDI.at( wifeIndex ).givn );
		else 
		{
			wifeIndex = -1;
			wife.Empty();
		}

		fwprintf( fl, L"@F%05d@ @I%05d@ @I%05d@ ", i+1, husbIndex+1, wifeIndex+1 );
		fwprintf( fl, L"%-30s %-30s %-20s %-10s\n", husband, wife, vFAM.at(i).place, vFAM.at(i).date ); 
	}

	CString surn;
	CString givn;
	int fam_id;
	int fam_id_prev = -1;
	fwprintf( fl, L"\n\nCHIL rekordok:\n\n" );
	fwprintf( fl, L"%-8s %-8s %s\n", L"FAM_ID", L"CHIH_ID", L"név" );

	for( UINT i = 0; i < vCHIL.size(); ++i )
	{
		surn = vINDI.at( vCHIL.at(i).chil_id ).surn;
		givn = vINDI.at( vCHIL.at(i).chil_id ).givn;
		name.Format( L"%s %s", surn, givn );
		fam_id = vCHIL.at(i).fam_id;
		if( fam_id != fam_id_prev )
			fwprintf( fl, L"\n" );
		fam_id_prev = fam_id;

		fwprintf( fl, L"@F%05d@ @I%05d@ ", fam_id+1, vCHIL.at(i).chil_id+1 );
		fwprintf( fl, L"%s\n", name ); 
	}

	fclose( fl );
}
