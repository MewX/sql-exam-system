/**
 * Name: ODBCManagement.cpp
 * Author: MewCatcher
 * Date: 2014/05/06
 * Description: See header ODBCManagement.h
 **/

#include "StdAfx.h"
#include "ODBCManagement.h"

ODBCManagement::ODBCManagement( )
{
	status = true;
	return;
}

ODBCManagement::~ODBCManagement( )
{
	return;
}

bool ODBCManagement::ODBCConnect( TCHAR* szDSN, TCHAR* szUID, TCHAR* szAuthStr )
{
	if( status ) ODBCDisconnect( );

	ret = SQLAllocHandle( SQL_HANDLE_ENV, SQL_NULL_HANDLE, &henv );
	ret = SQLSetEnvAttr( henv, SQL_ATTR_ODBC_VERSION, (void *)SQL_OV_ODBC3, 0 );
	ret = SQLAllocHandle( SQL_HANDLE_DBC, henv, &hdbc );
	ret = SQLConnect( hdbc, szDSN, SQL_NTS, szUID, SQL_NTS, szAuthStr, SQL_NTS );

	status = SQL_SUCCEEDED( ret );
	return status;
}

void ODBCManagement::ODBCDisconnect( )
{
	if( status ) {
		SQLFreeHandle( SQL_HANDLE_STMT, hstmt );
		SQLDisconnect( hdbc );
		SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
		SQLFreeHandle( SQL_HANDLE_ENV, henv );
		status = false;
	}
	return;
}
