/**
 * Name: TableStudent.h
 * Author: MewCatcher
 * Date: 2014/05/15
 * Description: This class get the info from daatabase, and manage the info on this client
 **/

#ifndef _TABLESTUDENT_H_
#define _TABLESTUDENT_H_

typedef struct {
	char Name[ 256 ];
	int ExamTime;
} StructStudent;

class TableStudent
{
public:
	TableStudent( );
	~TableStudent( );

	bool Init( ODBCManagement& ODBC, SQLHDBC hdbc_loc );
	int get_SS_count( ) const;
	StructStudent get_SS( int line ) const;

private:
	bool hasInit;
	StructStudent *SS;
	int SS_count;

};

#endif
