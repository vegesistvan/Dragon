#include "stdafx.h"
#include "Dragon.h"
#include "DragonDlg.h"
#include "afxdialogex.h"
//#include "Relatives.h"
//#include "about.h"
#include "SelectOne.h"
#include "SelectedFiles.h"
#include "GetString.h"
#include "utilities.h"
#include "GAtoDB.h"
#include "GA_input.h"
#include "SelectTableName.h"
#include "SelectFamily.h"
#include "SelectFamilyName.h"
#include "SelectTableHtml.h"
#include "ProgressWnd.h"
#include "Table_GaTables.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDescendantFile()
{
	CDescendant dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= L"";
	dlg.DoModal();

/*
	if( !m_pDescendantsF )
	{
		m_pDescendantsF = new CDescendant(this);
		m_pDescendantsF->m_familyNumber	= 0;
		m_pDescendantsF->m_tableNumber	= 0;
		m_pDescendantsF->m_lineNumber		= 0;
		m_pDescendantsF->m_tableName		= L"";
	
		m_pDescendantsF->Create(CDescendant::IDD,GetDesktopWindow());
		m_pDescendantsF->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDescendantsF->ShowWindow(SW_RESTORE);
		m_pDescendantsF->SetForegroundWindow();
	}
*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDescendantTable()
{
	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;

	tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
	if( dlgT.DoModal() == IDCANCEL ) 
	{
		return;
	}
	tableNumber = dlgT.m_tableNumberSelected;
	tableName	= dlgT.m_tableNameSelected;
	theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );

	CDescendant dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= tableNumber;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= tableName;
	dlg.DoModal();


/*
	if( !m_pDescendantsT )
	{
		tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}
		tableNumber = dlg.m_tableNumberSelected;
		tableName	= dlg.m_tableNameSelected;
		theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );

		m_pDescendantsT = new CDescendant(this);
		m_pDescendantsT->m_familyNumber	= 0;
		m_pDescendantsT->m_tableNumber	= tableNumber;
		m_pDescendantsT->m_lineNumber	= 0;
		m_pDescendantsT->m_tableName		= tableName;

		m_pDescendantsT->Create(CDescendant::IDD,GetDesktopWindow());
		m_pDescendantsT->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDescendantsT->SetForegroundWindow();
		m_pDescendantsT->ShowWindow(SW_RESTORE);
	}
*/
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnDescendantLine()
{
	CGetString dlgL;
	int lineNumber;
	
	lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
	dlgL.m_string.Format( L"%d", lineNumber ); 

	dlgL.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
	if( dlgL.DoModal() == IDCANCEL ) return;
	
	lineNumber = _wtoi( dlgL.m_string );
	theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );

	CDescendant dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber		= lineNumber;
	dlg.m_tableName		= L"";
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMarriagesFile()
{
	CHtmlMarriages dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= L"";
	dlg.DoModal();

}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMarriagesTable()
{
	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;


	tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
	if( dlgT.DoModal() == IDCANCEL ) return;

	tableNumber = dlgT.m_tableNumberSelected;
	tableName	= dlgT.m_tableNameSelected;
	theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );


	CHtmlMarriages dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= tableNumber;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= tableName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnMarriagesLine()
{
	CGetString dlgL;
	int lineNumber;

	lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
	dlgL.m_string.Format( L"%d", lineNumber ); 

	dlgL.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
	if( dlgL.DoModal() == IDCANCEL ) return;

	lineNumber = _wtoi( dlgL.m_string );
	theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );


	CHtmlMarriages dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= lineNumber;
	dlg.DoModal();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CDragonDlg::OnSpousesFile()
{
	CHtmlSpouses dlg;
	
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber		= 0;
	dlg.m_tableName	= L"";
	dlg.DoModal();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSpousesTable()
{
	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;

	tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
	if( dlgT.DoModal() == IDCANCEL ) return;
	
	tableNumber = dlgT.m_tableNumberSelected;
	tableName	= dlgT.m_tableNameSelected;
	theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );


	CHtmlSpouses dlg;
	
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= tableNumber;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= tableName;
	dlg.DoModal();


}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSpousesLine()
{
	CGetString dlgL;
	int lineNumber;

	lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
	dlgL.m_string.Format( L"%d", lineNumber ); 

	dlgL.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
	if( dlgL.DoModal() == IDCANCEL ) return;

	lineNumber = _wtoi( dlgL.m_string );
	theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );


	CHtmlSpouses dlg;
	
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= lineNumber;
	dlg.m_tableName		= L"";
	dlg.DoModal();
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSsFile()
{
	CSpouseSpouses dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber		= 0;
	dlg.DoModal();

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSsTable()
{
	CSelectTableName dlgT;
	int tableNumber;
	CString tableName;

	tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
	if( dlgT.DoModal() == IDCANCEL ) return;
	tableNumber = dlgT.m_tableNumberSelected;
	tableName	= dlgT.m_tableNameSelected;
	theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );


	CSpouseSpouses dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= tableNumber;
	dlg.m_lineNumber	= 0;
	dlg.m_tableName		= tableName;
	dlg.DoModal();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonDlg::OnSsLine()
{
	CGetString dlgL;
	int lineNumber;


	lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
	dlgL.m_string.Format( L"%d", lineNumber ); 
	dlgL.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
	if( dlgL.DoModal() == IDCANCEL ) return;

	lineNumber = _wtoi( dlgL.m_string );
	theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );

	CSpouseSpouses dlg;
	dlg.m_familyNumber	= 0;
	dlg.m_tableNumber	= 0;
	dlg.m_lineNumber	= lineNumber;
	dlg.DoModal();
}
