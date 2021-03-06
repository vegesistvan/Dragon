// HtmlBrackets.cpp : implementation file
//

#include "stdafx.h"
#include "Dragon.h"
#include "html_Brackets.h"
#include "afxdialogex.h"
#include "ProgressWnd.h"
#include "utilities.h"

enum
{
	MIND = 0,
	FAMILY,
	TABLE,
	DESCENDANT,
	ROUND_ALL,
	ROUND_FAMILY,
	ROUND_TABLE,
	ROUND_DESCENDANT,
};

enum
{
	ALL = 0,
	TORZS,
	AG,
	NONE,
	ADOPT,
	TABLES_ALL,
	DESCENDANTS_ALL,
	CSALAD,
	VONAL,
	OS,
	OTHER,

	ROUND_DESC_ALL,
	ROUND_RELATIVES,
	ROUND_OTHER,
};
//#define FILTER_NONE 100
// CHtmlBrackets dialog

IMPLEMENT_DYNAMIC(CHtmlBrackets, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlBrackets::CHtmlBrackets(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHtmlBrackets::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CHtmlBrackets::~CHtmlBrackets()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST, m_ListCtrl);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CHtmlBrackets, CDialogEx)
	ON_WM_SIZE()
	ON_WM_SIZING()
	ON_MESSAGE(WM_SET_COLUMN_COLOR, OnSetColumnColor)
	ON_MESSAGE(WM_CLICKED_COLUMN, OnColumnSorted)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST, &CHtmlBrackets::OnDblclkList)

	ON_COMMAND(ID_FILTER_MIND_ALL, &CHtmlBrackets::OnFilterMindAll)
	ON_COMMAND(ID_FILTER_MIND_AG, &CHtmlBrackets::OnFilterMindAg)
	ON_COMMAND(ID_FILTER_MIND_ADOPT, &CHtmlBrackets::OnFilterMindAdopt)
	ON_COMMAND(ID_FILTER_MIND_CSALAD, &CHtmlBrackets::OnFilterMindCsalad)
	ON_COMMAND(ID_FILTER_MIND_OS, &CHtmlBrackets::OnFilterMindOs)
	ON_COMMAND(ID_FILTER_MIND_OTHER, &CHtmlBrackets::OnFilterMindOther)
	ON_COMMAND(ID_FILTER_MIND_TORZS, &CHtmlBrackets::OnFilterMindTorzs)
	ON_COMMAND(ID_FILTER_MIND_VONAL, &CHtmlBrackets::OnFilterMindVonal)

	ON_COMMAND(ID_FILTER_F_ALL, &CHtmlBrackets::OnFilterFAll)
	ON_COMMAND(ID_FILTER_F_TORZS, &CHtmlBrackets::OnFilterFTorzs)
	ON_COMMAND(ID_FILTER_F_AG, &CHtmlBrackets::OnFilterFAg)
	ON_COMMAND(ID_FILTER_F_OTHER, &CHtmlBrackets::OnFilterFOther)

	
	ON_COMMAND(ID_FILTER_T_ALL, &CHtmlBrackets::OnFilterTAll)
	ON_COMMAND(ID_FILTER_T_AG, &CHtmlBrackets::OnFilterTAg)
	ON_COMMAND(ID_FILTER_T_VONAL, &CHtmlBrackets::OnFilterTVonal)
	ON_COMMAND(ID_FILTER_T_OTHER, &CHtmlBrackets::OnFilterTOther)

	ON_COMMAND(ID_FILTER_D_ALL, &CHtmlBrackets::OnFilterDAll)
	ON_COMMAND(ID_FILTER_D_AG, &CHtmlBrackets::OnFilterDAg)
	ON_COMMAND(ID_FILTER_D_CSALAD, &CHtmlBrackets::OnFilterDCsalad)
	ON_COMMAND(ID_FILTER_D_TORZS, &CHtmlBrackets::OnFilterDTorzs)
	ON_COMMAND(ID_FILTER_D_VONAL, &CHtmlBrackets::OnFilterDVonal)
	ON_COMMAND(ID_FILTER_D_ADOPT, &CHtmlBrackets::OnFilterDAdopt)
	ON_COMMAND(ID_FILTER_D_OTHER, &CHtmlBrackets::OnFilterDOther)
	ON_COMMAND(ID_FILTER_D_OS, &CHtmlBrackets::OnFilterDOs )
	ON_COMMAND(ID_FILTER_R_FAMILY, &CHtmlBrackets::OnFilterRFamily)
	ON_COMMAND(ID_R_DESC_ALL, &CHtmlBrackets::OnRDescAll)
	ON_COMMAND(ID_R_DESC_OTHER, &CHtmlBrackets::OnRDescOther)
	ON_COMMAND(ID_R_DESC_RELATIVES, &CHtmlBrackets::OnRDescRelatives)
	ON_COMMAND(ID_FILTER_R_TABLE, &CHtmlBrackets::OnFilterRTable)
	ON_COMMAND(ID_FILTER_R_ALL, &CHtmlBrackets::OnFilterRAll)

	ON_MESSAGE(WM_LISTCTRL_MENU, OnListCtrlMenu)
	ON_COMMAND(ID_HTML_EDIT, &CHtmlBrackets::OnHtmlEdit)
	ON_COMMAND(ID_HTML_NOTEPAD, &CHtmlBrackets::OnHtmlNotepad)


END_MESSAGE_MAP()
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CHtmlBrackets::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	EASYSIZE_ADD( IDC_LIST,	ES_BORDER,	ES_BORDER,		ES_BORDER,		ES_BORDER,	0 );
	EASYSIZE_INIT()

	if( _square )
	{
		_caption.Format( L"Sz�gletes z�r�jelek a %s f�jlban.", theApp.m_htmlFileSpec );
		_first	= '[';
		_last	= ']';

		SetMenu(NULL);
        ::DestroyMenu(GetMenu()->GetSafeHmenu());
        menu.LoadMenuW(IDR_SQUARE);
        SetMenu(&menu);

		_filter1 = MIND;
		_filter2 = ALL;
	}
	if( _round )
	{
		_caption.Format( L"Kerek z�r�jelek a %s f�jlban.", theApp.m_htmlFileSpec );
		_first	= '(';
		_last	= ')';
		SetMenu(NULL);
        ::DestroyMenu(GetMenu()->GetSafeHmenu());
        menu.LoadMenuW(IDR_ROUND);
        SetMenu(&menu);

		_filter1 = ROUND_ALL;
		_filter2 = ALL;
	}
	if( _braces	)
	{
		_caption.Format( L"Kapocsos z�r�jelek a %s f�jlban.", theApp.m_htmlFileSpec );
		_first	= '{';
		_last	= '}';

		_filter1 = MIND;
		_filter2 = ALL;
	}
	

	m_ListCtrl.SortByHeaderClick(TRUE);
	m_ListCtrl.SetExtendedStyle(m_ListCtrl.GetExtendedStyle()| LVS_EX_GRIDLINES );
	m_ListCtrl.SortByHeaderClick(TRUE);

	m_ListCtrl.InsertColumn( 0,	L"line#",	LVCFMT_RIGHT,	 40,-1,COL_HIDDEN );
	m_ListCtrl.InsertColumn( 1,	L"line#",	LVCFMT_RIGHT,	 50,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 2,	L"%",		LVCFMT_RIGHT,	 60,-1,COL_NUM);
	m_ListCtrl.InsertColumn( 3,	L"",		LVCFMT_LEFT,	1000,-1,COL_TEXT);

	_orderix = 0;
	_filterText = _caption;
	fillTable();

	return TRUE;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void CHtmlBrackets::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	CDialogEx::OnSize(nType, cx, cy);
	EASYSIZE_RESIZE()
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);
	CDialogEx::OnSizing(fwSide, pRect);
	EASYSIZE_MINSIZE(430,314,fwSide,pRect); 
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::fillTable()
{
	CString cLine;
	int fileLength;
	int position;
	int	pos1;
	int	pos2;
	int	lineNumber = 0;
	CString target;

	theApp.m_inputCode = GetInputCode( theApp.m_htmlFileSpec );
	CStdioFile file( theApp.m_htmlFileSpec, CFile::modeRead); 
	fileLength = (int)file.GetLength();

	SetWindowText( _filterText );
	
	CProgressWnd wndP(NULL, L"Z�r�jelek beolvas�sa....");
	wndP.GoModal();
	wndP.SetRange(0, fileLength );
	wndP.SetPos(0);
	wndP.SetStep(1);

	m_ListCtrl.DeleteAllItems();
	
	int nItem = 0;

	int start;
	while(file.ReadString(cLine)) 
	{
		if( cLine.IsEmpty() ) continue;
		cLine.Trim();
		if( theApp.m_inputCode == UTF8 || theApp.m_inputCode == UTF8BOM ) cLine = Utf8ToAnsi( cLine );
		cLine.Replace( '\'', '|' );	 // a nevekben el�fordulhat az ' jel, amit az SQLite nem szeret
		++lineNumber;

		_p.Empty();
		if(  cLine.Left( 3 ) == L"%%%" ) _p = L"%%%";
		else if( cLine.Left( 2 ) == L"%%" ) _p = L"%%";
		
		start = 0;
		while( ( pos1 = cLine.Find( _first, start ) )!= -1 && ( pos2 = cLine.Find( _last, start+1 ) ) != -1 )
		{
			target = cLine.Mid( pos1, pos2-pos1+1 );
			switch( _filter1 )
			{
				case MIND:
					break;
				case FAMILY:
					if( _p.Compare( L"%%%" ) ) goto cont;
					break;
				case TABLE:
					if( _p.Compare( L"%%" ) ) goto cont;
					break;
				case DESCENDANT:
					if( !_p.Compare( L"%%%" ) || !_p.Compare( L"%%" ) ) goto cont;
					break;
				case ROUND_ALL:
					break;
				case ROUND_FAMILY:
					if( _p.Compare( L"%%%" ) ) goto cont;
					break;
				case ROUND_TABLE:
					if( _p.Compare( L"%%" ) ) goto cont;
					break;
				case ROUND_DESCENDANT:
					if( !_p.Compare( L"%%%" ) || !_p.Compare( L"%%" )  ) goto cont;
					break;
			}
			switch( _filter2 )
			{
				case ALL:
					break;
				case TORZS:
					if( target.Find( L"t�rzs" ) == -1 ) goto cont;
					break;
				case AG:
					if( target.Find( L" �g" ) == -1  && target.Find( L"al�g" )  ) goto cont;
					break;
				case VONAL:
					if( target.Find( L"vonal" ) == -1 ) goto cont;
					break;
				case ADOPT:
					if( target.Find( L"adopt" ) == -1 ) goto cont;
					break;
				case CSALAD:
					if( target.Find( L"csal�d]" ) == -1 ) goto cont;
					break;
				case OS:
					if( target.Find( L"�se]" ) == -1 ) goto cont;
					break;
				case OTHER:
					if( target.Find( L"t�rzs" )		!= -1 ||\
						target.Find( L" �g" )		!= -1 ||\
						target.Find( L"al�g" )		!= -1 ||\
						target.Find( L"vonal" )		!= -1 ||\
						target.Find( L"adopt" )		!= -1 ||\
						target.Find( L"csal�d]" )	!= -1 ||\
						target.Find( L"�se]" )		!= -1 )  goto cont;
					break;

				case ROUND_RELATIVES:
					if( target.Find( '-' ) == -1 &&\
						target.Find( L"f." ) == -1 &&\
						target.Find( L" fia" ) == -1 &&\
						target.Find( L"l�nya" ) == -1 &&\
						target.Find( L"le�nya" ) == -1  ) goto cont;
					break;
				case ROUND_OTHER:
					if( target.Find( '-' ) != -1  ||\
						target.Find( L"f." ) != -1 ||\
						target.Find( L" fia" ) != -1 ||\
						target.Find( L"l�nya" ) != -1 ||\
						target.Find( L"le�nya" ) != -1 ) goto cont;
					break;
			}

			str.Format( L"%d", lineNumber );
			m_ListCtrl.InsertItem( nItem, str );
			m_ListCtrl.SetItemText( nItem, 1, str );
			m_ListCtrl.SetItemText( nItem, 2, _p );
			m_ListCtrl.SetItemText( nItem, 3, target );

			++nItem;
cont:		start = pos2;
		}

		position = (int)file.GetPosition();
		wndP.SetPos( position );
		wndP.StepIt();
		wndP.PeekAndPump();
		if (wndP.Cancelled()) break;
	}

	wndP.DestroyWindow();
	file.Close();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CHtmlBrackets::OnSetColumnColor(WPARAM wParam, LPARAM lParam)//wparam: oszlopszam, lparam: a COLUMNCOLOR struktura cime
{
	CELLCOLOR* cColor= (CELLCOLOR*) lParam;//a COLUMNCOLOR struktura deklaralva van a ListCtrlEx.h -ban.

	if( _orderix==(int)wParam)
	{
		cColor->bg	= RGB(250,250,250);
		cColor->tx	= RGB(250,0,0);
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CHtmlBrackets::OnColumnSorted(WPARAM wParam, LPARAM lParam) //wparam: column number
{
	_orderix = wParam;
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnDblclkList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	int nItem = m_ListCtrl.GetNextItem(-1,LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 1 ) );
	theApp.listHtmlLine( lineNumber );
	*pResult = 0;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindAll()
{
	_filter1 = MIND;
	_filter2 = ALL;
	_filterText = _caption;
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindAg()
{
	_filter1 = MIND;
	_filter2 = AG;
	_filterText = L"Sz�gletes z�r�jelek, amelyek tartalmazz�k az ' �g' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindAdopt()
{
	_filter1 = MIND;
	_filter2 = ADOPT;
	_filterText = L"Sz�gletes z�r�jelek, amelyek tartalmazz�k az 'adopt' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindCsalad()
{
	_filter1 = MIND;
	_filter2 = CSALAD;
	_filterText = L"Sz�gletes z�r�jelek,  amelyek tartalmazz�k a 'csal�d]' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindOs()
{
	_filter1 = MIND;
	_filter2 = OS;
	_filterText = L"Sz�gletes z�r�jelek,  amelyek tartalmazz�k az '�se]' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindTorzs()
{
	_filter1 = MIND;
	_filter2 = TORZS;
	_filterText = L"Sz�gletes z�r�jelek,  amelyek tartalmazz�k a 't�rzs' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindVonal()
{
	_filter1 = MIND;
	_filter2 = VONAL;
	_filterText = L"Sz�gletes z�r�jelek,  amelyek tartalmazz�k a 'vonal' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterMindOther()
{
	_filter1 = MIND;
	_filter2 = OTHER;
	_filterText = L"Sz�gletes z�r�jelek,  amelyek nem tartlmazz�k a viizsg�lt karakter sorozatot.";
	fillTable();
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterFAll()
{
	_filter1 = FAMILY;
	_filter2 = ALL;
	_filterText = L"Sz�gletes z�r�jelek a %%% t�bla fejl�ceiben.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterFTorzs()
{
	_filter1 = FAMILY;
	_filter2 = TORZS;
	_filterText = L"Sz�gletes z�r�jelek a %%% t�bla fejl�ceiben, amelyek tartalmazz�k a 't�rzs' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterFAg()
{
	_filter1 = FAMILY;
	_filter2 = AG;
	_filterText = L"Sz�gletes z�r�jelek a %%% t�bla fejl�ceiben, amelyek tartalmazz�k a ' �g' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterFOther()
{
	_filter1 = FAMILY;
	_filter2 = OTHER;
	_filterText = L"Sz�gletes z�r�jelek a %%% t�bla fejl�ceiben, amelyek nem tartalmazz�k a vizsg�lt karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterTAll()
{
	_filter1 = TABLE;
	_filter2 = ALL;
	_filterText = L"Sz�gletes z�r�jelek a %%% t�bla fejl�ceiben, amelyek tartalmazz�k a 't�rzs' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterTVonal()
{
	_filter1 = TABLE;
	_filter2 = VONAL;
		_filterText = L"Sz�gletes z�r�jelek a %%% t�bla fejl�ceiben, amelyek tartalmazz�k a 'vonal' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterTAg()
{
	_filter1 = TABLE;
	_filter2 = AG;
		_filterText = L"Sz�gletes z�r�jelek a %%% t�bla fejl�ceiben, amelyek tartalmazz�k az ' �g' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterTOther()
{
	_filter1 = TABLE;
	_filter2 = OTHER;
	_filterText = L"Sz�gletes z�r�jelek a %% t�bla fejl�ceiben, amelyek nem tartalmazz�k a vizsg�lt karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDAll()
{
	_filter1 = DESCENDANT;
	_filter2 = ALL;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban";
	fillTable();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDAg()
{
	_filter1 = DESCENDANT;
	_filter2 = AG;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban, amelyek tartlamzz�k az ' �g' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDCsalad()
{
	_filter1 = DESCENDANT;
	_filter2 = CSALAD;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban, amelyek tartlamzz�k a 'csal�d' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDTorzs()
{
	_filter1 = DESCENDANT;
	_filter2 = TORZS;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban, amelyek tartlamzz�k az 't�rzs' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDVonal()
{
	_filter1 = DESCENDANT;
	_filter2 = VONAL;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban, amelyek tartlamzz�k a 'vonal' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDAdopt()
{
	_filter1 = DESCENDANT;
	_filter2 = ADOPT;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban, amelyek tartlamzz�k az 'adopt' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDOs()
{
	_filter1 = DESCENDANT;
	_filter2 = OS;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban, amelyek tartlamzz�k az '�s]' karakter sorozatot.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterDOther()
{
	_filter1 = DESCENDANT;
	_filter2 = OTHER;
	_filterText = L"Sz�gletes z�r�jelek a lesz�rmazotti rekordokban, amelyek nem tartlamznak kulcssz�t.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterRAll()
{
	_filter1 = ROUND_ALL;
	_filter2 = ALL;
	_filterText = L"Kerek z�r�jelek a html f�jlban";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterRFamily()
{
	_filter1 = ROUND_FAMILY;
	_filter2 = ALL;
	_filterText = L"Kerek z�r�jelek a %%% t�bla fejl�ceiben.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnFilterRTable()
{
	_filter1 = ROUND_TABLE;
	_filter2 = ALL;
	_filterText = L"Kerek z�r�jelek a %% t�bla fejl�ceiben.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnRDescAll()
{
	_filter1 = ROUND_DESCENDANT;
	_filter2 = ALL;
	_filterText = L"Kerek z�r�jelek a lesz�rmazotti rekordokban.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CHtmlBrackets::OnRDescParents()
//{
//	_filter1 = ROUND_DESCENDANT;
//	_filter2 = ROUND_PARENTS;
//	_filterText = L"Kerek z�r�jelek a lesz�rmazotti rekordokban, amelyek '-' k�t�jelet tartalmaznak.";
//	fillTable();
//}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnRDescRelatives()
{
	_filter1 = ROUND_DESCENDANT;
	_filter2 = ROUND_RELATIVES;
	_filterText = L"Kerek z�r�jelek a lesz�rmazotti rekordokban, amelyek rokonokat tartalmaznak. ( '-' vagy 'f.' vagy ' fia', vagy 'l�nya' vagy 'le�nya' kulcsszavak )";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnRDescOther()
{
	_filter1 = ROUND_DESCENDANT;
	_filter2 = ROUND_OTHER;
	_filterText = L"Kerek z�r�jelek a lesz�rmazotti rekordokban, amelyek nem tartalmaznak kulcssz�t.";
	fillTable();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//void CHtmlBrackets::OnRDescFia()
//{
//	_filter1 = ROUND_DESCENDANT;
//	_filter2 = ROUND_FIA_LANYA;
//	_filterText = L"Kerek z�r�jelek a lesz�rmazotti rekordokban, amelyek 'fia' vagy 'l�nya' kulcssz�t tartalmaznak.";
//	fillTable();
//}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
LRESULT CHtmlBrackets::OnListCtrlMenu(WPARAM wParam, LPARAM lParam)
{
	CPoint* point=(CPoint*) lParam;
    CMenu	Menu;
	CMenu*	pPopup;
	int		MENU_IDR;

	MENU_IDR = IDR_DROPDOWN_HTML;
	if(Menu.LoadMenu( MENU_IDR ))
    {
		pPopup = Menu.GetSubMenu(0);

		pPopup->EnableMenuItem(ID_DB_EDIT,MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_NOTEPAD_PARENTS,MF_BYCOMMAND | MF_GRAYED);
		pPopup->EnableMenuItem(ID_HTML_FATHERANDSIBLINGS,MF_BYCOMMAND | MF_GRAYED);
		pPopup->TrackPopupMenu(TPM_LEFTALIGN|TPM_RIGHTBUTTON,point->x,point->y,this);
    }
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnHtmlEdit()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	int lineNumber = _wtoi( m_ListCtrl.GetItemText( nItem, 	0 ) );
	theApp.listHtmlLine( lineNumber );
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CHtmlBrackets::OnHtmlNotepad()
{
	int nItem = m_ListCtrl.GetNextItem(-1, LVNI_SELECTED);
	CString lineNumber = m_ListCtrl.GetItemText( nItem, 0 );
	if( !lineNumber.IsEmpty() ) 
		theApp.editNotepad( lineNumber );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
