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

	bool ODBCConnect( TCHAR *szDSN, TCHAR *szUID, TCHAR *szAuthStr );
	bool ODBCExecDirect( string Query );
	bool ODBCExecDirect( wstring Query );
	void ODBCDisconnect( );

	SQLHSTMT hstmt; // Operation handle
	SQLHDBC getHdbc( ) const;
	long getOneValue( string str );  // Exec and get table(1,1) value
	long getOneValue( wstring wstr );

private:
	bool status;    // true - connected
	SQLHENV  henv;  // Environment handle
	SQLHDBC  hdbc;  // Connector handle
	SQLRETURN ret;  // return value

};

#endif
