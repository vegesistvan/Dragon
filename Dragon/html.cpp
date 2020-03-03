#include "stdafx.h"
#include "Fa.h"
#include "FaDlg.h"
#include "afxdialogex.h"
//#include "Relatives.h"
//#include "about.h"
#include "SelectOne.h"
#include "SelectedFiles.h"
#include "GetString.h"
#include "utilities.h"
#include "GA_input_parameters.h"
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
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CFaDlg::OnGahtml()
{
	ShellExecute(NULL, L"open", theApp.m_htmlFileSpec,NULL, NULL, SW_SHOWNORMAL);
}
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
void CFaDlg::OnGaTables()
{
	CTableGaTables dlg;
	dlg.DoModal();
}
*/
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnDescendantFile()
{
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
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnDescendantTable()
{
	CSelectTableName dlg;
	int tableNumber;
	CString tableName;

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
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnDescendantLine()
{
	CGetString dlg;
	int lineNumber;

	if( !m_pDescendantsL )
	{
		lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
		dlg.m_string.Format( L"%d", lineNumber ); 

		dlg.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}

		lineNumber = _wtoi( dlg.m_string );
		theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );

		m_pDescendantsL = new CDescendant(this);
		m_pDescendantsL->m_familyNumber	= 0;
		m_pDescendantsL->m_tableNumber	= 0;
		m_pDescendantsL->m_lineNumber		= lineNumber;
		m_pDescendantsL->m_tableName		= L"";
	
		m_pDescendantsL->Create(CDescendant::IDD,GetDesktopWindow());
		m_pDescendantsL->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pDescendantsL->SetForegroundWindow();		// ha az ablak másik ablak alatt van
		m_pDescendantsL->ShowWindow( SW_RESTORE);		// ha az ablakot "bezártok" vagy levittük a toolbarra
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnMarriagesFile()
{
	if( !m_pMarriagesF )
	{
		m_pMarriagesF = new CMarriages(this);
		m_pMarriagesF->m_familyNumber	= 0;
		m_pMarriagesF->m_tableNumber	= 0;
		m_pMarriagesF->m_lineNumber	= 0;
		m_pMarriagesF->m_tableName		= L"";

//		m_reloadMs = 1;

		m_pMarriagesF->Create(CMarriages::IDD,GetDesktopWindow());
		m_pMarriagesF->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pMarriagesF->SetForegroundWindow();
		m_pMarriagesF->ShowWindow(SW_RESTORE);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnMarriagesTable()
{
	CSelectTableName dlg;
	int tableNumber;
	CString tableName;

	if( !m_pMarriagesT )
	{
		tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}
		tableNumber = dlg.m_tableNumberSelected;
		tableName	= dlg.m_tableNameSelected;
		theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );

		m_pMarriagesT = new CMarriages(this);
		m_pMarriagesT->m_familyNumber	= 0;
		m_pMarriagesT->m_tableNumber	= tableNumber;
		m_pMarriagesT->m_lineNumber	= 0;
		m_pMarriagesT->m_tableName		= tableName;
//		m_reloadMs = 2;


		m_pMarriagesT->Create(CMarriages::IDD,GetDesktopWindow());
		m_pMarriagesT->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pMarriagesT->SetForegroundWindow();
		m_pMarriagesT->ShowWindow(SW_RESTORE);
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnMarriagesLine()
{
	CGetString dlg;
	int lineNumber;

	if( !m_pMarriagesL )
	{
		lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
		dlg.m_string.Format( L"%d", lineNumber ); 

		dlg.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}

		lineNumber = _wtoi( dlg.m_string );
		theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );

		m_pMarriagesL = new CMarriages(this);
		m_pMarriagesL->m_familyNumber	= 0;
		m_pMarriagesL->m_tableNumber	= 0;
		m_pMarriagesL->m_lineNumber		= lineNumber;
//		m_reloadMs = 3;

		m_pMarriagesL->Create(CMarriages::IDD,GetDesktopWindow());
		m_pMarriagesL->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pMarriagesL->SetForegroundWindow();		// ha az ablak másik ablak alatt van
		m_pMarriagesL->ShowWindow( SW_RESTORE);		// ha az ablakot "bezártok" vagy levittük a toolbarra
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnSpousesFile()
{
	if( !m_pSpousesF )
	{
		m_pSpousesF = new CHtmlSpouses(this);
		m_pSpousesF->m_familyNumber	= 0;
		m_pSpousesF->m_tableNumber	= 0;
		m_pSpousesF->m_lineNumber		= 0;
		m_pSpousesF->m_tableName	= L"";
	
		m_pSpousesF->Create(CHtmlSpouses::IDD,GetDesktopWindow());
		m_pSpousesF->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSpousesF->ShowWindow(SW_RESTORE);
		m_pSpousesF->SetForegroundWindow();
	}
	
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnSpousesTable()
{
	CSelectTableName dlg;
	int tableNumber;
	CString tableName;

	if( !m_pSpousesT )
	{
		tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}
		tableNumber = dlg.m_tableNumberSelected;
		tableName	= dlg.m_tableNameSelected;
		theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );

		m_pSpousesT = new CHtmlSpouses(this);
		m_pSpousesT->m_familyNumber	= 0;
		m_pSpousesT->m_tableNumber	= tableNumber;
		m_pSpousesT->m_lineNumber	= 0;
		m_pSpousesT->m_tableName	= tableName;
	
		m_pSpousesT->Create(CSpouseSpouses::IDD,GetDesktopWindow());
		m_pSpousesT->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSpousesT->SetForegroundWindow();
		m_pSpousesT->ShowWindow(SW_RESTORE);
	}
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnSpousesLine()
{
	CGetString dlg;
	int lineNumber;

	if( !m_pSpousesL )
	{
		lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
		dlg.m_string.Format( L"%d", lineNumber ); 

		dlg.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}

		lineNumber = _wtoi( dlg.m_string );
		theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );

		m_pSpousesL = new CHtmlSpouses(this);
		m_pSpousesL->m_familyNumber	= 0;
		m_pSpousesL->m_tableNumber	= 0;
		m_pSpousesL->m_lineNumber		= lineNumber;
		m_pSpousesL->m_tableName	= L"";
	
		m_pSpousesL->Create(CSpouseSpouses::IDD,GetDesktopWindow());
		m_pSpousesL->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSpousesL->SetForegroundWindow();		// ha az ablak másik ablak alatt van
		m_pSpousesL->ShowWindow( SW_RESTORE);		// ha az ablakot "bezártok" vagy levittük a toolbarra
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnSsFile()
{
	if( !m_pSsF )
	{
		m_pSsF = new CSpouseSpouses(this);
		m_pSsF->m_familyNumber	= 0;
		m_pSsF->m_tableNumber	= 0;
		m_pSsF->m_lineNumber		= 0;
	
		m_pSsF->Create(CSpouseSpouses::IDD,GetDesktopWindow());
		m_pSsF->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSsF->ShowWindow(SW_RESTORE);
		m_pSsF->SetForegroundWindow();
	}
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnSsTable()
{
	CSelectTableName dlg;
	int tableNumber;
	CString tableName;

	if( !m_pSsT )
	{
		tableNumber = theApp.GetProfileInt(	L"Settings", L"tableNumber", 0 );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}
		tableNumber = dlg.m_tableNumberSelected;
		tableName	= dlg.m_tableNameSelected;
		theApp.WriteProfileInt( L"Settings", L"tableNumber", tableNumber  );

		m_pSsT = new CSpouseSpouses(this);
		m_pSsT->m_familyNumber	= 0;
		m_pSsT->m_tableNumber	= tableNumber;
		m_pSsT->m_lineNumber	= 0;
		m_pSsT->m_tableName		= tableName;

		m_pSsT->Create(CSpouseSpouses::IDD,GetDesktopWindow());
		m_pSsT->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSsT->SetForegroundWindow();
		m_pSsT->ShowWindow(SW_RESTORE);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CFaDlg::OnSsLine()
{
	CGetString dlg;
	int lineNumber;

	if( !m_pSsL )
	{
		lineNumber = theApp.GetProfileInt(	L"Settings", L"lineNumber", 0 );
		dlg.m_string.Format( L"%d", lineNumber ); 

		dlg.m_caption.Format( L"Add meg a %s fájlban lévõ sor sorszámát!", theApp.m_htmlFileName );
		if( dlg.DoModal() == IDCANCEL ) 
		{
			return;
		}

		lineNumber = _wtoi( dlg.m_string );
		theApp.WriteProfileInt( L"Settings", L"lineNumber", lineNumber  );

		m_pSsL = new CSpouseSpouses(this);
		m_pSsL->m_familyNumber	= 0;
		m_pSsL->m_tableNumber	= 0;
		m_pSsL->m_lineNumber		= lineNumber;
	
		m_pSsL->Create(CSpouseSpouses::IDD,GetDesktopWindow());
		m_pSsL->ShowWindow(SW_SHOW);
	}
	else
	{
		m_pSsL->SetForegroundWindow();		// ha az ablak másik ablak alatt van
		m_pSsL->ShowWindow( SW_RESTORE);		// ha az ablakot "bezártok" vagy levittük a toolbarra
	}
}
