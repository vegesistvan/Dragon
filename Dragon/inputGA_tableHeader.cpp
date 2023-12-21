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
void CInputGA::splitTableHeader(CString cLine)
{
	CStringArray A;
	int pos;
	int pos1;
	int pos2;
	int i;
	int n;

// tableHeader v�ltoz�k //////////////////////////////////////////////
	
	CString percent;		// %%% vagy %%
	CString familyName;		// a tableHeaddr-b�l kivett csal�dn�v
	CString titolo;			// a tableHeaderb�l kivett el�n�v, ill. el�tt �ll� 
	CString alias;			// a tableHeaderb�l kivett alias
	CString tableRoman;		// a t�bla r�mai sz�ma a csal�don bel�l
	int		joint = 1;			// 0 = nem kapcs
	CString torzs;
	CString arm;
	CString comment;		// a tableHeaderb�l kivett comment
	CString tableHeader;	// a teljes t�bla sor
	int z;
////////////////////////////////////////////////////////////////////
	cLine.Replace('\'', '|');	 // a nevekben el�fordulhat az ' jel, amit az SQLite nem szeret
	if ((pos = cLine.Find('<')) != -1)   // az esetleges html formatt�l�s lev�g�sa
		cLine = cLine.Left(pos);
	tableHeader = cLine;

	if ((pos = cLine.Find(L"Svaiczer")) != -1)
		z = 1;


	if ((pos = cLine.Find('/')) != -1)
	{
		comment = cLine.Mid(pos);
		cLine = cLine.Left(pos - 1);
	}

	percent = getFirstWord(cLine);
	cLine.Remove('%');
	cLine.Trim();

	++m_tableNumber;

	m_peerHeader.Empty();
	CString peerP;
	for (int i = 0; i < sizeof(peers); ++i)
	{
		peerP = peers[i].lg + L"+";
		if (cLine.Find(peerP) == -1 && cLine.Find(peers[i].lg) != -1 )
		{
			m_peerHeader = peers[i].lg;
			break;
		}
	}

	// sz�gletes z�r�jelek tartalm�nak meg�rz�se �s leszed�se a cLine-r�l
	if ((pos1 = cLine.ReverseFind('[')) != -1)			// [] z�r�jelek leszed�se
	{
		if ((pos2 = cLine.ReverseFind(']')) != -1)
		{
			if (pos1 < pos2)
			{
				str = cLine.Mid(pos1 + 1, pos2 - pos1 - 1);
				cLine = cLine.Left(pos1 - 1);
				if ((pos1 = str.Find(L"t�rzs:")) != -1)
					torzs = str.Mid(7);
				else
					arm = str;
			}
		}
	}

	if (percent == "%%")
	{
		titolo = m_tableHeader.titolo;		// a %% t�bl�k �r�klik az el�z� t�bla titolo-j�t, de ez fel�l�rhat�

		if ((pos = cLine.Find(L"nem kapcs")) != -1)
		{
			joint = 0;
			str = cLine.Mid(pos);
			str = dropFirstWord(str);
			str = dropFirstWord(str);
			str.Trim();
			if (!str.IsEmpty())
				comment = str;
			cLine = cLine.Left(pos - 1);								// nem kapcs leszed�se
		}

		/////////////////// romam meghat�rz�sa ////////////////////////////////////////////////////////////////////
		LPCTSTR valid = L"IVXabcde"; // a r�mai t�blasz�mban lehetnek bet�k!!!
		LPCTSTR validX = L"IVX";
		CString substr;
		int		len;
		TCHAR	kar;
		

		n = splitCString(cLine, ' ', false, &A);      // cLine [] z�r�jelek, nem kapcs �s comment n�lk�l
		for (i = 0; i < n; ++i)
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
					tableRoman = A[i];
					if (i + 1 > n)  // m�g van m�g�tte valami, az comment
						comment = packWords(&A, i + 1, n - i);
					break;
				}
			}
		}
		if (!tableRoman.IsEmpty())
		{
			str = packWords(&A, 0, i);
			cLine = str;
		}
	}
	else
	{
		titolo.Empty();
		++m_familyNumber;
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	// csal�dn�v[, el�n�v] alias X feldolgoz�sa

	if ((pos = cLine.Find(L"alias")) != -1)
	{
		alias = cLine.Mid(pos + 6);
		alias.Remove(')');
		alias.Trim();
		cLine = cLine.Left(pos-1);
	}

	familyName = getFirstWord(cLine);
	if ((pos = familyName.Find(',')) != -1)
	{
		familyName.Replace(',', ' ');
		familyName.Trim();
		titolo = cLine.Mid(pos + 1);
	}
	else									// %%% Antall (Antal), Kisjen�i
	{
//		str = getWord( cLine, 2, &pos1 );   // n�v ut�ni (m�s nevek),
		if (cLine.Find('(') != -1)
		{
			if ((pos = cLine.Find(')')) != -1)
			{
				if (cLine.GetAt(pos + 1) == ',')
				{
					titolo = cLine.Mid(pos + 3);
				}
			}
		}
	}

	// el�n�v pontos�t�sa

	if ((pos = titolo.Find('(')) != -1)
	{
		if (pos == 0)
			titolo = titolo.Mid(1, titolo.GetLength() - 2); // (Benedicti) kiveszi bel�le az el�nevet
		else
			titolo = titolo.Left(pos);
	}
	titolo.Trim();

	str = getSecondWord(titolo);
//	if ( str[0] == '-' || str[0] == '[' || isStringUpper( str ) || isdigit( str[0] ) )
	if (!isalnum(str[0] )|| isStringUpper(str) || isdigit(str[0]))
		titolo = getFirstWord(titolo);

	CString word;
	n = splitCString(titolo, ' ', false, &A);
	for (i = 0; i < n; ++i)
	{
		word = A[i];
		if (word == L"�s") continue;
		if (word == L"ill.") continue;
		if (iswlower(word.GetAt(0))) break;			// a titolo az els� kisbet�ig tart vagy a v�g�ig
	}
	titolo = packWords(&A, 0, i);

	if ((pos = titolo.Find(L"ill.")) != -1)
		titolo = titolo.Left(pos);

	if (isLastCharacter(titolo, ','))
		titolo = dropLastCharacter(titolo);
//	if ((pos = titolo.Find('-')) != -1)
//		titolo = titolo.Left(pos);


// tableHeader strukt�ra kit�lt�se
	m_tableHeader.tableNumber = m_tableNumber;		// ezek glob�lis counterek
	m_tableHeader.lineNumber = m_lineNumber;
	m_tableHeader.familyNumber = m_familyNumber;

	m_tableHeader.percent		= percent;
	m_tableHeader.peer			= m_peerHeader;
	m_tableHeader.titolo		= titolo;
	m_tableHeader.familyName	= familyName;
	m_tableHeader.alias			= alias;
	m_tableHeader.tableRoman	= tableRoman;
	m_tableHeader.joint			= joint;
	m_tableHeader.arm			= arm;
	m_tableHeader.torzs			= torzs;
	m_tableHeader.comment		= comment;
	m_tableHeader.tableHeader	= tableHeader;

	m_tableHeader.csalad.Empty();	// [csalad: xx] felsorol�sa. getBranch teszi bele
	m_tableHeader.folyt.Empty();	// a t�bl�ban l�v� foplyt sorok felsorol�sa (getBranch teszi bele)
	m_tableHeader.ancestor.Empty();	// a t�bla-�s rowid-ja, insertDescendant teszi bele

	m_familyName = familyName;			// sz�ks�g van erre a % n�v v�ltoz�sok k�vet�se miatt
}