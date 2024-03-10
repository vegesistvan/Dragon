#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLine: %%[%]nnnnnnn(, titolo) (tableNumber) [ág][törzs: mmmmmmmm ] ///comment
//
// nnnnnnnnnnn: familyName
// 
// tableNumber: római szám [[] azt jelzi, hogy opcionális
// mmmmmmmmmmm: szöveg   ( tényleg [] jelek között van
// titolo:		ha a 
// meghatározandó: tableNumber és mmmmmmmm
// 
// TABLE.header			// == cLine );
// TABLE.percent		// %% vagy %%% 
// TABLE.familyName     // nnnn elsõ szava
// TABLE.titolo			// ha a familyName után veszzõ van, az comment a római számig vagy végig 	
// TABLE.number			// tableNumber
// TABLE.comment		// ha van tableNumber és van nmég utána valami
// TABLE.torzs			// mmmmmmmmmm			// rögtön az elején le kell venni
// TABLE.arm
// TABLE.knownAs		// a tableHeader-t követõ % knownAs sor
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::splitTableHeader( CString cLine )  
{
	CString percent;
	CString familyName;
	CString alias;
	CString title;
	CString peer;
	CString titolo(L"");
	CString titoloFull(L"");
	CString tableRoman(L"");
	CString comment(L"");
	CString str;
	CStringArray A;
	int pos;
	int pos1;
	int	pos2;
	int	joint;

	titolo = m_tableHeader.titolo;
	titoloFull = m_tableHeader.titoloFull;

	clearTableHeader();
	++m_tableNumber;

	if ((pos = cLine.Find(L"báró")) != -1)
		peer = L"báró ";
	if ((pos = cLine.Find(L"gróf")) != -1)
		peer += L"gróf ";
	if ((pos = cLine.Find(L"herceg")) != -1)
		peer += L"herceg";

	m_tableHeader.peer			= peer.TrimRight();
	m_tableHeader.tableHeader	= cLine;
	m_tableHeader.tableNumber	= m_tableNumber;
	m_tableHeader.lineNumber	= m_lineNumber;
	
	if( cLine.Left(3) == L"%%%" )
	{
		++m_familyNumber;
		m_tableHeader.percent = cLine.Left(3 );
	}
	else
	{
		m_tableHeader.percent = cLine.Left(2);
		m_tableHeader.titolo = titolo;
		m_tableHeader.titoloFull = titoloFull;
	}
	
	m_tableHeader.familyNumber	= m_familyNumber;

	cLine.Replace( '\'', '|' );	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
	cLine.Replace( '%', ' ' );
	cLine.Trim();
	
	if( ( pos = cLine.Find( L"nem kapcs" ) ) != -1  )
	{
		joint = 0;
		str = cLine.Mid( pos );
		str = dropFirstWord( str );
		str = dropFirstWord( str );
		str.Trim();
		if( !str.IsEmpty() )
			comment = str;
		cLine = cLine.Left( pos-1 );								// nem kapcs leszedése
	}
	else
		joint = 1;

	if( ( pos1 = cLine.Find( '<' ) ) != -1 )   // az esetleges html formattálás levágása
	{
		cLine = cLine.Left( pos1 );
	}

	// szögletes zárójelek tartalmának megõrzése és leszedése a cLine-ról
	while( ( pos1 = cLine.ReverseFind( '[' ) ) != -1 )			// [] zárójelek leszedése
	{
		if( ( pos2 = cLine.ReverseFind( ']' ) ) != -1 )
		{
			if( pos1 < pos2 )
			{
				str		= cLine.Mid( pos1+1, pos2-pos1-1 );
				cLine	= cLine.Left( pos1-1 );
				if( ( pos1 = str.Find( L"törzs:" ) )!= -1 )
				{
					m_tableHeader.torzs = str.Mid( 7 );
				}
				else
					m_tableHeader.arm = str;
			}
		}
		else
			break;
	}
	/////////////////// romam meghatárzása ////////////////////////////////////////////////////////////////////
	LPCTSTR valid	= L"IVXabcde";
	LPCTSTR validX	= L"IVX";
	CString substr;
	int		len;
	TCHAR	kar;
	int z;

	int n = splitCString(cLine, ' ', false, &A);      // cLine [] zárójelek, nem kapcs és comment nélkül
	for ( int i = 0; i < n; ++i)
	{
		str	= A[i];
		len = str.GetLength();
		kar = str[len-1];
		if (iswalpha(kar) && iswlower(kar))
		{
			str = str.Left(len - 1);			// lehagyja a szóvégi betõ karaktert
			--len;
			if (!len) continue;
		}

		substr	= str.SpanIncluding(validX);
		if ( len == substr.GetLength()  )		// az utolsó nélkül csak IVX karakterek vannak a stringben 
		{
			substr = A[i].SpanIncluding(validX);
			if ( substr.GetLength() == len)						// csak validX szerepel benne, semmi más
			{
				tableRoman = A[i];
				if (i + 1 > n)  // még van mögötte valami, az comment
					comment = packWords(&A, i + 1, n - i);
				break;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( (pos = cLine.Find( '/' ) )  != -1 )						// - comment leszedése
		cLine = cLine.Left( pos );

	familyName = getFirstWord(cLine);

	if( ( pos = cLine.Find( ',' ) ) != -1 )
	{
		familyName.Replace( ',',' ' );
		familyName.Trim();
		titolo	= cLine.Mid( pos+1 );
		cLine	= cLine.Left( pos );
	}
	if( ( pos = cLine.Find( L"alias" ) ) != -1 )
	{
		alias = cLine.Mid( pos+6 );
	}


	if (m_tableHeader.percent == L"%%%")
	{
		if ((pos = titolo.Find('/')) != -1)
			titolo = titolo.Left(pos);

		if ((pos = titolo.Find('(')) != -1)
		{
			if (pos == 0)
				titolo = titolo.Mid(1, titolo.GetLength() - 2); // (Benedicti) kiveszi belõle az elõnevet
			else
				titolo = titolo.Left(pos);
		}

		titolo.Trim();

		CString word;
		int i;
		n = splitCString(titolo, ' ', false, &A);
		for (i = 0; i < n; ++i)
		{
			word = A[i];
			if (word == L"és") continue;
			if (word == L"ill.") continue;
			if (iswlower(word.GetAt(0))) break;			// a titolo az elsõ kisbetûig tart vagy a végéig
		}
		titolo = packWords(&A, 0, i);

		if (m_tableHeader.percent == "%%%")
		{
			m_tableHeader.titoloFull = titolo;
			m_tableHeader.titolo = titolo;
			if ((pos = titolo.Find(L"ill.")) != -1)
				m_tableHeader.titolo = titolo.Left(pos);
		}
	}

	m_familyName				= familyName.Trim();
	m_tableHeader.familyName	= m_familyName;
	m_tableHeader.tableRoman	= tableRoman;
	m_tableHeader.comment		= comment.Trim();
	m_tableHeader.joint			= joint;
	m_tableHeader.alias			= alias;
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::clearTableHeader()
{
	m_tableHeader.arm.Empty();
	m_tableHeader.comment.Empty();
	m_tableHeader.familyName.Empty();
	m_tableHeader.alias.Empty();
	m_tableHeader.familyNumber = 0;
	m_tableHeader.folyt.Empty();
	m_tableHeader.joint = 0;
	m_tableHeader.known_as.Empty();
	m_tableHeader.lineNumber = 0;
	m_tableHeader.percent.Empty();
	m_tableHeader.torzs.Empty();
	m_tableHeader.csalad.Empty();
	m_tableHeader.tableHeader.Empty();
	m_tableHeader.tableNumber = 0;
	m_tableHeader.tableRoman.Empty();
	m_tableHeader.titolo.Empty();
	m_tableHeader.peer.Empty();
	m_tableHeader.ancestor.Empty();
	
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CInputGA::splitTableHeader2(CString cLine)
{
	int pos;
	int pos1;
	int pos2;

	int n;
	CStringArray A;
	CString peer;
	CString titolo;
	


	clearTableHeader();
	++m_tableNumber;

	cLine.Replace('\'', '|');	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
	cLine.Trim();


	if ((pos = cLine.Find(L"báró")) != -1)
		peer = L"báró ";
	if ((pos = cLine.Find(L"gróf")) != -1)
		peer += L"gróf ";
	if ((pos = cLine.Find(L"herceg")) != -1)
		peer += L"herceg";

	m_tableHeader.peer			= peer.TrimRight();
	m_tableHeader.tableHeader	= cLine;
	m_tableHeader.tableNumber	= m_tableNumber;
	m_tableHeader.lineNumber	= m_lineNumber;
	m_tableHeader.joint			= 1;

	// szögletes zárójelek tartalmának megõrzése és leszedése a cLine-ról
	while ((pos1 = cLine.ReverseFind('[')) != -1)			// [] zárójelek leszedése
	{
		if ((pos2 = cLine.ReverseFind(']')) != -1)
		{
			if (pos1 < pos2)
			{
				str = cLine.Mid(pos1 + 1, pos2 - pos1 - 1);
				cLine = cLine.Left(pos1 - 1);			
				if ((pos1 = str.Find(L"törzs:")) != -1)
				{
					m_tableHeader.torzs = str.Mid(7);
				}
				else
					m_tableHeader.arm = str;
			}
		}
		else
			break;
	}
		

	if (cLine.Left(3) == L"%%%")
	{
		++m_familyNumber;
		m_tableHeader.familyNumber = m_familyNumber;
		m_tableHeader.percent = L"%%%";

		cLine = cLine.Mid(3);
		cLine.Trim();


		if ((pos = cLine.Find('/')) != -1)						// - comment leszedése
			cLine = cLine.Left(pos);

		m_tableHeader.familyName = getFirstWord(cLine);
		m_tableHeader.familyName.Replace(',', ' ');
		m_tableHeader.familyName.Trim();
		
		if ((pos = cLine.Find(',')) != -1)		// titolo elõtti vesszõ
		{
			titolo = cLine.Mid(pos + 1);
			cLine = cLine.Left(pos);
		}
		if ((pos = cLine.Find(L"alias")) != -1)
		{
			m_tableHeader.alias = cLine.Mid(pos + 6);
		}

		// titolo meghatározása

		if ((pos = titolo.Find('/')) != -1)
		{
			titolo = titolo.Left(pos);
			m_tableHeader.comment = titolo.Mid(pos + 1);
		}

		if ((pos = titolo.Find('(')) != -1)
		{
			if (pos == 0)
				titolo = titolo.Mid(1, titolo.GetLength() - 2); // (Benedicti) kiveszi belõle az elõnevet
			else
				titolo = titolo.Left(pos);
		}
		titolo.Trim();

		CString word;
		int i;
		n = splitCString(titolo, ' ', false, &A);
		for (i = 0; i < n; ++i)
		{
			word = A[i];
			if (word == L"és") continue;
			if (word == L"ill.") continue;
			if (iswlower(word.GetAt(0))) break;			// a titolo az elsõ kisbetûig tart vagy a végéig
		}
		m_tableHeader.titoloFull = packWords(&A, 0, i);
		m_tableHeader.comment = packWords(&A, i, n - i );

		m_tableHeader.titolo = m_tableHeader.titoloFull;
		m_titolo = m_tableHeader.titolo;

		if ((pos = m_tableHeader.titoloFull.Find(L"ill.")) != -1)
			m_tableHeader.titolo = m_tableHeader.titoloFull.Left(pos);
	}
	else
	{
		m_tableHeader.percent = L"%%";
		m_tableHeader.familyNumber = m_familyNumber;

		cLine = dropFirstWord(cLine);
		m_tableHeader.familyName = getFirstWord(cLine);
		m_tableHeader.familyName.Replace(',', ' ');
		m_tableHeader.familyName.Trim();
		m_tableHeader.titolo = m_titolo;

		if ((pos = cLine.Find(L"nem kapcs")) != -1)
		{
			m_tableHeader.joint = 0;
			str = cLine.Mid(pos);
			str = dropFirstWord(str);
			str = dropFirstWord(str);
			str.Trim();
			if (!str.IsEmpty())
				m_tableHeader.comment = str;
			cLine = cLine.Left(pos - 1);								// nem kapcs leszedése
		}
		else
			m_tableHeader.joint = 1;
		
		
		/////////////////// romam meghatárzása ////////////////////////////////////////////////////////////////////
		LPCTSTR valid = L"IVXabcde";
		LPCTSTR validX = L"IVX";
		CString substr;
		int		len;
		TCHAR	kar;
		int z;

		int n = splitCString(cLine, ' ', false, &A);      // cLine [] zárójelek, nem kapcs és comment nélkül
		for (int i = 0; i < n; ++i)
		{
			str = A[i];
			len = str.GetLength();
			kar = str[len - 1];
			if (iswalpha(kar) && iswlower(kar))
			{
				str = str.Left(len - 1);			// lehagyja a szóvégi betõ karaktert
				--len;
				if (!len) continue;
			}

			substr = str.SpanIncluding(validX);
			if (len == substr.GetLength())		// az utolsó nélkül csak IVX karakterek vannak a stringben 
			{
				substr = A[i].SpanIncluding(validX);
				if (substr.GetLength() == len)						// csak validX szerepel benne, semmi más
				{
					m_tableHeader.tableRoman = A[i];
					if (i + 1 > n)  // még van mögötte valami, az comment
						m_tableHeader.comment = packWords(&A, i + 1, n - i);
					break;
				}
			}
		}
	}
}