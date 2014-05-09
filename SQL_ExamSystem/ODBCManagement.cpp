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

bool ODBCManagement::ODBCConnect( TCHAR *szDSN, TCHAR *szUID, TCHAR *szAuthStr )
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
		SQLFreeHandle( SQL_HANDLE_STMT, hstmt ); // free statement handle
		SQLDisconnect( hdbc );
		SQLFreeHandle( SQL_HANDLE_DBC, hdbc );
		SQLFreeHandle( SQL_HANDLE_ENV, henv );
		status = false;
	}
	return;
}

bool ODBCManagement::ODBCExecDirect( string Query )
{
	SQLHSTMT h;
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &h );
	ret = SQLExecDirectA( h, (SQLCHAR *)Query.c_str( ), SQL_NTS );
	SQLFreeHandle( SQL_HANDLE_STMT, h );
	MessageBoxA( NULL, Query.c_str( ), NULL, NULL );

	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

bool ODBCManagement::ODBCExecDirect( wstring Query )
{
	SQLHSTMT h;
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &h );
	ret = SQLExecDirectW( h, (SQLWCHAR *)Query.c_str( ), SQL_NTS );
	SQLFreeHandle( SQL_HANDLE_STMT, h );
	MessageBoxW( NULL, Query.c_str( ), NULL, NULL );

	return ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO;
}

SQLHDBC ODBCManagement::getHdbc( ) const
{
	return hdbc;
}

long ODBCManagement::getOneValue( string str )
{
	wstring wstr( str.begin( ), str.end( ) );
	return getOneValue( wstr );
}

long ODBCManagement::getOneValue( wstring wstr )
{
	SQLHSTMT h;
	long test = -1;

	SQLAllocHandle( SQL_HANDLE_STMT, hdbc, &h );
	SQLExecDirectW( h, (SQLWCHAR *)wstr.c_str( ), SQL_NTS );
	if( SQL_SUCCEEDED( SQLFetch( h ) ) ) {
		if( !SQL_SUCCEEDED( SQLGetData( h, 1, SQL_C_LONG, &test, 0, NULL ) ) )
			MessageBox( NULL, _T("SQLGetData Error 0x0000!"), NULL ,NULL );
	}
	else {
		MessageBoxW( NULL, ( L"SQLFetch Error 0x0000!\r\nError in querying?\r\n-> " + wstr ).c_str( ), NULL ,NULL );
	}
	SQLFreeHandle( SQL_HANDLE_STMT, h );
	return test;
}

