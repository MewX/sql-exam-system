/**
 * Name: ODBCManagement.h
 * Author: MewCatcher
 * Date: 2014/05/06
 * Description: This file define the class - ODBCManagement.
 *              This class process all the request to the ODBC,
 *              playing the role of management of this project.
 **/

#ifndef _ODBCMANAGEMENT_H_
#define _ODBCMANAGEMENT_H_

class ODBCManagement
{
public:
	ODBCManagement( );
	~ODBCManagement( );

	SQLRETURN ODBCConnect( TCHAR* szDSN, TCHAR* szUID, TCHAR* szAuthStr );
	void ODBCDisconnect( );

private:
	bool status;    // true - connected
	SQLHENV  henv;  // Environment handle
	SQLHDBC  hdbc;  // Connector handle
	SQLHSTMT hstmt; // Operation handle
	SQLRETURN ret;  // return value

};

#endif
