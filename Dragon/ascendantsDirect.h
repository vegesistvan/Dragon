#pragma once


// CAscendantsDirect

class CAscendantsDirect : public CWnd
{
	DECLARE_DYNAMIC(CAscendantsDirect)

public:
	CAscendantsDirect();
	virtual ~CAscendantsDirect();
	CString m_rowid;
	CString m_name;

	void CAscendantsDirect::directFatherAscendants();
	void CAscendantsDirect::directMotherAscendants();
protected:

	CString str;
	CString m_command;
	CString m_filespec;
	CString m_ulStyle;

	FILE* fh;
	bool	m_bold;
	int		m_code;
	int		m_cnt_ul;
	int		m_cnt;
	CSqliteDBRecordSet m_recordset;

	void CAscendantsDirect::ascendants( CString rowid, int g );
	void CAscendantsDirect::print(CString);
	void CAscendantsDirect::listPeople(CString rowid, UINT g);
	void CAscendantsDirect::printAscendants();
	CString CAscendantsDirect::indent(int gen, int genPrev);
	bool CAscendantsDirect::openHtml(CString file, CString title);
	CString CAscendantsDirect::getPeopleString(CString rowid);
	BOOL CAscendantsDirect::query(CString command);


	DECLARE_MESSAGE_MAP()
};


