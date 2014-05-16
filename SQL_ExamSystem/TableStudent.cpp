/**
 * Name: TableStudent.cpp
 * Author: MewCatcher
 * Date: 2014/05/15
 * Description: TableStudent.h
 **/

#include "stdafx.h"
#include "TableStudent.h"


TableStudent::TableStudent( )
{
	hasInit = false;
	SS = NULL;
	return;
}


TableStudent::~TableStudent( )
{
	delete [ ] SS;
	return;
}

bool TableStudent::Init( ODBCManagement& ODBC, SQLHDBC hdbc_loc )
{
	hasInit = false;

	SS_count = ODBC.getOneValue( "select count( * ) from Student_General;" );
	if( !SS_count ) goto SSEND;
	else {
		SS = new StructStudent [ SS_count ];
		memset( SS, '\0', sizeof( SS ) );
	}

	SQLHSTMT h;
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc_loc, &h );
	SQLExecDirectA( h, (SQLCHAR *)"select StuName, ExamTime from Student_General;", SQL_NTS );

	int i = 0;
	while( SQL_SUCCEEDED( SQLFetch( h ) ) ) {
		if( !( SQL_SUCCEEDED( SQLGetData( h, 1, SQL_C_CHAR, SS[ i ].Name, 20, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 2, SQL_C_LONG, &(SS[ i ].ExamTime), 0, NULL ) ) ) ) {
			MessageBox( NULL, _T( "tStudent GetData Failed!\r\nIn TableStudent::Init( );" ), _T( "Error" ), MB_OK );
			return false;
		}
		else i ++;
	}
	SQLFreeHandle( SQL_HANDLE_STMT, h );

SSEND:
	return true;
}

int TableStudent::get_SS_count( ) const
{
	return SS_count;
}

StructStudent TableStudent::get_SS( int line ) const
{
	if( 0 <= line && line < SS_count ) return SS[ line ];
	else {
		StructStudent s;
		memset( &s, '\0', sizeof( s ) );
		return s;
	}
}
