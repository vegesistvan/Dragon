// ascendantsDirect.cpp : implementation file
//

#include "pch.h"
#include "Dragon.h"
#include "ascendantsDirect.h"


// CAscendantsDirect

IMPLEMENT_DYNAMIC(CAscendantsDirect, CWnd)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsDirect::CAscendantsDirect()
{
	m_bold	= true;
	m_code	= ANSI;
	m_cnt	= 0;
	m_ulStyle = L"<ul style=list-style-type:none;><li>";
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CAscendantsDirect::~CAscendantsDirect()
{
}
BEGIN_MESSAGE_MAP(CAscendantsDirect, CWnd)
END_MESSAGE_MAP()
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ascendenza paterna
// Aztért jó az adatbázisból elõkeresni az apát, mert a vGKR vektorban nem biztos, hogy minden generáció benne van!!
void CAscendantsDirect::directFatherAscendants()
{
	m_command.Format(L"SELECT father_id FROM people WHERE rowid='%s'", m_rowid);
	if (!query(m_command)) return;
	if (!m_recordset.RecordsCount())
	{
		str.Format(L"%s-nek nincs apja!", m_name);
		AfxMessageBox(str);
	}
	CString father_id = m_recordset.GetFieldString(0);
//	if (father_id.IsEmpty() || father_id == L"0" || _wtoi(father_id) > NOFATHERID)
	if ( father_id == L"0" )
	{
		str.Format(L"%s-nek nincs apja!", m_name);
		AfxMessageBox(str);
		return;
	}

	CString title = L"ismert egyenesági apai felmenõi";
	CString filename = L"egyenesApaiFelmenok";
	openHtml(filename, title);
	listPeople(m_rowid, 0);
	ascendants( m_rowid, 1);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// ascendenza materna
void CAscendantsDirect::directMotherAscendants()
{
	m_command.Format(L"SELECT mother_id FROM people WHERE rowid='%s'", m_rowid);
	if (!query(m_command)) return;
	if (!m_recordset.RecordsCount())
	{
		str.Format(L"%s-nek nincs anyjea", m_name);
		AfxMessageBox(str);
	}
	CString mother_id = m_recordset.GetFieldString(0);
//	if (mother_id.IsEmpty() || mother_id == L"0" || _wtoi(mother_id) > NOFATHERID)
	if ( mother_id == L"0" )
	{
		str.Format(L"%s-nek nincs anyjea", m_name);
		AfxMessageBox(str);
		return;
	}

	CString title = L"ismert egyenesági anyai felmenõi";
	CString filename = L"egyenesAnyaiFelmenok";
	openHtml(filename, title);

	listPeople(m_rowid, 0);
	listPeople(mother_id, 1);
	ascendants( mother_id, 2);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsDirect::ascendants( CString rowid, int g )
{
	m_cnt_ul = 0;
	for (UINT i = 0; ; ++i)
	{
		m_command.Format(L"SELECT father_id FROM people WHERE rowid='%s'", rowid);
		if (!query(m_command)) return;

		rowid = m_recordset.GetFieldString(0);
	//	if (rowid.IsEmpty() || rowid == L"0" || _wtoi(rowid) > NOFATHERID)  break;
		if ( rowid == L"0" )  break;
		listPeople(rowid, i+g );
	}

	for (int j = 0; j < m_cnt_ul; ++j)
		fwprintf(fh, L"</ul>\n");

	str.Format(L"<br><br>Az ismert felmenõk száma: %d\n", m_cnt-1);
	print(str);

	fclose(fh);
	ShellExecute(NULL, L"open", m_filespec, NULL, NULL, SW_SHOWNORMAL);
	return;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool CAscendantsDirect::openHtml(CString file, CString title)
{
	CString filename;
	filename.Format(L"%s_%s", m_name, file);
	m_filespec = theApp.openHtmlFile(&fh, filename, L"w+");
	if (m_filespec.IsEmpty()) return false;

	CString program;
	program.Format(L"Dragon version %s", theApp.m_version);

	CString head;
	head = L"<!DOCTYPE html>\n<html>\n<head>\n</head>\n<body>";
	if (m_code == UTF8)
	{
		head = L"<!DOCTYPE html>\n<html>\n<head>\n<meta charset=\"UTF-8\">\n</head>\n<body>";
		head = UnicodeToUtf8(head);
	}
	fwprintf(fh, L"%s\n", head);


	str.Format(L"%s %s", m_name, title);
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"<center>%s</center>\n", str);

	fwprintf(fh, L"<pre>");
	str.Format(L"%s %s\n", L"program                           :", program);
	print(str);
	str.Format(L"%s %s\n", L"adatbázis                         :", theApp.m_dbPathName);
	print(str);
	str.Format(L"%s %s\n", L"listafájl                         :", m_filespec);
	print(str);
	str.Format(L"%s %s\n", L"lista készült                     :", theApp.getPresentDateTime());
	print(str);
	fwprintf(fh, L"</pre>\n");
	return true;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CAscendantsDirect::getPeopleString( CString rowid )
{
	CString people;

	CString sp(L" ");
	m_command.Format(L"SELECT rowid,* FROM people WHERE rowid='%s'", rowid);
	if (!query(m_command)) return(L"");

	CString	str;
	CString str1;
	CString generation;
	CString title;
	CString titolo;
	CString last_name;
	CString	first_name;
	CString	birth_place;
	CString	birth_date;
	CString death_place;
	CString	death_date;
	CString father_id;
	CString mother_id;
	CString peer;
	int		parentIndex;
	CString comment;
	CString arm;

	generation = m_recordset.GetFieldString(DBP_GENERATION);
	title = m_recordset.GetFieldString(DBP_TITLE);
	titolo = m_recordset.GetFieldString(DBP_TITOLO);
	last_name = m_recordset.GetFieldString(DBP_LAST_NAME);
	first_name = m_recordset.GetFieldString(DBP_FIRST_NAME);
	birth_place = m_recordset.GetFieldString(DBP_BIRTH_PLACE);
	birth_date = m_recordset.GetFieldString(DBP_BIRTH_DATE);
	death_place = m_recordset.GetFieldString(DBP_DEATH_PLACE);
	death_date = m_recordset.GetFieldString(DBP_DEATH_DATE);
	father_id = m_recordset.GetFieldString(DBP_FATHER_ID);
	mother_id = m_recordset.GetFieldString(DBP_MOTHER_ID);
	parentIndex = _wtoi(m_recordset.GetFieldString(DBP_PARENTINDEX));
	comment = m_recordset.GetFieldString(DBP_COMMENT);
	arm = m_recordset.GetFieldString(DBP_ARM);
	peer = m_recordset.GetFieldString(DBP_PEER);

	m_name.Format(L"%s %s", last_name, first_name);

	if (peer == L"gróf")
		peer = L"gr";
	else if (peer == L"báró")
		peer = L"br";
	else if (peer == L"herceg")
		peer = L"hg";

	if (m_bold)
		str = L"<b>";
	else
		str.Empty();

	if (!peer.IsEmpty()) { str += peer, str += L" "; };
	if (!title.IsEmpty()) { str += title, str += " "; }
	if (!titolo.IsEmpty()) { str += titolo, str += L" "; }
	if (!last_name.IsEmpty()) { str += last_name, str += L" "; };
	if (!first_name.IsEmpty()) { str += first_name, str += L" "; };


	str.TrimRight();

	if (m_bold) str += L"</b>";

	if (parentIndex)
	{
		str1.Format(L"/%d", parentIndex);
		str += str1;
	}

	if (!birth_place.IsEmpty() || !birth_date.IsEmpty())
	{
		str += L" *";
		if (!birth_place.IsEmpty()) str += birth_place;
		if (!birth_date.IsEmpty())
		{
			if (!birth_place.IsEmpty()) str += sp;
			str += birth_date;
		}
	}

	if (!death_place.IsEmpty() || !death_date.IsEmpty())
	{
		str += L" +";
		if (!death_place.IsEmpty()) str += death_place;
		if (!death_date.IsEmpty())
		{
			if (!death_place.IsEmpty()) str += sp;
			str += death_date;
		}
	}

	if (!comment.IsEmpty()) { str += " "; str += comment; }

	if (!arm.IsEmpty())
	{
		str += L" [";
		str += arm;
		str += L"]";
	}
	return str;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsDirect::listPeople( CString rowid, UINT g )
{
	CString people = getPeopleString( rowid );
	str.Format(L"%s&diams;%s\n", m_ulStyle, people);
	print(str);
	++m_cnt_ul;
	++m_cnt;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CAscendantsDirect::print(CString)
{
	if (m_code == UTF8) str = UnicodeToUtf8(str);
	fwprintf(fh, L"%s", str);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BOOL CAscendantsDirect::query(CString command)
{
	if (m_recordset.Query(command, theApp.mainDB))
	{
		str.Format(L"%s\n%s", command, m_recordset.GetLastError());
		AfxMessageBox(str);
		return FALSE;
	}
	return TRUE;
}