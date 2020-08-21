#include "stdafx.h"
#include "Dragon.h"
#include "ProgressWnd.h"
#include "ColumnsList.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::exportAll( CString logFile, CString title, CListCtrlEx* p_ListCtrl )
{
//	int n = p_ListCtrl->GetItemCount();
	for( int i=0; i < p_ListCtrl->GetItemCount(); ++i)
	{
		p_ListCtrl->SetItemState(i, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED);
	}
	exportSelected( logFile, title, p_ListCtrl );
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CDragonApp::exportSelected( CString logFile, CString title, CListCtrlEx* p_ListCtrl )
{
	if( p_ListCtrl->GetSelectedCount() == 0 )
	{
		message( L"", L"Nincs kijelölve egy sor sem!" );
//		AfxMessageBox( L"No selected lines to list!" );
		return;
	}
//	int n = p_ListCtrl->GetSelectedCount();

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
BOOL CDragonApp::listTable( FILE * fl, CListCtrlEx* p_ListCtrl )
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
//		AfxMessageBox( L"No selected lines to list!" );
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
void CDragonApp::underlining( FILE * fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns )
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
void CDragonApp::header( FILE * fl, std::vector<BOOL>* selected, std::vector<COLUMNS>* columns )
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

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
int selectFromTextFile( CListCtrlEx* p_ListCtrl, int column, CString caption  )
{
	CSelectTxtFile dlg;

	CStringArray	A;

	CString		fileSpec;
	CString		cLine;
	CString		line;
	BOOL		FOUND;

	int			nItem;
	int			n = 0;
	int			numberoflines_requested = 0;


	str.Format(L"Select text file containing %s!", caption ); 
	dlg.m_caption		= str;
	str.Format(L"The text file should contain 1 column containing %s names!", caption ); 
	dlg.m_description	= str;   

	if( dlg.DoModal() == IDCANCEL ) return 0;
	fileSpec = dlg.m_txtfile;

	CStdioFile file(fileSpec, CFile::modeRead); 
	while(file.ReadString(cLine))++n; 

	// unselect all lines
	for( int i=0; i < p_ListCtrl->GetItemCount(); ++i)
	{
		p_ListCtrl->SetItemState(i, 0, LVIS_SELECTED|LVIS_FOCUSED);
	}
	m_requested = 0;
	v_notfound.clear();
	CProgressWnd wndProgress(NULL,L"Searching for selected lines..." ); 
	wndProgress.GoModal();
	wndProgress.SetRange(0,n);
	wndProgress.SetPos(0);
	wndProgress.SetStep(1);

	file.SeekToBegin();
	while(file.ReadString(cLine)) 
	{
		if(cLine.GetLength() == 0) continue;
		A.RemoveAll();
		n = wordList( &A,cLine,' ',MORE );				// sor értékeinek felbontása a dataA tömbbe 
		if( n != 1) continue;
		line = A[0];
		FOUND = FALSE;
		++numberoflines_requested;
		++m_requested;
		for( nItem=0; nItem < p_ListCtrl->GetItemCount(); nItem++ )		// ha egy keresett elembõl több van, mindet kijelöli!!
		{
			if( line == p_ListCtrl->GetItemText( nItem, column ) )
			{
				p_ListCtrl->SetItemState( nItem, LVIS_SELECTED|LVIS_FOCUSED, LVIS_SELECTED|LVIS_FOCUSED );
				FOUND = TRUE;
			}
		}
		if( !FOUND )
			v_notfound.push_back( line );
		wndProgress.StepIt();
		wndProgress.PeekAndPump();
		if (wndProgress.Cancelled()) break;
	}
	wndProgress.DestroyWindow();
	return numberoflines_requested;
}
*/
