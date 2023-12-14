#pragma once
#include "ProgressWnd.h"

// CFatherMother

class CFatherMother : public CWnd
{
	DECLARE_DYNAMIC(CFatherMother)

public:
	CFatherMother();
	virtual ~CFatherMother();

	void fatherMother();

protected:
	CProgressWnd wndP; 

	CSqliteDBRecordSet*	 m_recordset;
	CSqliteDBRecordSet*	 m_recordset1;

	CString str;
	CString m_command;

	BOOL query( CString command );
	BOOL query1( CString command );
	DECLARE_MESSAGE_MAP()
};


