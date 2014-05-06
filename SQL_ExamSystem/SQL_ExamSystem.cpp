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
/*TCHAR szTitle[MAX_LOADSTRING];					// 标题栏文本
TCHAR szWindowClass[MAX_LOADSTRING];			// 主窗口类名

// 此代码模块中包含的函数的前向声明:
ATOM				MyRegisterClass(HINSTANCE hInstance);*/

// UI Procs
INT_PTR CALLBACK	DlgAboutProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	DlgLoginProc(HWND, UINT, WPARAM, LPARAM);

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
	InitCommonControls( );
	DialogBox( hInstance, (LPCTSTR)IDD_DLG_LOGIN, NULL, (DLGPROC)DlgLoginProc );


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
		if( !SQL_SUCCEEDED( ODBCM.ODBCConnect( _T( "MewCatcherSQLServer" ), _T("sa"), _T("sa") ) ) ) { // test connection info
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

		case IDC_DLG_LOGIN_BUTTON_LOGIN:
			GetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_PASSWORD ), Password, 255 ); // test
			//SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_USERNAME ), Password );

			sha256_32byte( (unsigned char *)Password, (unsigned char *)temp );
			byte_array_to_str( (unsigned char *)temp, UserName, 32 );
			SetWindowTextA( GetDlgItem( hDlg, IDC_DLG_LOGIN_EDIT_USERNAME ), UserName );

			return (INT_PTR)TRUE;

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
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}