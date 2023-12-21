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
void CInputGA::splitTableHeader(CString cLine)
{
	CStringArray A;
	int pos;
	int pos1;
	int pos2;
	int i;
	int n;

// tableHeader változók //////////////////////////////////////////////
	
	CString percent;		// %%% vagy %%
	CString familyName;		// a tableHeaddr-bõl kivett családnév
	CString titolo;			// a tableHeaderbõl kivett elõnév, ill. elõtt álló 
	CString alias;			// a tableHeaderbõl kivett alias
	CString tableRoman;		// a tábla római száma a családon belül
	int		joint = 1;			// 0 = nem kapcs
	CString torzs;
	CString arm;
	CString comment;		// a tableHeaderbõl kivett comment
	CString tableHeader;	// a teljes tábla sor
	int z;
////////////////////////////////////////////////////////////////////
	cLine.Replace('\'', '|');	 // a nevekben elõfordulhat az ' jel, amit az SQLite nem szeret
	if ((pos = cLine.Find('<')) != -1)   // az esetleges html formattálás levágása
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

	// szögletes zárójelek tartalmának megõrzése és leszedése a cLine-ról
	if ((pos1 = cLine.ReverseFind('[')) != -1)			// [] zárójelek leszedése
	{
		if ((pos2 = cLine.ReverseFind(']')) != -1)
		{
			if (pos1 < pos2)
			{
				str = cLine.Mid(pos1 + 1, pos2 - pos1 - 1);
				cLine = cLine.Left(pos1 - 1);
				if ((pos1 = str.Find(L"törzs:")) != -1)
					torzs = str.Mid(7);
				else
					arm = str;
			}
		}
	}

	if (percent == "%%")
	{
		titolo = m_tableHeader.titolo;		// a %% táblák öröklik az elõzõ tábla titolo-ját, de ez felülírható

		if ((pos = cLine.Find(L"nem kapcs")) != -1)
		{
			joint = 0;
			str = cLine.Mid(pos);
			str = dropFirstWord(str);
			str = dropFirstWord(str);
			str.Trim();
			if (!str.IsEmpty())
				comment = str;
			cLine = cLine.Left(pos - 1);								// nem kapcs leszedése
		}

		/////////////////// romam meghatárzása ////////////////////////////////////////////////////////////////////
		LPCTSTR valid = L"IVXabcde"; // a római táblaszámban lehetnek betûk!!!
		LPCTSTR validX = L"IVX";
		CString substr;
		int		len;
		TCHAR	kar;
		

		n = splitCString(cLine, ' ', false, &A);      // cLine [] zárójelek, nem kapcs és comment nélkül
		for (i = 0; i < n; ++i)
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
					tableRoman = A[i];
					if (i + 1 > n)  // még van mögötte valami, az comment
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
	// családnév[, elõnév] alias X feldolgozása

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
	else									// %%% Antall (Antal), Kisjenõi
	{
//		str = getWord( cLine, 2, &pos1 );   // név utáni (más nevek),
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

	// elõnév pontosítása

	if ((pos = titolo.Find('(')) != -1)
	{
		if (pos == 0)
			titolo = titolo.Mid(1, titolo.GetLength() - 2); // (Benedicti) kiveszi belõle az elõnevet
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
		if (word == L"és") continue;
		if (word == L"ill.") continue;
		if (iswlower(word.GetAt(0))) break;			// a titolo az elsõ kisbetûig tart vagy a végéig
	}
	titolo = packWords(&A, 0, i);

	if ((pos = titolo.Find(L"ill.")) != -1)
		titolo = titolo.Left(pos);

	if (isLastCharacter(titolo, ','))
		titolo = dropLastCharacter(titolo);
//	if ((pos = titolo.Find('-')) != -1)
//		titolo = titolo.Left(pos);


// tableHeader struktúra kitöltése
	m_tableHeader.tableNumber = m_tableNumber;		// ezek globális counterek
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

	m_tableHeader.csalad.Empty();	// [csalad: xx] felsorolása. getBranch teszi bele
	m_tableHeader.folyt.Empty();	// a táblában lévõ foplyt sorok felsorolása (getBranch teszi bele)
	m_tableHeader.ancestor.Empty();	// a tábla-õs rowid-ja, insertDescendant teszi bele

	m_familyName = familyName;			// szükség van erre a % név változások követése miatt
}