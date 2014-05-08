/**
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

// UI Procs
INT_PTR CALLBACK	DlgAboutProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgLoginProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgRegProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgAdminShellProc(HWND, UINT, WPARAM, LPARAM);

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
	DialogBox( hInstance, (LPCTSTR)IDD_DLG_LOGIN, NULL, (DLGPROC)DlgLoginProc );

	if( ProcedureId == 2 ) { // Admin Shell Dialog
		DialogBox( hInstance, (LPCTSTR)IDD_DLG_ADMSHELL, NULL, (DLGPROC)DlgAdminShellProc );
		goto BEGIN;
	}
	if( ProcedureId == 3 ) { // Student Shell Dialog
		;
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


/**
 *  All the dialog procs' main function
 **/
INT_PTR CALLBACK DlgLoginProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	static int ConnectionInfo = 0; // 0 - Not Connected; -1 - Connection failed; 1 - Connection succeeded.
	static char UserName[ 256 ] = { '\0' }, Password[ 256 ] = { '\0' }, temp[ 256 ] = { '\0' };
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
		//SendMessage( hDlg, WM_PAINT, (WPARAM)GetDC( hDlg ), NULL );
		//Invalidate( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) );
		//SetBkColor( GetDC( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) ), GetPixel( GetDC(hDlg), 16, 16 ) );
		//InvalidateRect( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ), &tempRECT, FALSE );
		//UpdateWindow( GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) );
		//RedrawWindow( hDlg, &tempRECT, NULL, RDW_ERASE );

		//SendMessage( hDlg, WM_CTLCOLORDLG, (WPARAM)GetDC( hDlg ), NULL );
		//RedrawWindow( hDlg, &tempRECT, NULL, RDW_ERASE | RDW_NOCHILDREN );

		hdc = (HDC)wParam;
		if( (HWND)lParam == GetDlgItem( hDlg, IDC_DLG_LOGIN_CONNECTSTATUES ) ) { // Set Specific Color
			if( ConnectionInfo == 1 ) SetTextColor( (HDC)wParam, RGB( 21, 125, 240 ) );
			else if( ConnectionInfo == -1 ) SetTextColor( (HDC)wParam, RGB( 255, 0, 0 ) );
		}
		SetBkMode( (HDC)wParam, TRANSPARENT ); // Set TRANSPARENT

		//return (LRESULT)GetStockObject( NULL_BRUSH );
		return (LRESULT)CreateSolidBrush(GetPixel( GetDC(hDlg), 16, 16 ) );


	case WM_COMMAND:
		switch( LOWORD(wParam) ) {
		case IDCANCEL:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;

		case IDC_DLG_LOGIN_BUTTON_ABOUT:
			DialogBox( hInst, (LPCTSTR)IDD_DLG_ABOUT, hDlg, (DLGPROC)DlgAboutProc );
			return (INT_PTR)TRUE;

		case IDC_DLG_LOGIN_BUTTON_LOGIN: // This the kernel part !!!
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_PASSWORD ), Password, 255 ); // test
			//SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_USERNAME ), Password );

			sha256_32byte( (unsigned char *)Password, (unsigned char *)temp );
			byte_array_to_str( (unsigned char *)temp, UserName, 32 );
			SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_USERNAME ), UserName );

			ProcedureId = 2; // test
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
	case WM_INITDIALOG: {
		// reset window position
			int ScreenWidth, ScreenHeight;
			RECT rc = { 0 };
			ScreenWidth = GetSystemMetrics( SM_CXSCREEN );
			ScreenHeight = GetSystemMetrics( SM_CYSCREEN );
			::GetWindowRect(hDlg, &rc);
			::SetWindowPos( hDlg, HWND_NOTOPMOST, ( ScreenWidth - ( rc.right - rc.left ) ) / 2, ( ScreenHeight - ( rc.bottom - rc.top ) ) / 2,
				( rc.right - rc.left ), ( rc.bottom - rc.top ), SWP_SHOWWINDOW);
		}
		SetWindowText( GetDlgItem( hDlg, IDC_DLG_ABOUT_EDIT_OS ), _T(" OpenSource - https://github.com/MewCatcher/SQL_ExamSystem/") );
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
	case WM_INITDIALOG: {
		// reset window position
			int ScreenWidth, ScreenHeight;
			RECT rc = { 0 };
			ScreenWidth = GetSystemMetrics( SM_CXSCREEN );
			ScreenHeight = GetSystemMetrics( SM_CYSCREEN );
			::GetWindowRect(hDlg, &rc);
			::SetWindowPos( hDlg, HWND_NOTOPMOST, ( ScreenWidth - ( rc.right - rc.left ) ) / 2, ( ScreenHeight - ( rc.bottom - rc.top ) ) / 2,
				( rc.right - rc.left ), ( rc.bottom - rc.top ), SWP_SHOWWINDOW);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if( LOWORD(wParam) == IDCANCEL ) {
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		else if( LOWORD(wParam) == IDC_DLG_REG_BTN_REG )
		{
			// Here process the reg logic
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_USERNAME ), UserName, 255 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD1 ), Password1, 255 );
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_REG_EDIT_PASSWORD2 ), Password2, 255 );

			// test enmpty
			if( strlen( UserName ) < 4 || strlen( UserName ) > 20 ) {
				MessageBox( hDlg, _T( "UserName长度非法！" ), _T( "Error" ), MB_OK );
				return (INT_PTR)TRUE;
			}
			
			// test case able
			for( int i = 0; i < strlen( UserName ); i ++ ) {
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
			for( int i = 0; i < strlen( Password1 ); i ++ ) {
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
			else MessageBoxW( hDlg, ((wstring)L"注册成功！\r\n您是第" + wtempId + L"位注册的考生！\r\n祝您使用愉快！~" ).c_str( ), NULL ,NULL );

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
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG: {
		// reset window position
			int ScreenWidth, ScreenHeight;
			RECT rc = { 0 };
			ScreenWidth = GetSystemMetrics( SM_CXSCREEN );
			ScreenHeight = GetSystemMetrics( SM_CYSCREEN );
			::GetWindowRect(hDlg, &rc);
			::SetWindowPos( hDlg, HWND_NOTOPMOST, ( ScreenWidth - ( rc.right - rc.left ) ) / 2, ( ScreenHeight - ( rc.bottom - rc.top ) ) / 2,
				( rc.right - rc.left ), ( rc.bottom - rc.top ), SWP_SHOWWINDOW);
		}
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		switch( LOWORD(wParam) ) {
		case IDCANCEL: case IDC_DLG_ADMSHELL_BTN_EXIT:
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
