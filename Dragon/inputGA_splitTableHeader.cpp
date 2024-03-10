#include "stdafx.h"
#include "Dragon.h"
#include "afxdialogex.h"
#include "utilities.h"
#include "inputGA.h"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// cLine: %%[%]nnnnnnn(, titolo) (tableNumber) [�g][t�rzs: mmmmmmmm ] ///comment
//
// nnnnnnnnnnn: familyName
// 
// tableNumber: r�mai sz�m [[] azt jelzi, hogy opcion�lis
// mmmmmmmmmmm: sz�veg   ( t�nyleg [] jelek k�z�tt van
// titolo:		ha a 
// meghat�rozand�: tableNumber �s mmmmmmmm
// 
// TABLE.header			// == cLine );
// TABLE.percent		// %% vagy %%% 
// TABLE.familyName     // nnnn els� szava
// TABLE.titolo			// ha a familyName ut�n veszz� van, az comment a r�mai sz�mig vagy v�gig 	
// TABLE.number			// tableNumber
// TABLE.comment		// ha van tableNumber �s van nm�g ut�na valami
// TABLE.torzs			// mmmmmmmmmm			// r�gt�n az elej�n le kell venni
// TABLE.arm
// TABLE.knownAs		// a tableHeader-t k�vet� % knownAs sor
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

	if ((pos = cLine.Find(L"b�r�")) != -1)
		peer = L"b�r� ";
	if ((pos = cLine.Find(L"gr�f")) != -1)
		peer += L"gr�f ";
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

	cLine.Replace( '\'', '|' );	 // a nevekben el�fordulhat az ' jel, amit az SQLite nem szeret
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
		cLine = cLine.Left( pos-1 );								// nem kapcs leszed�se
	}
	else
		joint = 1;

	if( ( pos1 = cLine.Find( '<' ) ) != -1 )   // az esetleges html formatt�l�s lev�g�sa
	{
		cLine = cLine.Left( pos1 );
	}

	// sz�gletes z�r�jelek tartalm�nak meg�rz�se �s leszed�se a cLine-r�l
	while( ( pos1 = cLine.ReverseFind( '[' ) ) != -1 )			// [] z�r�jelek leszed�se
	{
		if( ( pos2 = cLine.ReverseFind( ']' ) ) != -1 )
		{
			if( pos1 < pos2 )
			{
				str		= cLine.Mid( pos1+1, pos2-pos1-1 );
				cLine	= cLine.Left( pos1-1 );
				if( ( pos1 = str.Find( L"t�rzs:" ) )!= -1 )
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
	/////////////////// romam meghat�rz�sa ////////////////////////////////////////////////////////////////////
	LPCTSTR valid	= L"IVXabcde";
	LPCTSTR validX	= L"IVX";
	CString substr;
	int		len;
	TCHAR	kar;
	int z;

	int n = splitCString(cLine, ' ', false, &A);      // cLine [] z�r�jelek, nem kapcs �s comment n�lk�l
	for ( int i = 0; i < n; ++i)
	{
		str	= A[i];
		len = str.GetLength();
		kar = str[len-1];
		if (iswalpha(kar) && iswlower(kar))
		{
			str = str.Left(len - 1);			// lehagyja a sz�v�gi bet� karaktert
			--len;
			if (!len) continue;
		}

		substr	= str.SpanIncluding(validX);
		if ( len == substr.GetLength()  )		// az utols� n�lk�l csak IVX karakterek vannak a stringben 
		{
			substr = A[i].SpanIncluding(validX);
			if ( substr.GetLength() == len)						// csak validX szerepel benne, semmi m�s
			{
				tableRoman = A[i];
				if (i + 1 > n)  // m�g van m�g�tte valami, az comment
					comment = packWords(&A, i + 1, n - i);
				break;
			}
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if( (pos = cLine.Find( '/' ) )  != -1 )						// - comment leszed�se
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
				titolo = titolo.Mid(1, titolo.GetLength() - 2); // (Benedicti) kiveszi bel�le az el�nevet
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
			if (word == L"�s") continue;
			if (word == L"ill.") continue;
			if (iswlower(word.GetAt(0))) break;			// a titolo az els� kisbet�ig tart vagy a v�g�ig
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

	cLine.Replace('\'', '|');	 // a nevekben el�fordulhat az ' jel, amit az SQLite nem szeret
	cLine.Trim();


	if ((pos = cLine.Find(L"b�r�")) != -1)
		peer = L"b�r� ";
	if ((pos = cLine.Find(L"gr�f")) != -1)
		peer += L"gr�f ";
	if ((pos = cLine.Find(L"herceg")) != -1)
		peer += L"herceg";

	m_tableHeader.peer			= peer.TrimRight();
	m_tableHeader.tableHeader	= cLine;
	m_tableHeader.tableNumber	= m_tableNumber;
	m_tableHeader.lineNumber	= m_lineNumber;
	m_tableHeader.joint			= 1;

	// sz�gletes z�r�jelek tartalm�nak meg�rz�se �s leszed�se a cLine-r�l
	while ((pos1 = cLine.ReverseFind('[')) != -1)			// [] z�r�jelek leszed�se
	{
		if ((pos2 = cLine.ReverseFind(']')) != -1)
		{
			if (pos1 < pos2)
			{
				str = cLine.Mid(pos1 + 1, pos2 - pos1 - 1);
				cLine = cLine.Left(pos1 - 1);			
				if ((pos1 = str.Find(L"t�rzs:")) != -1)
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


		if ((pos = cLine.Find('/')) != -1)						// - comment leszed�se
			cLine = cLine.Left(pos);

		m_tableHeader.familyName = getFirstWord(cLine);
		m_tableHeader.familyName.Replace(',', ' ');
		m_tableHeader.familyName.Trim();
		
		if ((pos = cLine.Find(',')) != -1)		// titolo el�tti vessz�
		{
			titolo = cLine.Mid(pos + 1);
			cLine = cLine.Left(pos);
		}
		if ((pos = cLine.Find(L"alias")) != -1)
		{
			m_tableHeader.alias = cLine.Mid(pos + 6);
		}

		// titolo meghat�roz�sa

		if ((pos = titolo.Find('/')) != -1)
		{
			titolo = titolo.Left(pos);
			m_tableHeader.comment = titolo.Mid(pos + 1);
		}

		if ((pos = titolo.Find('(')) != -1)
		{
			if (pos == 0)
				titolo = titolo.Mid(1, titolo.GetLength() - 2); // (Benedicti) kiveszi bel�le az el�nevet
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
			if (word == L"�s") continue;
			if (word == L"ill.") continue;
			if (iswlower(word.GetAt(0))) break;			// a titolo az els� kisbet�ig tart vagy a v�g�ig
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
			cLine = cLine.Left(pos - 1);								// nem kapcs leszed�se
		}
		else
			m_tableHeader.joint = 1;
		
		
		/////////////////// romam meghat�rz�sa ////////////////////////////////////////////////////////////////////
		LPCTSTR valid = L"IVXabcde";
		LPCTSTR validX = L"IVX";
		CString substr;
		int		len;
		TCHAR	kar;
		int z;

		int n = splitCString(cLine, ' ', false, &A);      // cLine [] z�r�jelek, nem kapcs �s comment n�lk�l
		for (int i = 0; i < n; ++i)
		{
			str = A[i];
			len = str.GetLength();
			kar = str[len - 1];
			if (iswalpha(kar) && iswlower(kar))
			{
				str = str.Left(len - 1);			// lehagyja a sz�v�gi bet� karaktert
				--len;
				if (!len) continue;
			}

			substr = str.SpanIncluding(validX);
			if (len == substr.GetLength())		// az utols� n�lk�l csak IVX karakterek vannak a stringben 
			{
				substr = A[i].SpanIncluding(validX);
				if (substr.GetLength() == len)						// csak validX szerepel benne, semmi m�s
				{
					m_tableHeader.tableRoman = A[i];
					if (i + 1 > n)  // m�g van m�g�tte valami, az comment
						m_tableHeader.comment = packWords(&A, i + 1, n - i);
					break;
				}
			}
		}
	}
}