#pragma once
#include "stdafx.h"
#include "ProgressWnd.h"
#include "uniteStructures.h"


class CUniteEntries : public CWnd
{
	DECLARE_DYNAMIC(CUniteEntries)

public:
	CUniteEntries();
	virtual ~CUniteEntries();

	int m_loopMax;		// maximális iteráció szám
	int m_algorithm;
	CString m_name;
	bool	m_contracted;
	CString R1;
	CString R2;
	bool	m_snameEnough;
	bool    m_term;

	bool CUniteEntries::parameteres();
protected:
	CString str;
	CString m_command;
		
	CString m_fileSpecTextU;
	CString m_fileSpecTextD;
	CString m_fileSpecTextN;
	CString m_filenameTextU;
	CString m_filenameTextD;
	CString m_filenameTextN;

	FILE* textU;
	FILE* textD;
	FILE* textN;

	CSqliteDBRecordSet* m_rset;
	CSqliteDBRecordSet* m_rset1;
	CSqliteDBRecordSet* m_rset2;
	CSqliteDBRecordSet* m_rset3;
	CSqliteDBRecordSet* m_rset4;

	GETNAME3 p;

	std::vector<ENTRIES> vEntries;

	std::vector<LASTFIRST> vLF;
	std::vector<USAME3> vSameNames;
	std::vector<USPOUSES3> vSpouses;
	std::vector<USPOUSES3> vSpouses1;
	std::vector<USPOUSES3> vSpouses2;
	std::vector<OTHERS> vOthers;

	CString m_first_name;

	int B;
	int D;
	int F;
	int FB;
	int FD;
	int M;
	int MB;
	int MD;
	int S;
	int m_match;
	int m_matchP;
	int m_matchS;
	int m_matchC;
	int m_matchDummy;

	// elõrható paraméterek
	

	int nItem;
	int m_loop;
	bool m_modified;
	bool m_deleted;
	bool m_saved;

	bool CUniteEntries::slow();
	bool CUniteEntries::fast();
	BOOL CUniteEntries::push(CString rowid );
	BOOL CUniteEntries::processSameNames();
	BOOL CUniteEntries::getPeople(CString rowid);
	bool CUniteEntries::identical(UINT i, UINT j);

	bool CUniteEntries::saveValues();
	int CUniteEntries::same(CString par1, CString par2, int* match);
	int CUniteEntries::sameSpouses( int i1, int i2);
	int CUniteEntries::oneSpouse(int i1, int i2);
	bool CUniteEntries::uniteSamePeople();
	bool CUniteEntries::deleteEntry(CString rowid1, CString rowid0, CString sex_id);
	void CUniteEntries::deleteFromvEntries(CString rowid);
	bool CUniteEntries::deleteSameMarriages();
	void CUniteEntries::listPeople( FILE* fl);
	void CUniteEntries::listSameNames( FILE* fl, int i);
	void CUniteEntries::emptyLine(FILE* fl);
	void CUniteEntries::whichHW();
	bool CUniteEntries::childNameSame(int i, int j);


	BOOL CUniteEntries::query( CSqliteDBRecordSet* r, CString command);


	DECLARE_MESSAGE_MAP()
public:
	
};


