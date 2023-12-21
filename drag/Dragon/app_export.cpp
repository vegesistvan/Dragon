#include "stdafx.h"
#include "Dragon.h"
#include "ProgressWnd.h"

#include "listctrlex.h"


class CColumnsList : public CDialogEx
{
	DECLARE_DYNAMIC(CColumnsList)

public:
	CColumnsList(CWnd* pParent = NULL);   // standard constructor
	virtual ~CColumnsList();

	enum { IDD = IDD_COLUMNSLIST };

	int	m_selected;
	int	m_numberOfColumns;

	BOOL m_underline;
	
	CListCtrl* p_ListCtrl;
	std::vector<BOOL>* selected;
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	CListCtrlEx m_ListCtrl;
	CString	str;

	DECLARE_MESSAGE_MAP()
	afx_msg void OnBnClickedOk();
	afx_msg void OnClickedCheckUnderline();
	virtual BOOL OnInitDialog();
};

IMPLEMENT_DYNAMIC(CColumnsList, CDialogEx)
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CColumnsList::CColumnsList(CWnd* pParent /*=NULL*/)
	: CDialogEx(CColumnsList::IDD, pParent)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CColumnsList::~CColumnsList()
{
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CColumnsList::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LISTCOLUMNS, m_ListCtrl);
	DDX_Check(pDX, IDC_CHECK_UNDERLINE, m_underline);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BEGIN_MESSAGE_MAP(CColumnsList, CDialogEx)
	ON_BN_CLICKED(IDOK, &CColumnsList::OnBnClickedOk)
	ON_BN_CLICKED(IDC_CHECK_UNDERLINE, &CColumnsList::OnClickedCheckUnderline)
END_MESSAGE_MAP()
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CColumnsList::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	
	int i;
	m_numberOfColumns = p_ListCtrl->GetHeaderCtrl()->GetItemCount();
	HDITEM hditem = {0};
	TCHAR  lpBuffer[100];
	hditem.pszText		= lpBuffer;
	hditem.mask			= HDI_TEXT;
	hditem.cchTextMax	= 100;


	str.Format( L"List %d selected lines", m_selected );
	SetWindowText( str );

	m_ListCtrl.EnableHeaderChkbox( TRUE );
	m_ListCtrl.InsertColumn( 0, L"column name", LVCFMT_LEFT, 100, -1, COL_TEXT );
	for( i=0; i < m_numberOfColumns; i++ )
	{
		VERIFY( p_ListCtrl->GetHeaderCtrl()->GetItem(i, &hditem ));
		m_ListCtrl.InsertItem( i, lpBuffer );
		m_ListCtrl.SetCheck( i, 1 );
	}
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CColumnsList::OnBnClickedOk()
{
	for( int i = 0; i < m_numberOfColumns; i++ )
	{
		selected->push_back( m_ListCtrl.GetCheck( i ) );
	}
	CDialogEx::OnOK();
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CColumnsList::OnClickedCheckUnderline()
{
	m_underline = !m_underline;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::exportAll( CString logFile, CString title, CListCtrlEx* p_ListCtrl )
{
	for( int i=0; i < p_ListCtrl->GetItemCount(); ++i)
	{
		p_ListCtrl->SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	exportSelected( logFile, title, p_ListCtrl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::exportSelected( CString logFile, CString title, CListCtrlEx* p_ListCtrl )
{
	if( p_ListCtrl->GetSelectedCount() == 0 )
	{
		message( L"", L"Nincs kijelölve egy sor sem!" );
		return;
	}

	if( openLogFile( logFile,title )==NULL) return;
	if( !listTable( fl, p_ListCtrl ) )
	{
		fclose(fl);
		return;
	}
	fclose(fl);
	showLogFile();
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CDragApp::listTable( FILE * fl, CListCtrlEx* p_ListCtrl )
{
	int	i;
	int	j;
	int	just;
	

	std::vector<COLUMNS> columns;
	COLUMNS column;

	CString		item;
	int			nItem;
	POSITION	pos = p_ListCtrl->GetFirstSelectedItemPosition();
	
	HDITEM hditem = {0};
	TCHAR  lpBuffer[100];
	hditem.pszText		= lpBuffer;
	hditem.mask			= HDI_TEXT;
	hditem.cchTextMax	= 100;

	LV_COLUMN lvcol;
	lvcol.mask = LVCF_FMT;

	int numberOfSelectedLines = p_ListCtrl->GetSelectedCount();
	if( numberOfSelectedLines == 0 )
	{
		message( L"", L"Nincs kijelölve egy sor sem!" );
		return FALSE;
	}

	CColumnsList	dlg;
	std::vector<BOOL> selected;

	selected.clear();

	dlg.p_ListCtrl = p_ListCtrl;
	dlg.selected = &selected;
	dlg.m_selected = numberOfSelectedLines;
	if( dlg.DoModal() == IDCANCEL ) return FALSE;

	BOOL m_underline = dlg.m_underline;
	int numberOfColumns = p_ListCtrl->GetHeaderCtrl()->GetItemCount();
	if( m_requested )
	{
		if( !v_notfound.size() )
		{
			if( m_requested == numberOfSelectedLines )
				str.Format( L"All %d requested items are found.\n", m_requested );
			else
				str.Format( L"%d items are found from %d requested items.\n", numberOfSelectedLines, m_requested );
		}
		else
		{
			str.Format( L"%d items are found from %d requested items. The following %d items are not found:\n", numberOfSelectedLines, m_requested, v_notfound.size() );
		}
		fwprintf( fl, str );
		for( UINT j = 0; j < v_notfound.size(); j++ )
		{
			fwprintf( fl, L"%s\n", v_notfound.at(j) );
		}
		fwprintf( fl, L"\n" );
		v_notfound.clear();
	}


	// oszopok igazításának iránya és header szélességek összegyûjtése
	columns.clear();
	for( i= 0; i < numberOfColumns; i++ )
	{
		VERIFY( p_ListCtrl->GetHeaderCtrl()->GetItem(i, &hditem ));
		column.columnname	= lpBuffer;
		lvcol.fmt = 0;
		p_ListCtrl->GetColumn(i, &lvcol);				
		just = 	lvcol.fmt & LVCFMT_JUSTIFYMASK;
		if( i == 0 ) just = LVCFMT_RIGHT;
		column.justification	= just;
		column.length			= column.columnname.GetLength();
		columns.push_back( column );
	}

	// oszlopok szélessége
	pos = p_ListCtrl->GetFirstSelectedItemPosition();
	while( pos )
	{
		nItem = p_ListCtrl->GetNextSelectedItem( pos );
		for( j = 0; j < numberOfColumns; j++ )
		{
			item = p_ListCtrl->GetItemText( nItem,j);
			item.Trim();
			columns.at(j).length = __max( columns.at(j).length, item.GetLength() );
		}
	}


	// oszop adatok kiírása
	header( fl, &selected, &columns );
	underlining( fl, &selected, &columns );

	nItem = -1;

	pos = p_ListCtrl->GetFirstSelectedItemPosition();
	while( pos )
	{
		nItem = p_ListCtrl->GetNextSelectedItem( pos );
		for( j = 0; j < numberOfColumns; j++ )
		{
			if( selected[j] )
			{
				item = p_ListCtrl->GetItemText( nItem, j );
				item.Trim();
				if( m_underline ) item.Replace(' ', '_');
				switch( columns.at(j).justification )
				{
				case LVCFMT_LEFT:
					fwprintf( fl, L"%-*s ", columns.at(j).length,  item );
					break;
				case LVCFMT_RIGHT:
					fwprintf( fl, L"%*s ", columns.at(j).length,  item );
				}
			}
		}
		fwprintf( fl, L"\n" );
	}
	underlining( fl, &selected, &columns );
	header( fl, &selected, &columns );
	return TRUE;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::underlining( FILE * fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns )
{
	for( UINT i = 0; i < columns->size(); i++ )
	{
		if( selected->at(i) )
		{
			for( int j = 0; j< columns->at(i).length; j++ ) fwprintf( fl, L"-" );
			fwprintf( fl, L" " );
		}
	}
	fwprintf( fl, L"\n" );
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragApp::header( FILE * fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns )
{
	for( UINT j = 0; j < columns->size(); j++ )
	{
		if( selected->at(j) )
		{
			if( columns->at(j).justification == LVCFMT_LEFT )
				fwprintf( fl, L"%-*s ", columns->at(j).length, columns->at(j).columnname );
			else
				fwprintf( fl, L"%*s ", columns->at(j).length, columns->at(j).columnname );
		}
	}
	fwprintf(fl,L"\n" );
}


