#pragma once


// CConnectCsalad

class CConnectCsalad : public CWnd
{
	DECLARE_DYNAMIC(CConnectCsalad)

public:
	CConnectCsalad();
	virtual ~CConnectCsalad();

	void connectCsalad( BOOL flag );

protected:
	CString m_command;

	DECLARE_MESSAGE_MAP()
};


