#ifndef _CONNECT_SQL_
#define _CONNECT_SQL_

#include <iostream>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <windows.h>
#pragma comment(lib,"odbc32.lib")

BOOL ValidateLogon(const char LoginUsername[20], const char LoginPassword[20])//��¼��֤��
{
	SQLRETURN ret;//������Ϣ
	SQLHENV henv;//�������
	SQLHDBC hdbc;//���Ӿ��
	SQLHSTMT hstmt;//�����

	SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//���뻷��
	SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//���û���
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//�������ݿ�����

	ret = SQLConnect(hdbc, (SQLCHAR*)"CMSDB", SQL_NTS, (SQLCHAR*)"sa", SQL_NTS, (SQLCHAR*)"123456", SQL_NTS);//�������ݿ�
	if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)//�������ݿ�ɹ�
	{
		SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//����SQL���
		SQLCHAR ConnectDatabaseSql[] = "use CMSDB;";//ʹ��ָ�����ݿ�
		SQLExecDirect(hstmt, (SQLCHAR*)ConnectDatabaseSql, SQL_NTS);//ִ�����
		SQLCHAR SearchResult[1024];//����SQL���
		char SQLSearch[1024];
		sprintf_s(SQLSearch, "select * from User_Info where User_ID = '%s'", LoginUsername);
		strcpy((char*)SearchResult, SQLSearch);//��丳ֵ
		SQLPrepare(hstmt, (SQLCHAR*)SearchResult, SQL_NTS);//�����׼��ִ��
		ret = SQLExecute(hstmt);//ִ���������
		if (ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)//�����ɹ�
		{
			SQLCHAR USERNAME[20], PASSWORD[20];
			SQLINTEGER USERNAME_Len, PASSWORD_Len;
			while (SQLFetch(hstmt) != SQL_NO_DATA)
			{
				SQLGetData(hstmt, 1, SQL_C_CHAR, USERNAME, 20, &USERNAME_Len);
				SQLGetData(hstmt, 2, SQL_C_CHAR, PASSWORD, 20, &PASSWORD_Len);
				if (strcmp(LoginUsername, (char*)USERNAME) == 0 && strcmp(LoginPassword, (char*)PASSWORD) == 0)
				{
					SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//�ͷ����
					SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//�ͷ�����
					SQLFreeHandle(SQL_HANDLE_ENV, henv);//�ͷŻ���
					return TRUE;
				}
				SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//�ͷ����
				SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//�ͷ�����
				SQLFreeHandle(SQL_HANDLE_ENV, henv);//�ͷŻ���
				return FALSE;
			}
		}
		SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//�ͷ����
		SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//�ͷ�����
		SQLFreeHandle(SQL_HANDLE_ENV, henv);//�ͷŻ���
		return FALSE;
	}
	SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//�ͷ�����
	SQLFreeHandle(SQL_HANDLE_ENV, henv);//�ͷŻ���
	return FALSE;
}

















#endif // !_CONNECT_SQL_
