#ifndef _CONNECT_SQL_
#define _CONNECT_SQL_

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <windows.h>
#pragma comment(lib,"odbc32.lib")

BOOL ValidateLogon(const char LoginUsername[20], const char LoginPassword[20])//登录验证。
{
	SQLRETURN ret;//返回信息
	SQLHENV henv;//环境句柄
	SQLHDBC hdbc;//连接句柄
	SQLHSTMT hstmt;//语句句柄

	SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境
	SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接

	ret = SQLConnect(hdbc, (SQLCHAR*)"CMSDB", SQL_NTS, (SQLCHAR*)"sa", SQL_NTS, (SQLCHAR*)"123456", SQL_NTS);//连接数据库
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)//连接数据库成功
	{
		SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请SQL语句
		SQLCHAR ConnectDatabaseSql[] = "use CMSDB;";//使用指定数据库
		SQLExecDirect(hstmt, (SQLCHAR*)ConnectDatabaseSql, SQL_NTS);//执行语句
		SQLCHAR SearchResult[1024];//定义SQL语句
		char SQLSearch[1024];
		sprintf_s(SQLSearch, "select * from User_Info where User_ID = '%s'", LoginUsername);
		strcpy((char*)SearchResult, SQLSearch);//语句赋值
		SQLPrepare(hstmt, (SQLCHAR*)SearchResult, SQL_NTS);//绑定语句准备执行
		ret = SQLExecute(hstmt);//执行搜索语句
		if (ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)//搜索成功
		{
			SQLCHAR USERNAME[20], PASSWORD[20];
			SQLINTEGER USERNAME_Len, PASSWORD_Len;
			while (SQLFetch(hstmt) != SQL_NO_DATA)
			{
				SQLGetData(hstmt, 1, SQL_C_CHAR, USERNAME, 20, &USERNAME_Len);
				SQLGetData(hstmt, 2, SQL_C_CHAR, PASSWORD, 20, &PASSWORD_Len);
				if (strcmp(LoginUsername, (char*)USERNAME) == 0 && strcmp(LoginPassword, (char*)PASSWORD) == 0)
				{
					SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
					SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
					SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
					return TRUE;
				}
				SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
				SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
				return FALSE;
			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
		SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
		return FALSE;
	}
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
	SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
	return FALSE;
}

















#endif // !_CONNECT_SQL_
