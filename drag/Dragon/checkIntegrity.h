#pragma once
#include "ProgressWnd.h"

// CCheckIntegrity

class CCheckIntegrity : public CWnd
{
	DECLARE_DYNAMIC(CCheckIntegrity)

public:
	void integrity();
	CCheckIntegrity();
	virtual ~CCheckIntegrity();

	
protected:
	CProgressWnd wndP; 

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;

	CString str;
	CString m_command;
	FILE* fl;
	bool m_hiba;
	
	BOOL query( CString command );
	BOOL query1( CString command );

	void couples();
	void fathers();
	DECLARE_MESSAGE_MAP()
};


