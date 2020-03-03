// LifespanParam.cpp : implementation file
//

#include "stdafx.h"
#include "Fa.h"
#include "checkLifespan.h"
#include "afxdialogex.h"
#include "checkLifespanListCtrl.h"

// adatbázis mezõk
enum
{
	L_ROWID = 0,
	L_LINENUMBER,
	L_TABLENUMBER,
	L_GENERATION,
	L_SOURCE,
	L_UNITED,
	L_LAST_NAME,
	L_FIRST_NAME,
	L_BIRTH_DATE,
	L_DEATH_DATE,
};

// CLifespanParam dialog

IMPLEMENT_DYNAMIC(CLifespanParam, CDialogEx)
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLifespanParam::CLifespanParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CLifespanParam::IDD, pParent)
{
	m_recordset		= new CSqliteDBRecordSet;
	m_recordset1	= new CSqliteDBRecordSet;
	m_recordset2	= new CSqliteDBRecordSet;

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CLifespanParam::~CLifespanParam()
{
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_RADIO_MFC, m_RadioMfc);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CLifespanParam, CDialogEx)
	ON_BN_CLICKED(IDOK, &CLifespanParam::OnBnClickedOk)
END_MESSAGE_MAP()
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLifespanParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();



	CString info = L"\
Azokat az embereket listázzuk, akiknek az életkora nagyobb az elõírtnál vagy kisebb mint 0.\
\r\n\r\n\
Választani lehet, hogy a kimenet html fájl vagy MFC lista legyen.\
\r\n\r\n\
Ha MFC listát kérünk, akkor a jobb egérgombbal történõ kattintással megjelenõ legördülõ menübõl választható \
funkciókkal lehetõség van a kijelölt emberek eredeti html fájl-sorainak megjelenítésére és azok szerkesztésére.\
\r\n\
";

	GetDlgItem( IDC_EDIT )->SetWindowTextW( info );

	_lifespan = 100;
	str.Format( L"%d", _lifespan );
	GetDlgItem( IDC_LIFESPAN )->SetWindowTextW( str );

	m_RadioMfc.SetCheck( TRUE );

	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CLifespanParam::OnBnClickedOk()
{
	GetDlgItem( IDC_LIFESPAN )->GetWindowTextW( str );
	_lifespan = _wtoi( str );
	_mfc = m_RadioMfc.GetCheck();

	CString fields = L"\
rowid,\
linenumber,\
tablenumber,\
generation,\
source,\
united,\
last_name,\
first_name,\
birth_date,\
death_date\
";

	CString title	= L"Élettartam ellenõrzése";
	CString fileName = L"checkLifespan";
	CString fileSpec;

	CString rowid;
	CString lineNumber;
	CString tableNumber;
	CString	generation;
	CString united;
	CString source;
	CString name;
	CString birth_date;
	CString death_date;

	int birth;
	int death;
	int lifespan;
	int cnt = 0;
	int	diff;

	CProgressWnd wndProgress(NULL,L"Az emberek életkorának ellenõrzése ..." ); 
	wndProgress.GoModal();

	m_command.Format( L"SELECT %s FROM people WHERE length(birth_date)!=0 AND length(death_date)!=0 ORDER BY last_name, first_name", fields );
	if( !query( m_command ) ) return;

	wndProgress.SetRange(0, m_recordset->RecordsCount() );
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	if( _mfc )
	{
		if( (fileSpec = theApp.openTextFile( &theApp.fl, fileName, L"w+" ) ) == L"" ) return;
	}
	else
	{
		if(( fl=theApp.openLogFile( fileName,title ))==NULL)return;
	}


	for( UINT i = 0; i < m_recordset->RecordsCount(); ++i, m_recordset->MoveNext() )
	{

		birth_date = m_recordset->GetFieldString( L_BIRTH_DATE );
		birth = theApp.getYearFromDate( birth_date );
		death_date = m_recordset->GetFieldString( L_DEATH_DATE );
		death = theApp.getYearFromDate( death_date );
		if( birth == 0 || !checkDate( birth_date ) || death == 0 || !checkDate( death_date ) )
			continue;

		lifespan = death - birth;
		diff = lifespan - _lifespan;
		if( diff > 0 || lifespan < 0)
		{
			if( !cnt && !_mfc )
			{
				str.Format( L"Emberek, akiknek az élettartama nagyobb mint %d év:\n\n", _lifespan );
				fwprintf( fl, str );
				str.Format( L"%8s %8s %8s %1s %1s %1s %-30s %5s\n", L"rowid", L"line #", L"table#", L"G", L"S", L"U", L"név", L"év" );
				fwprintf( fl, str );
			}
			rowid		= m_recordset->GetFieldString( L_ROWID );
			lineNumber	= m_recordset->GetFieldString( L_LINENUMBER );
			tableNumber	= m_recordset->GetFieldString( L_TABLENUMBER );
			generation	= m_recordset->GetFieldString( L_GENERATION );
			source		= m_recordset->GetFieldString( L_SOURCE );
			united		= m_recordset->GetFieldString( L_UNITED );
			name.Format( L"%s %s", m_recordset->GetFieldString( L_LAST_NAME ), m_recordset->GetFieldString( L_FIRST_NAME )  );


			if( _mfc )
			{
				str.Format( L"%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\t%d\t\n", rowid, lineNumber, tableNumber, generation, source, united,name, birth_date, death_date, lifespan ); 
				fwprintf( theApp.fl, str );
			}
			else
			{
				str.Format( L"%8s %8s %8s %1s %1s %1s %-30s %12s %12s %5d\n", rowid, lineNumber, tableNumber, generation, source, united,name, birth_date, death_date, lifespan ); 
				fwprintf( fl, str );
			}
			++cnt;
		}
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();

	if( _mfc )
	{
		fclose( theApp.fl );

		CLifespanListCtrl dlg;
		dlg._fileSpec = fileSpec;
		dlg._caption.Format( L"%d évnél tovább élt emberek", _lifespan );
		dlg.DoModal();
	}
	else
	{
		fwprintf(fl, L"\nElapsed time: %s\n", theApp.get_time_elapsed() );
		fclose( fl );
		theApp.showLogFile();
	}
	CDialogEx::OnOK();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CLifespanParam::query( CString command )
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
BOOL CLifespanParam::query1( CString command )
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
BOOL CLifespanParam::query2( CString command )
{
	if( m_recordset2->Query(command,theApp.mainDB))
	{
		str.Format(L"%s\n%s",command,m_recordset2->GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}

