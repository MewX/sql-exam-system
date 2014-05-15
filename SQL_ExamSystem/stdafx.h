/**
 * Name: StdAfx.h
 * Author: MewCatcher
 * Date: 2014/05/05
 * Description: This file define the most shared headers and variables.
 **/

#ifndef _STDAFX_H_
#define _STDAFX_H_

#include "targetver.h"

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头中排除极少使用的资料

// Windows 头文件:
#include <windows.h>
#include <commctrl.h>

#pragma comment( lib, "comctl32.lib" )

// SQL Headers
#include <sql.h>
#include <sqlext.h>
#include <Sqltypes.h>

// C/C++ 运行时头文件
#include <cstdlib>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <string>
#include <sstream>

using namespace std;

// My Headers
#include "sha256.h"
#include "ODBCManagement.h"
#include "TableAdmin.h"
#include "TableStudent.h"

// My Global Functions

// My Global Vars
static ODBCManagement ODBCM;
static TableAdmin tAdmin;
static TableStudent tStudent;

#endif
