#pragma once


// CCheckBefore

class CCheckBefore : public CWnd
{
	DECLARE_DYNAMIC(CCheckBefore)
	DECLARE_MESSAGE_MAP()
public:
	CCheckBefore();
	virtual ~CCheckBefore();

	void CCheckBefore::checkFolyt();
	void CCheckBefore::repeatedFamilies();

protected:
	CString m_command;
	CString str;
	FILE* fl;
};


