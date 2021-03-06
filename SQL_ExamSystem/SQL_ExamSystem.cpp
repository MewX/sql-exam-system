﻿/**
 * Name: SQL_ExamSystem.cpp
 * Author: MewCatcher
 * Date: 2014/05/05
 * Description: This file contains all the UI proc funtions.
 **/

#include "stdafx.h"
#include "SQL_ExamSystem.h"

#define MAX_LOADSTRING 100

// 全局变量:
HINSTANCE hInst;								// 当前实例
int ProcedureId;
/*TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);*/

// My functions
void ResetListViewData( HWND h );

// UI Procs
INT_PTR CALLBACK	DlgTemplateProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgAboutProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgLoginProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgRegProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgAdminShellProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgAdminPwProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
	/*UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// 初始化全局字符串
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_SQL_EXAMSYSTEM, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);*/
	hInst = hInstance;

	// Main Window
BEGIN:
	InitCommonControls( );
	// Login Dialog
	ProcedureId = 1;
	DialogBox( hInst, (LPCTSTR)IDD_DLG_LOGIN, NULL, (DLGPROC)DlgLoginProc );

	if( ProcedureId == 2 ) { // Admin Shell Dialog
		
		DialogBox( hInst, (LPCTSTR)IDD_DLG_ADMSHELL, NULL, (DLGPROC)DlgAdminShellProc );
		goto BEGIN;
	}
	if( ProcedureId == 3 ) { // Student Shell Dialog
		MessageBox( NULL, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );;
		goto BEGIN;
	}

	ODBCM.ODBCDisconnect( );
	return 0;
}



//
//  函数: MyRegisterClass()
//
//  目的: 注册窗口类。
//
//  注释:
//
//    仅当希望
//    此代码与添加到 Windows 95 中的“RegisterClassEx”
//    函数之前的 Win32 系统兼容时，才需要此函数及其用法。调用此函数十分重要，
//    这样应用程序就可以获得关联的
//    “格式正确的”小图标。
//
/*ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style			= CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc	= NULL;
	wcex.cbClsExtra		= 0;
	wcex.cbWndExtra		= 0;
	wcex.hInstance		= hInstance;
	wcex.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SQL_EXAMSYSTEM));
	wcex.hCursor		= LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground	= (HBRUSH)(COLOR_WINDOW+1);
	wcex.lpszMenuName	= NULL;
	wcex.lpszClassName	= szWindowClass;
	wcex.hIconSm		= LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
}*/

/****************************************
 *  Tool Functions
 ****************************************/
void ResetListViewData( HWND h ) // handle of ListView
{
	int nCols = 0;
	HWND hWndHeader;
	LVITEM lvItem;
	
	SendMessage( h, LVM_DELETEALLITEMS, 0, 0 );
	
	hWndHeader = (HWND)SendMessage( h, LVM_GETHEADER, 0,0 ); // get column count
	nCols = SendMessage( hWndHeader, HDM_GETITEMCOUNT, 0, 0 );
	
	//删除ListView所有列
	while( nCols ) {
		SendMessage( h, LVM_DELETECOLUMN, 0, (LPARAM)&lvItem );
		nCols--;
	}
	return;
}

void MoveWindowToClientCentre( HWND h )
{
	// reset window position
	int ScreenWidth, ScreenHeight;
	RECT rc = { 0 };
	ScreenWidth = GetSystemMetrics( SM_CXSCREEN );
	ScreenHeight = GetSystemMetrics( SM_CYSCREEN );
	GetWindowRect( h, &rc );
	SetWindowPos( h, HWND_NOTOPMOST, ( ScreenWidth - ( rc.right - rc.left ) ) / 2,
		( ScreenHeight - ( rc.bottom - rc.top ) ) / 2,
		( rc.right - rc.left ), ( rc.bottom - rc.top ), SWP_SHOWWINDOW );
	return;
}

/****************************************
 *  All the dialog procs' main function
 ****************************************/
INT_PTR CALLBACK DlgTemplateProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		MoveWindowToClientCentre( hDlg );
		
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgLoginProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int ConnectionInfo = 0; // 0 - Not Connected; -1 - Connection failed; 1 - Connection succeeded.
	static char UserName[ 256 ] = { '\0' }, Password[ 256 ] = { '\0' }, temp[ 256 ] = { '\0' }, temp2[ 256 ] = { '\0' };
	static HDC hdc;
	static const RECT tempRECT = { 0, 0, 600, 600 };


	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		// Here Connect SQL Database
		

RETRY_CONNECT:
		if( !ODBCM.ODBCConnect( _T( "MewCatcherSQLServer" ), _T(""), _T("") ) ) { // test connection info
			ConnectionInfo = -1;
			SetWindowText( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ), _T( "Failed to connect.  :(" ) );
			EnableWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_BUTTON_LOGIN ), FALSE );
			EnableWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_BUTTON_REGISTER ), FALSE );
			EnableWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_BUTTON_RETRY ), TRUE );
		}
		else {
			ConnectionInfo = 1;
			SetWindowText( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ), _T( "Connected!  :)" ) );
			EnableWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_BUTTON_LOGIN ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_BUTTON_REGISTER ), TRUE );
			EnableWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_BUTTON_RETRY ), FALSE );
		}

		return (INT_PTR)TRUE;

	case WM_CTLCOLORSTATIC: // Set Text Color, and its background
		// Need refresh !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		// SendMessage( hDlg, WM_PAINT, (WPARAM)GetDC( hDlg ), NULL );
		// Invalidate( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) );
		// SetBkColor( GetDC( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) ), GetPixel( GetDC(hDlg), 16, 16 ) );
		// InvalidateRect( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ), &tempRECT, FALSE );
		// UpdateWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) );
		// RedrawWindow( hDlg, &tempRECT, NULL, RDW_ERASE );
		   
		// SendMessage( hDlg, WM_CTLCOLORDLG, (WPARAM)GetDC( hDlg ), NULL );
		// RedrawWindow( hDlg, &tempRECT, NULL, RDW_ERASE | RDW_NOCHILDREN );

		hdc = (HDC)wParam;
		if( (HWND)lParam == GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) ) { // Set Specific Color
			if( ConnectionInfo == 1 ) SetTextColor( (HDC)wParam, RGB( 21, 125, 240 ) );
			else if( ConnectionInfo == -1 ) SetTextColor( (HDC)wParam, RGB( 255, 0, 0 ) );
		}
		SetBkMode( (HDC)wParam, TRANSPARENT ); // Set TRANSPARENT

		// return (LRESULT)GetStockObject( NULL_BRUSH );
		return (LRESULT)CreateSolidBrush(GetPixel( GetDC(hDlg), 16, 16 ) );


	case WM_COMMAND:
		switch( LOWORD(wParam) ) {
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDC_DLG_LOGIN_BUTTON_ABOUT:
			DialogBox( hInst, (LPCTSTR)IDD_DLG_ABOUT, hDlg, (DLGPROC)DlgAboutProc );
			return (INT_PTR)TRUE;

		case IDC_DLG_LOGIN_BUTTON_LOGIN: // This is the kernel part !!!
			memset( UserName, '\0', 256 );
			memset( Password, '\0', 256 );
			memset( temp, '\0', 256 );
			memset( temp2, '\0', 256 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_USERNAME ), UserName, 255 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_PASSWORD ), Password, 255 );

			// test length
			if( strlen( UserName ) > 20 || strlen( UserName ) < 2 ) {
				MessageBox( hDlg, _T( "用户名长度非法！" ), _T( "Error" ), MB_OK );
				return (INT_PTR)TRUE;
			}
			else if( strlen( Password ) > 20 ) {
				MessageBox( hDlg, _T( "密码长度非法！" ), _T( "Error" ), MB_OK );
				return (INT_PTR)TRUE;
			}

			// test legality, avoid SQL injecting
			for( unsigned int i = 0; i < strlen( UserName ); i ++ ) {
				if( 'A' <= UserName[ i ] && UserName[ i ] <= 'Z' ) UserName[ i ] |= 0x20; // to low case
				else if( !( 'a' <= UserName[ i ] && UserName[ i ] <= 'z' || i != 0 && '0' <= UserName[ i ] && UserName[ i ] <= '9' ) ) {
					MessageBox( hDlg, _T( "SQL injecting detected!" ), _T( "Error" ), MB_OK );
					return (INT_PTR)TRUE;
				}
			}
			
			// test UserName, Password matchment
			sha256_32byte( (unsigned char *)Password, (unsigned char *)temp );
			byte_array_to_str( (unsigned char *)temp, temp2, 32 );
			if( ODBCM.getOneValue( (string)"select count( * ) from Admin where AdminName = \'"
									+ UserName + "\' and AdminPassword = \'" + temp2 + "\';" ) == 1 ) {
				// Init tAdmin
				if( tAdmin.Init( ODBCM.getHdbc( ), UserName, temp2 ) ) ProcedureId = 2;
				else {
					MessageBox( hDlg, _T( "Cannot seek Admin info! (Unknow Error)" ), _T( "Error" ), MB_OK );
					return (INT_PTR)TRUE;
				}
			}
			else if( ODBCM.getOneValue( (string)"select count( * ) from Student where StuName = \'"
										+ UserName + "\' and StuPassword = \'" + temp2 + "\';" ) == 1 ) {
				// Init Student
				// if(  )
				ProcedureId = 3;
			}
			else {
				MessageBox( hDlg, _T( "用户名或密码不正确！" ), _T( "Error" ), MB_OK );
				// MessageBoxA( hDlg, temp2, NULL, NULL ); // Show Hash Value
				SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_PASSWORD ), "" );
				return (INT_PTR)TRUE;
			}
			EndDialog(hDlg, 1);

			return (INT_PTR)TRUE;

		case IDC_DLG_LOGIN_BUTTON_REGISTER:
			DialogBox( hInst, (LPCTSTR)IDD_DLG_REG, hDlg, (DLGPROC)DlgRegProc );

		case IDC_DLG_LOGIN_BUTTON_RETRY: // Attemp to reconnct to SQL DB
			goto RETRY_CONNECT; // Using this way is efficient
			return (INT_PTR)TRUE;

		} // switch ID
		return (INT_PTR)TRUE;
	} // switch message
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK DlgAboutProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		MoveWindowToClientCentre( hDlg );
		SetWindowText( GetDlgItem( hDlg, IDC_DLG_ABOUT_EDIT_OS ),
			_T(" OpenSource - https://github.com/MewCatcher/SQL_ExamSystem/") );
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


INT_PTR CALLBACK DlgRegProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static char UserName[ 256 ] = { '\0' }, Password1[ 256 ] = { '\0' }, Password2[ 256 ] = { '\0' }, temp[ 256 ] = { '\0' };

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		MoveWindowToClientCentre( hDlg );
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if( LOWORD(wParam) == IDCANCEL ) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if( LOWORD(wParam) == IDC_DLG_REG_BTN_REG )
		{
			// Here process the reg logic
			memset( UserName, '\0', 256 );
			memset( Password1, '\0', 256 );
			memset( Password2, '\0', 256 );
			memset( temp, '\0', 256 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_USERNAME ), UserName, 255 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD1 ), Password1, 255 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD2 ), Password2, 255 );

			// test enmpty
			if( strlen( UserName ) < 4 || strlen( UserName ) > 20 ) {
				MessageBox( hDlg, _T( "UserName长度非法！" ), _T( "Error" ), MB_OK );
				return (INT_PTR)TRUE;
			}
			
			// test case able
			for( unsigned int i = 0; i < strlen( UserName ); i ++ ) {
				if( 'A' <= UserName[ i ] && UserName[ i ] <= 'Z' ) UserName[ i ] |= 0x20; // to low case
				else if( !( 'a' <= UserName[ i ] && UserName[ i ] <= 'z' || i != 0 && '0' <= UserName[ i ] && UserName[ i ] <= '9' ) ) {
					MessageBox( hDlg, _T( "UserName内容错误！" ), _T( "Error" ), MB_OK );
					return (INT_PTR)TRUE;
				}
			}
			if( strcmp( Password1, Password2 ) ) {
				MessageBox( hDlg, _T( "两次输入的密码不匹配，请重新输入！" ), _T( "Error" ), MB_OK );
				SetWindowText( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD1 ), _T( "" ) );
				SetWindowText( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD2 ), _T( "" ) );
				return (INT_PTR)TRUE;
			}
			for( unsigned int i = 0; i < strlen( Password1 ); i ++ ) {
				if( 'A' <= Password1[ i ] && Password1[ i ] <= 'Z' ) Password1[ i ] |= 0x20; // to low case
				else if( !( 'a' <= Password1[ i ] && Password1[ i ] <= 'z' || '0' <= Password1[ i ] && Password1[ i ] <= '9' ) ) {
					MessageBox( hDlg, _T( "Password含有非法字符！" ), _T( "Error" ), MB_OK );
					SetWindowText( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD1 ), _T( "" ) );
					SetWindowText( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD2 ), _T( "" ) );
					return (INT_PTR)TRUE;
				}
			}
			if( strlen( Password1 ) > 20 ) {
				MessageBox( hDlg, _T( "Password长度非法！" ), _T( "Error" ), MB_OK );
				return (INT_PTR)TRUE;
			}
			
			// Test repeated in database Admin
			if( ODBCM.getOneValue( (string)"select count( * ) from Admin where AdminName = \'" + UserName + "\';" ) == 1 ) {
				MessageBox( hDlg, _T( "已存在同名管理员，请换一个UserName！" ), NULL ,NULL );
				return (INT_PTR)TRUE;
			}

			// Test repeated in database Student
			if( ODBCM.getOneValue( (string)"select count( * ) from Student where StuName = \'" + UserName + "\';" ) == 1 ) {
				MessageBox( hDlg, _T( "已存在同名考生，请换一个UserName！" ), NULL ,NULL );
				return (INT_PTR)TRUE;
			}

			// Okay, can be registered
			long IdMax = ODBCM.getOneValue( "select count( * ) from Student;" );
			if( IdMax == 0 ) IdMax = 1;
			else IdMax = ODBCM.getOneValue( "select max( StuId ) from Student;" ) + 1;

			string tempId;      // All of these are just for fun XD, use sprintf is better
			wstring wtempId;
			stringstream oss;
			wstringstream woss;
			oss.clear( );
			woss.clear( );
			oss << IdMax;
			woss << IdMax;
			oss >> tempId;
			woss >> wtempId;

			sha256_32byte( (unsigned char *)Password1, (unsigned char *)Password2 );
			byte_array_to_str( (unsigned char *)Password2, temp, 32 );

			if( !ODBCM.ODBCExecDirect( "insert into Student( StuId, StuName, StuPassword ) "
									   "values ( " + tempId + ", \'" + UserName + "\', \'" + temp + "\' );" ) ) {
				MessageBox( hDlg, _T( "注册失败 0x0004！" ), NULL ,NULL );
				return (INT_PTR)TRUE;
			}
			else MessageBoxW( hDlg, ((wstring)L"注册成功！\r\n您是第" + wtempId +
				L"位注册的考生！\r\n祝您使用愉快！~" ).c_str( ), L"注册完成",NULL );

			// End Dialog
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

			/*The SQLBindCol strategy:
			SQLExecDirect(...,"SELECT ...",...);
			SQLBindCol(...,1,&var_1,...);
			SQLBindCol(...,2,&var_2,...);
			for (;;) {
				if (SQLFetch(...)==100) break; }
				SQLGetData(...,2,...,&var_2,...); }

			The SQLGetData strategy:
			SQLExecDirect(...,"SELECT ...",...);
			for (;;) { 
			   if (SQLFetch(...)==100) break;
			   SQLGetData(...,1,...,&var_1,...);*/
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgAdminShellProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	/**
	 *  0 - None
	 *  1 - IDC_DLG_ADMSHELL_BTN_EDITADM
	 *  2 - IDC_DLG_ADMSHELL_BTN_EDITSTU
	 *  3 - IDC_DLG_ADMSHELL_BTN_EDITQUEST
	 *  4 - IDC_DLG_ADMSHELL_BTN_EDITPAPER
	 *  5 - IDC_DLG_ADMSHELL_BTN_EDITEXAM
	 *  6 - IDC_DLG_ADMSHELL_BTN_EDITGRADE
	 **/
	static int CurrentTable; // 0 - None; 1~? Other tables

	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		MoveWindowToClientCentre( hDlg );

		// Set button usable
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDITADM ), tAdmin.get_canManageAdmin( ) );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDITSTU ), tAdmin.get_canManageStudent( ) );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDITQUEST ), tAdmin.get_canSetProblem( ) );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDITPAPER ), tAdmin.get_canSetPaper( ) );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDITEXAM ), tAdmin.get_canSetExam( ) );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDITGRADE ),
			tAdmin.get_canDeleteGrade( ) || tAdmin.get_canEditGrade( ) );
		
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_DEL ), false );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_ADD ), false );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDIT ), false );
		EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), false );

		SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_STA_UNM ),
			( "Logged as: " + tAdmin.get_UserName( ) ).c_str( ) );
		SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_STA_TAB ), "Current table: ( None )" );
		CurrentTable = 0;

		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch( LOWORD(wParam) ) {
		case IDCANCEL: case IDC_DLG_ADMSHELL_BTN_EXIT:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDITINFO:
			// Edit self infomation
			DialogBox( hInst, (LPCTSTR)IDD_DLG_ADMINPW, hDlg, (DLGPROC)DlgAdminPwProc );
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDITADM: {
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), true );

				REFRESH_ADM_SHOW:
				// Show TableAdmin : AdminName, canManageAdmin, canManageStudent, canSetProblem,
				//                   canSetPaper, canSetExam, canDeleteGrade, canEditGrade

				// Empty table
				ResetListViewData( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ) );

				// Set columns
				LV_COLUMN lvc;

				lvc.mask = LVCF_TEXT | LVCF_WIDTH;
				lvc.pszText = L"管理员用户名";
				lvc.cx = 100;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 0, (long)&lvc );

				lvc.pszText = L"能否管理管理员";
				lvc.cx = 100;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 1, (long)&lvc );

				lvc.pszText = L"能否管理考生";
				lvc.cx = 90;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 2, (long)&lvc );

				lvc.pszText = L"能否设置考题";
				lvc.cx = 90;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 3, (long)&lvc );
				
				lvc.pszText = L"能否设置考卷";
				lvc.cx = 90;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 4, (long)&lvc );
				
				lvc.pszText = L"能否设置考试";
				lvc.cx = 90;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 5, (long)&lvc );

				lvc.pszText = L"能否删除成绩";
				lvc.cx = 90;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 6, (long)&lvc );
				
				lvc.pszText = L"能否修改成绩";
				lvc.cx = 90;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 7, (long)&lvc );

				// Fetch data
				if( !tAdmin.FetchAdminList( ODBCM, ODBCM.getHdbc( ) ) ) {
					MessageBox( hDlg, _T( "Error when Fetching AdminList!" ), _T( "Sorry" ), MB_OK );
					return (INT_PTR)TRUE;
				}

				// Add data to ListView
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_DEL ), true );
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_ADD ), true );
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDIT ), true );

				SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_STA_UNM ),
					( "Logged as: " + tAdmin.get_UserName( ) ).c_str( ) );
				SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_STA_TAB ), "Current table: Admin" );

				// Add items
				LVITEMA lvi;
				lvi.mask = LVIF_TEXT;   // Text Style
                lvi.cchTextMax = 256; // Max size of test
                
				for( int i = 0; i < tAdmin.get_SA_count( ); i ++ ) {
					StructAdmin s = tAdmin.get_SA( i );

					lvi.iItem = i;          // choose item
					lvi.iSubItem = 0;       // Put in first coluom
					lvi.pszText = s.Name;   // Text to display (can be from a char variable) (Items)
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTITEMA, i, (LPARAM)&lvi);

					lvi.iSubItem ++;       // Put in next coluom
					lvi.pszText = ( s.canManageAdmin ? "o" : "x" );
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
					
					lvi.iSubItem ++;       // Put in next coluom
					lvi.pszText = ( s.canManageStudent ? "o" : "x" );
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
					
					lvi.iSubItem ++;       // Put in next coluom
					lvi.pszText = ( s.canSetProblem ? "o" : "x" );
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
					
					lvi.iSubItem ++;       // Put in next coluom
					lvi.pszText = ( s.canSetPaper ? "o" : "x" );
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
					
					lvi.iSubItem ++;       // Put in next coluom
					lvi.pszText = ( s.canSetExam ? "o" : "x" );
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
					
					lvi.iSubItem ++;       // Put in next coluom
					lvi.pszText = ( s.canDeleteGrade ? "o" : "x" );
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
					
					lvi.iSubItem ++;       // Put in next coluom // 7
					lvi.pszText = ( s.canEditGrade ? "o" : "x" );
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
				}

				CurrentTable = 1;
			}
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDITSTU: {
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), true );

				REFRESH_STU_SHOW:
				// Show TableStudent : StudentName, ExamTime

				// Empty table
				ResetListViewData( GetDlgItem( hDlg,IDC_DLG_ADMSHELL_LIST ) );

				// Set columns
				LV_COLUMN lvc;

				lvc.mask = LVCF_TEXT | LVCF_WIDTH;
				lvc.pszText = L"学生用户名";
				lvc.cx = 100;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 0, (long)&lvc );
				
				lvc.pszText = L"参加考试次数";
				lvc.cx = 100;
				SendMessageW( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTCOLUMNW, 1, (long)&lvc );

				// Fetch data
				if( !tStudent.Init( ODBCM, ODBCM.getHdbc( ) ) ) {
					MessageBox( hDlg, _T( "Error when Fetching StudentList!" ), _T( "Sorry" ), MB_OK );
					return (INT_PTR)TRUE;
				}

				// Add data to ListView
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_DEL ), true );
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_ADD ), true );
				EnableWindow( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_BTN_EDIT ), true );

				SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_STA_UNM ),
					( "Logged as: " + tAdmin.get_UserName( ) ).c_str( ) );
				SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_STA_TAB ), "Current table: Student" );

				// Add items
				LVITEMA lvi;
				lvi.mask = LVIF_TEXT;   // Text Style
                lvi.cchTextMax = 256; // Max size of test

				for( int i = 0; i < tStudent.get_SS_count( ); i ++ ) {
					StructStudent s = tStudent.get_SS( i );

					lvi.iItem = i;          // choose item
					lvi.iSubItem = 0;       // Put in first coluom
					lvi.pszText = s.Name;   // Text to display (can be from a char variable) (Items)
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_INSERTITEMA, i, (LPARAM)&lvi);

					lvi.iSubItem ++;       // Put in next coluom
					char ExamTime[ 16 ] = { '\0' };
					sprintf_s( ExamTime, 15, "%d", s.ExamTime );
					lvi.pszText = ExamTime;
					SendMessageA( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ), LVM_SETITEMA, i, (LPARAM)&lvi);
				}

				CurrentTable = 2;
			}
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDITQUEST:
			// Edit self infomation
			MessageBox( hDlg, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDITPAPER:
			// Edit self infomation
			MessageBox( hDlg, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDITEXAM:
			// Edit self infomation
			MessageBox( hDlg, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDITGRADE:
			// Edit self infomation
			MessageBox( hDlg, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_EDIT:
			// Edit self infomation
			MessageBox( hDlg, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_DEL:
			// Edit self infomation
			if( CurrentTable == 1 ) {
				int i = ListView_GetSelectionMark( GetDlgItem( hDlg, IDC_DLG_ADMSHELL_LIST ) );
				if( i != -1 ) tAdmin.Delete( i, ODBCM.getHdbc( ) );
				else MessageBox( hDlg, _T( "请选择一项以删除！" ), NULL, NULL );
				goto REFRESH_ADM_SHOW;
			}
			else MessageBox( hDlg, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );
			return (INT_PTR)TRUE;

		case IDC_DLG_ADMSHELL_BTN_ADD:
			// Edit self infomation
			MessageBox( hDlg, _T( "On developing...Σ(っ °Д °;)っ" ), _T( "Sorry" ), MB_OK );
			return (INT_PTR)TRUE;

		//case 
		}
		break;
	}
	return (INT_PTR)FALSE;
}

INT_PTR CALLBACK DlgAdminPwProc( HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam )
{
	UNREFERENCED_PARAMETER(lParam);

	char P1[ 256 ], P2[ 256 ], P3[ 256 ], temp[ 256 ], temp2[ 256 ];

	switch (message)
	{
	case WM_INITDIALOG:
		MoveWindowToClientCentre( hDlg );
		ADMPW_RSALL:
		SetWindowText( GetDlgItem( hDlg, IDC_EDIT1 ), _T( "" ) );
		SetWindowText( GetDlgItem( hDlg, IDC_EDIT2 ), _T( "" ) );
		SetWindowText( GetDlgItem( hDlg, IDC_EDIT3 ), _T( "" ) );
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if( LOWORD(wParam) == IDCANCEL ) {
			ADMPW_ENDDLG:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if( LOWORD(wParam) == IDOK ) {
			memset( P1, '\0', 256 );
			memset( P2, '\0', 256 );
			memset( P3, '\0', 256 );
			memset( temp, '\0', 256 );
			memset( temp2, '\0', 256 );

			GetWindowTextA( GetDlgItem( hDlg, IDC_EDIT1 ), P1, 255 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_EDIT2 ), P2, 255 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_EDIT3 ), P3, 255 );

			if( strlen( P1 ) > 20 || strlen( P2 ) > 20 || strlen( P3 ) > 20 ) {
				MessageBox( hDlg, _T( "密码长度非法，0-20字符！" ), NULL ,NULL );
				goto ADMPW_RSALL;
			}
			else if( strcmp( P2, P3 ) ) {
				MessageBox( hDlg, _T( "两次输入的新密码不匹配！" ), NULL ,NULL );
				goto ADMPW_RSALL;
			}

			sha256_32byte( (unsigned char *)P1, (unsigned char *)temp );
			byte_array_to_str( (unsigned char *)temp, temp2, 32 );
			if( !tAdmin.JudgePassword( temp2 ) ) {
				MessageBox( hDlg, _T( "原密码错误！" ), NULL ,NULL );
				goto ADMPW_RSALL;
			}
			
			sha256_32byte( (unsigned char *)P2, (unsigned char *)P3 );
			byte_array_to_str( (unsigned char *)P3, temp, 32 );
			if( !tAdmin.SetNewPassword( ODBCM.getHdbc( ), temp2, temp ) ){
				MessageBox( hDlg, _T( "DlgAdminPwProc未知错误！" ), NULL ,NULL );
				goto ADMPW_RSALL;
			}
			else {
				MessageBox( hDlg, _T( "修改成功！" ), _T( "Succeeded" ) ,NULL );
				goto ADMPW_ENDDLG;
			}
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
