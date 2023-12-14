#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::splitLine( CString cLine)
{
	GENERATIONS	gen;	// a gener�ci�, az apa �s max 10 feles�g azonoos�t�ja, amit a v_genertion-ben �riz k�s�bbi felhaszn�l�sra
	clearAll();
	m_cLine = cLine;									// meg�rzi a teljes sort esetleges k�s�bbi felhaszn�l�sra ( hibajelz�s )
	cLine	= getBranch( cLine );
/*
	if (TEST)
	{
		fwprintf(textF, L"%d\n", m_lineNumber);
		fflush(textF);
	}
*/
	m_descendant = getDescendant( cLine );				// leszedi r�la a marrige-eket
	getMarriageSubstrings( cLine );						// marriage substringeket a v_marriages vektorba teszi


	processDescendantSubstring(m_descendant);			// d strukt�r�ba teszi az elemeket
	processSpouseSubstrings();

	for (UINT i = 0; i < 15; ++i)	gen.spouse_id[i] = L"0";	// egy lesz�rmazottnak max 10 h�zast�rs�nak rowid-j�t tartja nyilv�n majd
	gen.gen					= d.generation;				// gener�ci�
	gen.descendant_sex_id	= d.sex_id;					// lesz�rmazott neme
	gen.numOfSpouses		= v_marriages.size();		// h�zast�rsak sz�ma
	gen.parentIndex			= d.parentIndex;			// anya-index  ( apja hanyadik feles�gae az anyja )
	gen.whichChildFather	= 1;						// 
	gen.whichChildMother	= 1;						// 
	gen.familyName			= m_familyName;
	gen.familyName			= d.last_name;
	gen.title				= d.title;
	gen.peer				= d.peer;


/*
	if (v_marriages.size() > 10)
	{
		str.Format(L"%d h�zast�rs nem f�r el a v_generation vektorban!!", v_generations.size());
		AfxMessageBox(str, MB_ICONERROR);
		return;
	}


	for (UINT i = 0; i < v_marriages.size(); ++i)
	{
		gen.spouse_id[i] = v_marriages.at(i).rowid;		// beteszi az aktu�lis h�zast�rsak rowid-jeit!!! nincs m�g rowid!!
	}
*/
	v_generations.push_back(gen);						// meg�rzi a leend� ap�k, any�k azonos�t�t k�s�bbio felhaszn�l�sra
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Leszedi a sor v�g�r�l az el�gaz�s jelz�s�t, amit megtiszt�tva a d.folyt v�ltoz�ba tesz;
// valmaint az m_tableHeader.folyt.ban is akkumul�lja
// A megtiszt�tott sort visszadja.
CString CInputGA::getBranch( CString cLine )
{
	int pos;
	CString folyt;
	int z;
	if( ( pos = cLine.Find( L"%%% folyt" ) ) != -1 )
	{
//		z = cLine.GetLength();						// rejt�lyes m�don Debug m�dban en�lk�l lerobban a 39146.sorn�l
		str		= cLine.Mid( pos + 10 );
		folyt	= getFirstWord( str );				// lehet a romanNumber ut�n m�g valami feljegyz�s!!
		d.folyt	= folyt;
		folyt += L",";
		m_tableHeader.folyt += folyt;				// m_tabelHeader-ben gy�jti a t�bl�ban el�fordul� folyt r�mai sz�mait list�z�shoz

		cLine = cLine.Left(pos - 1);				// lehagyja a %%% jeleket
	}


	int pos1;
	int pos2;
	CString root;
	CString csalad;

	// ha a sor v�g�n [root] -et tal�l
	while( (pos2 = cLine.ReverseFind( ']' ) ) != -1 )
	{
		if( cLine.GetAt( cLine.GetLength() - 1 ) == ']' )		// a cLine v�g�n van
		{
			if( ( pos1 = cLine.ReverseFind( '[' ) ) != -1 )
			{
				if( pos1 < pos2 )
				{
					if( ( pos = cLine.Find( L"csal�d �se]" ) ) != -1  )
						d.gap	= 1;
					if( ( pos = cLine.Find( L"csal�d]" ) ) != -1 ||  ( pos = cLine.Find( L"csal�d �se]" ) ) != -1 )
					{
						root = cLine.Mid( pos1+1, pos2-pos1 - 1 );
						if( iswupper( root.GetAt( 0 ) ) )
						{
							csalad = getCsalad( root );
							d.csalad = csalad;
							csalad += L",";
							m_tableHeader.csalad += csalad;
						}
					}
					else
					{
						root = cLine.Mid( pos1+1, pos2-pos1 - 1 );
						d.arm = root;
					}
				}
			}
			cLine = cLine.Left( pos1 - 1 );
		}
		else
			break;
	}
	
	return cLine;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::getCsalad( CString root )
{
	CString csalad;
	CString first;
	CString roman;
	CStringArray A;
	int n;

	n = splitCString( root, ' ', false, &A );
	csalad = A[0];
	n = csalad.GetLength();
	if( csalad.GetAt( n-1 ) == ',' )
		csalad = csalad.Left( n-1 );

	if( isRoman( A[1] ) )
	{
		csalad += L" ";
		csalad += A[1];
	}
	return csalad;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CString CInputGA::getDescendant( CString cLine )
{
	int pos;

	m_descendant = cLine;
	if( (pos= cLine.Find( '=' ) ) != -1 )
	{
		m_descendant	= cLine.Left( pos - 1 );	// az esetleges n= h�zass�g sorsz�mot lev�gja
		m_descendant.TrimRight();					// ha volt h�zass�g sorsz�m, akkor a spsce-t is lev�gja
	}
	return m_descendant;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// marriageSubstr -ek bet�lt�se v_marriages vektorba
void CInputGA::getMarriageSubstrings( CString cLine )
{
	int order;
	int	pos;
	int posM = 0;
	int posNext;
	CString marriageSubstring;
	MARRIAGES	marriagesS;		// h�zass�g

	v_marriages.clear();
	while( ( pos = cLine.Find( '=', posM ) ) != -1 )		// minden h�zass�got megkeres
	{
		posM = pos + 1;
		//order = _wtoi( cLine.Mid( pos-1, 1 ) );
		//if( order == 0 ) order = 1;							// a h�zass�g sorsz�ma (ha nincs ki�rva, akkor 1 )
		//marriagesS.order	= order;
		//if( order )
		//	marriagesS.orderSpouse = order;  // itt 1 volt!!
		marriagesS.orderSpouse = cLine.Mid(pos - 1, 1);
		marriagesS.orderSpouse.Trim();

		if( ( posNext = cLine.Find( '=', posM ) ) == -1 )	// nincs t�bb h�zass�g
			posNext = cLine.GetLength()+1;
	
		marriageSubstring = cLine.Mid( pos+1, posNext-pos-2 );
		marriageSubstring.Trim();
		if( marriageSubstring.IsEmpty() ) break;  // ez hib�s GA.html-n�l fordulhat el�

		marriagesS.marriageSubstr = marriageSubstring;
		v_marriages.push_back( marriagesS );
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::clearAll()
{
/*
	d = { 0 };
	s = { 0 };
	sf = { 0 };
	sm = { 0 };
	ss = { 0 };
*/
	// egyes elemeknek �rt�ket ad!!!
	clearPeople( &d );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &s );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &sf );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &sm );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra
	clearPeople( &ss );		// az insert�l�shoz haszn�lt PEOPLE strukt�ra


	v_marriages.clear(); 
	v_spouseSpouses.clear();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::clearPeople(PEOPLE* p)
{
	p->rowid.Empty();
	p->fileNumber = 0;
	p->familyNumber = 0;
	p->tableNumber.Empty();
	p->lineNumber = 0;
	p->source = 0;
	p->united = 0;
	p->spouse = 0;
	p->spouseparent = 0;
	p->spousespouse = 0;
	p->generation = ' ';
	p->sex_id.Empty();
	p->peer.Empty();
	p->title.Empty();
	p->titolo.Empty();
	p->first_name.Empty();
	p->last_name.Empty();
	p->posterior.Empty();
	p->other_names.Empty();
	p->birth_place.Empty();
	p->birth_date.Empty();
	p->death_place.Empty();
	p->death_date.Empty();
	p->comment.Empty();
	p->tableAncestry = 0;
	p->tableRoman.Empty();
	p->arm.Empty();
	p->sources.Empty();
	p->occupation.Empty();
	p->csalad.Empty();
	p->gap = 0;
	p->folyt.Empty();
	p->father_id = L"0";
	p->mother_id = L"0";
	p->parentIndex = 1;
	p->whichHusband = 0;
	p->whichWife = 0;
	p->whichChildFather = 1;
	p->whichChildMother = 1;
	p->numOfSpouses = 0;
	p->numOfChildren = 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::splitToSubstr(CString line)
{
	m_birthSubstr.Empty();
	m_deathSubstr.Empty();

	int		pos;
	if ((pos = line.Find('+')) != -1)
	{
		m_deathSubstr = line.Mid(pos + 1);		// hal�loz�si blokk
		line = line.Left(pos);
	}
	if ((pos = line.Find('*')) != -1)
	{
		m_birthSubstr = line.Mid(pos + 1);		// sz�let�si blokk
		line = line.Left(pos);
	}
	m_nameSubstr = line;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/*
CString CInputGA::cutBirthDeath(CString line, CString *birthdeath )
{
	CString b;
	CString d;
	int		pos;
	if ((pos = line.Find('*')) != -1)
	{
		m_birthSubstr = line.Mid(pos);
		line = line.Left(pos - 1);

	}
	if ((pos = line.Find('+')) != -1)
	{
		m_deathSubstr = line.Mid(pos);
		line = line.Left(pos-1);
	}
	str.Format(L"%s %s", m_birthSubstr, m_deathSubstr );
	str.Trim();
	*birthdeath = str;
	return line;
}
*/