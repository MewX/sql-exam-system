/**
 * Name: TableAdmin.cpp
 * Author: MewCatcher
 * Date: 2014/05/09
 * Description: see TableAdmin.h
 **/

#include "stdafx.h"
#include "TableAdmin.h"


TableAdmin::TableAdmin( )
{
	hasInit = false;
	SA = NULL;
	return;
}


TableAdmin::~TableAdmin( )
{
	if( SA != NULL ) delete [ ] SA;
	return;
}

bool TableAdmin::Init( SQLHDBC hdbc_loc, string Name, string Password )
{
	hasInit = false;

	SQLHSTMT h;
	memset( AdminName, '\0', 256 );
	strcpy_s( PasswordHash, 255, Password.c_str( ) );

	SQLAllocHandle( SQL_HANDLE_STMT, hdbc_loc, &h );
	SQLExecDirectA( h, (SQLCHAR *)( "select AdminId, AdminName, canManageAdmin, canManageStudent, "
		"canSetProblem, canSetPaper, canSetExam, canDeleteGrade, canEditGrade, AdminType from Admin "
		"where AdminName = \'" + Name + "\' and AdminPassword = \'" + Password + "\';" ).c_str( ), SQL_NTS );
	if( SQL_SUCCEEDED( SQLFetch( h ) ) ) {
		if( !( SQL_SUCCEEDED( SQLGetData( h, 1, SQL_C_LONG, &AdminId, 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 2, SQL_C_CHAR, AdminName, 20, NULL ) ) // Last 2 cannot be NULL together
				&& SQL_SUCCEEDED( SQLGetData( h, 3, SQL_C_BIT, &canManageAdmin, 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 4, SQL_C_BIT, &canManageStudent, 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 5, SQL_C_BIT, &canSetProblem, 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 6, SQL_C_BIT, &canSetPaper, 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 7, SQL_C_BIT, &canSetExam, 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 8, SQL_C_BIT, &canDeleteGrade, 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 9, SQL_C_BIT, &canEditGrade, 0, NULL ) ) 
				&& SQL_SUCCEEDED( SQLGetData( h, 10, SQL_C_BIT, &AdminType, 0, NULL ) ) ) ) {
			MessageBox( NULL, _T( "tAdmin GetData Failed!" ), _T( "Error" ), MB_OK );
			return false;
		}
		/* else {
			char temp[ 1024 ];
			sprintf( temp, "AdminId: %d\r\nAdminName: %s\r\ncanManageAdmin: %d\r\n"
				"canManageStudent: %d\r\ncanSetProblem: %d\r\ncanSetPaper: %d\r\ncanSetExam: %d\r\n"
				"canDeleteGrade: %d\r\ncanEditGrade: %d\r\nAdminType: %d", AdminId, AdminName, (int)canManageAdmin,
				(int)canManageStudent, (int)canSetProblem, (int)canSetPaper, (int)canSetExam,
				(int)canDeleteGrade, (int)canEditGrade, (int)AdminType );
			MessageBoxA( NULL, temp, "Succeeded", MB_OK );
		} */
	}
	else {
		MessageBox( NULL, _T( "tAdmin Fetch Failed!\r\nIn Init( );" ), _T( "Error" ), MB_OK );
		return false;
	}

	SQLFreeHandle( SQL_HANDLE_STMT, h );
	hasInit = true;
	return true;
}


void TableAdmin::Delete( int line, SQLHDBC hdbc_loc )
{
	if( !( hasInit && line < SA_count ) ) return;

	// delete one record
	SQLHSTMT h;
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc_loc, &h );
	SQLExecDirectA( h, (SQLCHAR *)( (string)"delete from Admin where AdminName = \'" + SA[ line ].Name + "\';" ).c_str( ), SQL_NTS );
	SQLFreeHandle( SQL_HANDLE_STMT, h );

	delete [ ] SA;
	Init( hdbc_loc, AdminName, PasswordHash );

	return;
}

void TableAdmin::Add( StructAdmin StrAdmin, ODBCManagement& ODBC, SQLHDBC hdbc_loc )
{
	if( !hasInit ) return;

	// add one record
	int MaxId = ODBC.getOneValue( "select max( AdminId ) from Admin;" ) + 1;
	char temp[ 1024 ];

	sprintf( temp, "insert into Admin( AdminId, AdminName, AdminPassword, AdminType, "
                   "canManageAdmin, canManageStudent, canSetProblem, canSetPaper, canSetExam, canDeleteGrade, canEditGrade ) "
					"values ( %d, \'%s\', \'be9d9be4e8df0f4046100680ff3b209c15715e6c9f1057ca945c08cfc77b2a50\', "
					"1, %d, %d, %d, %d, %d, %d, %d );", MaxId, StrAdmin.Name, StrAdmin.canManageAdmin ? 1 : 0,
					StrAdmin.canManageStudent ? 1 : 0, StrAdmin.canSetProblem ? 1 : 0, StrAdmin.canSetPaper ? 1 : 0,
					StrAdmin.canSetExam ? 1 : 0, StrAdmin.canDeleteGrade ? 1 : 0, StrAdmin.canEditGrade ? 1 : 0 );

	SQLHSTMT h;
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc_loc, &h );
	SQLExecDirectA( h, (SQLCHAR *)temp, SQL_NTS );
	SQLFreeHandle( SQL_HANDLE_STMT, h );

	delete [ ] SA;
	Init( hdbc_loc, AdminName, PasswordHash );

	return;
}

void TableAdmin::Edit( int line, StructAdmin StrAdmin, SQLHDBC hdbc_loc )
{
	if( !( hasInit && line < SA_count ) ) return;

	// edit a record
	char temp[ 1024 ];
	sprintf( temp, "update Admin set canManageAdmin = %d, canManageStudent = %d, canSetProblem = %d, canSetPaper = %d, "
				   "canSetExam = %d, canDeleteGrade = %d, canEditGrade = %d where AdminName = \'%s\';",
				   StrAdmin.canManageAdmin ? 1 : 0, StrAdmin.canManageStudent ? 1 : 0, StrAdmin.canSetProblem ? 1 : 0,
				   StrAdmin.canSetPaper ? 1 : 0, StrAdmin.canSetExam ? 1 : 0, StrAdmin.canDeleteGrade ? 1 : 0,
				   StrAdmin.canEditGrade ? 1 : 0,  StrAdmin.Name );

	SQLHSTMT h;
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc_loc, &h );
	SQLExecDirectA( h, (SQLCHAR *)temp, SQL_NTS );
	SQLFreeHandle( SQL_HANDLE_STMT, h );

	delete [ ] SA;
	Init( hdbc_loc, AdminName, PasswordHash );
	return;
}


string TableAdmin::get_UserName( ) const
{
	return hasInit ? AdminName : "";
}

int TableAdmin::get_AdminId( ) const
{
	return hasInit ? AdminId : 0;
}

bool TableAdmin::get_canManageAdmin( ) const
{
	return hasInit ? canManageAdmin : false;
}

bool TableAdmin::get_canManageStudent( ) const
{
	return hasInit ? canManageStudent : false;
}

bool TableAdmin::get_canSetProblem( ) const
{
	return hasInit ? canSetProblem : false;
}

bool TableAdmin::get_canSetPaper( ) const
{
	return hasInit ? canSetPaper : false;
}

bool TableAdmin::get_canSetExam( ) const
{
	return hasInit ? canSetExam : false;
}

bool TableAdmin::get_canDeleteGrade( ) const
{
	return hasInit ? canDeleteGrade : false;
}

bool TableAdmin::get_canEditGrade( ) const
{
	return hasInit ? canEditGrade : false;
}

bool TableAdmin::FetchAdminList( ODBCManagement& ODBC, SQLHDBC hdbc_loc )
{
	if( !hasInit ) return false;

	SA_count = ODBC.getOneValue( (string)"select count( * ) from Admin where AdminName != \'"
		+ AdminName +"\' and AdminType != 0;" ); // I only worked out this method

	if( !SA_count ) goto SAEND;
	else {
		SA = new StructAdmin [ SA_count ];
		memset( SA, '\0', sizeof( SA ) );
	}

	SQLHSTMT h;
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc_loc, &h );
	SQLExecDirectA( h, (SQLCHAR *)( (string)"select AdminName, canManageAdmin, canManageStudent, canSetProblem, "
		+ "canSetPaper, canSetExam, canDeleteGrade, canEditGrade from Admin_General where AdminName != \'"
		+ AdminName +"\' and AdminType != 0;" ).c_str( ), SQL_NTS );

	int i = 0;
	while( SQL_SUCCEEDED( SQLFetch( h ) ) ) {
		if( !( SQL_SUCCEEDED( SQLGetData( h, 1, SQL_C_CHAR, SA[ i ].Name, 20, NULL ) ) // Last 2 cannot be NULL together
				&& SQL_SUCCEEDED( SQLGetData( h, 2, SQL_C_BIT, &(SA[ i ].canManageAdmin), 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 3, SQL_C_BIT, &(SA[ i ].canManageStudent), 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 4, SQL_C_BIT, &(SA[ i ].canSetProblem), 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 5, SQL_C_BIT, &(SA[ i ].canSetPaper), 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 6, SQL_C_BIT, &(SA[ i ].canSetExam), 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 7, SQL_C_BIT, &(SA[ i ].canDeleteGrade), 0, NULL ) )
				&& SQL_SUCCEEDED( SQLGetData( h, 8, SQL_C_BIT, &(SA[ i ].canEditGrade), 0, NULL ) ) ) ) {
			MessageBox( NULL, _T( "tAdmin GetData Failed!\r\nIn FetchAdminList( );" ), _T( "Error" ), MB_OK );
			return false;
		}
		else {
			/*char temp[ 1024 ];
			sprintf( temp, "AdminName: %s\r\ncanManageAdmin: %d\r\n"
				"canManageStudent: %d\r\ncanSetProblem: %d\r\ncanSetPaper: %d\r\ncanSetExam: %d\r\n"
				"canDeleteGrade: %d\r\ncanEditGrade: %d", SA[ i ].Name, (int)(SA[ i ].canManageAdmin),
				(int)(SA[ i ].canManageStudent), (int)(SA[ i ].canSetProblem), (int)(SA[ i ].canSetPaper),
				(int)(SA[ i ].canSetExam),(int)(SA[ i ].canDeleteGrade), (int)(SA[ i ].canEditGrade) );
			MessageBoxA( NULL, temp, "Succeeded", MB_OK ); */

			i ++;
		}
	}
	SQLFreeHandle( SQL_HANDLE_STMT, h );

SAEND:
	return true;
}

int TableAdmin::get_SA_count( ) const
{
	return SA_count;
}

StructAdmin TableAdmin::get_SA( int line ) const
{
	if( 0 <= line && line < SA_count ) return SA[ line ];
	else {
		StructAdmin s;
		memset( &s, '\0', sizeof( s ) );
		return s;
	}
}

bool TableAdmin::JudgePassword( string OldHash ) const // true - match
{
	if( !hasInit ) return false;
	else return OldHash == PasswordHash;
}

bool TableAdmin::SetNewPassword( SQLHDBC hdbc_loc, string OldHash, string NewHash )
{
	if( !JudgePassword( OldHash ) ) return false;
	
	SQLHSTMT h;
	strcpy( PasswordHash, NewHash.c_str( ) );
	SQLAllocHandle( SQL_HANDLE_STMT, hdbc_loc, &h );
	// update Admin set AdminPassword = \' ... \' where AdminName = \' ... \';
	SQLExecDirectA( h, (SQLCHAR *)( (string)"update Admin set AdminPassword = \'" + PasswordHash
		+ "\' where AdminName = \'" + AdminName + "\';" ).c_str( ), SQL_NTS );
	SQLFreeHandle( SQL_HANDLE_STMT, h );

	return true;
}
