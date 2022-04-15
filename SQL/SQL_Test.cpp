// SQL_Test.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。


#include <iostream>
#include <Windows.h>
#include <sql.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <string>
#include <atlstr.h>
#include <WinNls.h>
#include <comdef.h>

#pragma warning(disable:4996)
char* wstring2charPtr (const std::wstring Wstring_in)
{
	_bstr_t Temp = Wstring_in.c_str();
	char* pchar = (char*)Temp;
	char* result = new char;
	strcpy(result, pchar);
	return result;
}

void error(SQLRETURN err) {
	printf("结果输出：");
	switch (err) {
	case	SQL_SUCCESS:
		puts("****SQL_SUCCESS*****");
		break;
	case	SQL_SUCCESS_WITH_INFO:
		puts("SQL_SUCCESS_WITH_INFO");
		break;
	case	SQL_ERROR:
		puts("SQL_ERROR");
		break;
	case	SQL_INVALID_HANDLE:
		puts("SQL_INVALID_HANDLE");
		break;
	case	SQL_NO_DATA_FOUND:
		puts("SQL_NO_DATA_FOUND");
		break;
	case	SQL_NEED_DATA:
		puts("SQL_NEED_DATA");
		break;
	default:
		puts("err");
	}
}

bool IsInsertSql(std::wstring Sql_Select_Target, std::wstring Sql_Table_Insret)
{
	std::cout << "开始连接SQL\n";
	std::cout << "-------------------------------------\n";
#pragma region 句柄声明
	SQLRETURN ret = NULL;	//返回信息
	SQLHENV henv = NULL;	//环境句柄
	SQLHDBC hdbc = NULL;	//连接句柄
	SQLHSTMT hstmt = NULL;	//语句句柄

	SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境
	SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接
#pragma endregion

#pragma region Location_Test
	std::wstring DataBase = L"Test";
	std::wstring Login_Name = L"sa";
	std::wstring Login_Pass = L"123456";

	ret = SQLConnect(hdbc,
		(SQLWCHAR*)DataBase.c_str(), SQL_NTS,
		(SQLWCHAR*)Login_Name.c_str(), SQL_NTS,
		(SQLWCHAR*)Login_Pass.c_str(), SQL_NTS);//连接数据库
#pragma endregion

#pragma region Cust_Connect_Sql
	//ret = SQLConnect(hdbc,
	//	(SQLWCHAR*)aoi.cust_params[1].c_str(), SQL_NTS,	//database
	//	(SQLWCHAR*)aoi.cust_params[2].c_str(), SQL_NTS,	//UserName
	//	(SQLWCHAR*)aoi.cust_params[3].c_str(), SQL_NTS);//Password
#pragma endregion

	try
	{
		//数据表insert操作
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			std::cout << "SQL数据库连接成功\n";
			std::cout << "-------------------------------------\n";
			std::cout << "数据表开始查找\n";
			std::cout << "-------------------------------------\n";

			const char* Element_Target = "QinYuan";		//指定搜寻的元素

			//std::wstring Sql_Select_Target = _T("[Name],[Num],[Age]");		//指定表中的列名
			ret = SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);//申请语句
			//std::wstring Sql_Select_Target = _T("[Name],[Num],[Age]");		//指定表中的列名
			std::wstring Sql_Table = _T("dbo.Table_1");						//指定表名
			//std::wstring Sql_Table_Insret = _T("'QinYuan','2','25'");		//插入数据（插入后不可更改）	
			std::wstring str_insert = _T("insert into ") + DataBase + _T(".") + Sql_Table + _T("(") + Sql_Select_Target + _T(")")
				+ _T(" values(") + Sql_Table_Insret + _T(")");
			ret = SQLExecDirect(hstmt, (SQLWCHAR*)str_insert.c_str(), SQL_NTS);
			error(ret);
			std::cout << "-------------------------------------\n";
			if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO) 
			{
				std::cout << "数据表搜索成功！" << std::endl;
				std::cout << "-------------------------------------\n";
				std::wcout << "插入：" << Sql_Table_Insret << std::endl;
				std::cout << "-------------------------------------\n";
				std::cout << "数据库表插入成功,释放INSERT环境！\n";
				std::cout << "-------------------------------------\n";
				ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
				ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
				ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
				return true;
			}
			std::cout << "数据表插入失败,释放INSERT环境！\n";
			std::cout << "-------------------------------------\n";
			error(ret);
			ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
			ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
			ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
			return true;
		}
		else
		{
			std::cout << "-------------------------------------\n";
			std::cout << "SQL数据库连接失败！\n";
			std::cout << "-------------------------------------\n";
			ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
			ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
			return false;
		}
	}
	catch (...)
	{
		error(ret);
		return false;
	}
	std::cout << "-------------------------------------\n";
	std::cout << "SQL数据库连接失败！\n";
	std::cout << "-------------------------------------\n";
	error(ret);
	std::cout << "-------------------------------------\n";
	ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
	ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
	return false;
}

bool IsSelectSql(const char* Element_Target)
{
	std::cout << "开始连接SQL\n";
	std::cout << "-------------------------------------\n";
#pragma region 句柄声明
	SQLRETURN ret;	//返回信息
	SQLHENV henv;	//环境句柄
	SQLHDBC hdbc;	//连接句柄
	SQLHSTMT hstmt;	//语句句柄

	SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境
	SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接
#pragma endregion

#pragma region Location_Test
	std::wstring DataBase = L"Test";
	std::wstring Login_Name = L"sa";
	std::wstring Login_Pass = L"123456";

	ret = SQLConnect(hdbc,
		(SQLWCHAR*)DataBase.c_str(), SQL_NTS,
		(SQLWCHAR*)Login_Name.c_str(), SQL_NTS,
		(SQLWCHAR*)Login_Pass.c_str(), SQL_NTS);//连接数据库
#pragma endregion

#pragma region Cust_Connect_Sql
	//ret = SQLConnect(hdbc,
	//	(SQLWCHAR*)aoi.cust_params[1].c_str(), SQL_NTS,	//database
	//	(SQLWCHAR*)aoi.cust_params[2].c_str(), SQL_NTS,	//UserName
	//	(SQLWCHAR*)aoi.cust_params[3].c_str(), SQL_NTS);//Password
#pragma endregion

	try
	{
		//数据表操作
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			std::cout << "SQL数据库连接成功\n";
			std::cout << "-------------------------------------\n";
			std::cout << "数据表开始查找\n";
			std::cout << "-------------------------------------\n";

			//const char* Element_Target = "QinYuan";		//指定搜寻的元素

			SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);		//申请SQL语句
			//std::wstring DB_Name = _T("Test");	
			//std::wstring Sql_Apply_DB = _T("use ") + DB_Name;	//指定数据库
			std::wstring Sql_Select_Target = _T("[Name],[Num],[Age]");		//指定表中的列名
			std::wstring Sql_Table = _T("dbo.Table_1");						//指定表名
			std::wstring Sql_Apply_Table =_T(" SELECT ") + Sql_Select_Target 
											+ _T(" FROM ")
											+ DataBase 
											+ _T(".")
											+ Sql_Table;		//指定表 语句
			//ret = SQLExecDirect(hstmt, (SQLWCHAR*)Sql_Apply_DB.c_str(), SQL_NTS);	//指定数据库
			ret = SQLExecDirect(hstmt, (SQLWCHAR*)Sql_Apply_Table.c_str(), SQL_NTS); //指定表

			if (ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)//搜索成功
			{
				std::cout << "数据库表搜索成功\n";
				std::cout << "-------------------------------------\n";
				SQLWCHAR Name[20] = { 0 }, Num[20] = { 0 }, Age[20] = {0};
				SQLLEN len_str1, len_str2, len_str3;
				printf("姓名\t  编号\t年龄\n");
				while (SQLFetch(hstmt) != SQL_NO_DATA)//获取存在的每一行
				{
					SQLGetData(hstmt, 1, SQL_C_CHAR, Name, 50, &len_str1);   //获取第一列数据
					SQLGetData(hstmt, 2, SQL_C_CHAR, Num, 50, &len_str2);
					SQLGetData(hstmt, 3, SQL_C_CHAR, Age, 50, &len_str3);
					printf("%s\t  %s\t%s\n",(char*)Name,(char*)Num,(char*)Age);
					if (strcmp(Element_Target,(char*)Name) == 0)//获取目标元素
					{
						std::cout << "-------------------------\n";
						std::cout << "找到Target：" << (char*)Name << std::endl;
						std::cout << "-------------------------\n";
					}
				}
				std::cout << "-------------------------------------\n";
				std::cout << "数据库表检索结束,释放SELECT环境\n";
				std::cout << "-------------------------------------\n";
				ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
				ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
				ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
				return true;
			}
			else
			{
				std::cout << "-------------------------------------\n";
				std::cout << "数据库表搜索失败,释放环境\n";
				std::cout << "-------------------------------------\n";
				ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
				ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
				ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
			}
			ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
			ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
			ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
			return true;
		}
		else
		{
			std::cout << "-------------------------------------\n";
			std::cout << "SQL数据库连接失败\n";
			std::cout << "-------------------------------------\n";
			return false;
		}
	}
	catch (...)
	{
		return false;
	}
	std::cout << "-------------------------------------\n";
	std::cout << "数据库连接失败,释放环境\n";
	std::cout << "-------------------------------------\n";
	ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
	ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
	return false;
}

bool IsDelateSql(std::wstring Sql_Table_Delate)
{
	std::cout << "开始连接SQL\n";
	std::cout << "-------------------------------------\n";
#pragma region 句柄声明
	SQLRETURN ret;	//返回信息
	SQLHENV henv;	//环境句柄
	SQLHDBC hdbc;	//连接句柄
	SQLHSTMT hstmt;	//语句句柄

	SQLAllocHandle(SQL_HANDLE_ENV, NULL, &henv);//申请环境
	SQLSetEnvAttr(henv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER);//设置环境
	SQLAllocHandle(SQL_HANDLE_DBC, henv, &hdbc);//申请数据库连接
#pragma endregion

#pragma region Location_Test
	std::wstring DataBase = L"Test";
	std::wstring Login_Name = L"sa";
	std::wstring Login_Pass = L"123456";

	ret = SQLConnect(hdbc,
		(SQLWCHAR*)DataBase.c_str(), SQL_NTS,
		(SQLWCHAR*)Login_Name.c_str(), SQL_NTS,
		(SQLWCHAR*)Login_Pass.c_str(), SQL_NTS);//连接数据库
#pragma endregion

#pragma region Cust_Connect_Sql
	//ret = SQLConnect(hdbc,
	//	(SQLWCHAR*)aoi.cust_params[1].c_str(), SQL_NTS,	//database
	//	(SQLWCHAR*)aoi.cust_params[2].c_str(), SQL_NTS,	//UserName
	//	(SQLWCHAR*)aoi.cust_params[3].c_str(), SQL_NTS);//Password
#pragma endregion

	try
	{
		//数据表操作
		if (ret == SQL_SUCCESS || ret == SQL_SUCCESS_WITH_INFO)
		{
			std::cout << "SQL数据库连接成功\n";
			std::cout << "-------------------------------------\n";
			std::cout << "数据表开始查找\n";
			std::cout << "-------------------------------------\n";

			SQLAllocHandle(SQL_HANDLE_STMT, hdbc, &hstmt);		//申请SQL语句
			//std::wstring DB_Name = _T("Test");	
			//std::wstring Sql_Apply_DB = _T("use ") + DB_Name;	//指定数据库
			std::wstring Sql_Select_Target = _T("[Name],[Num],[Age]");		//指定表中的列名
			std::wstring Sql_Table = _T("dbo.Table_1");						//指定表名
			//std::wstring Sql_Table_Delate = L"6";						    //指定删除目标ID
			std::wstring Sql_Apply_Table = _T("delete from Test.dbo.Table_1 where [Num]=") + Sql_Table_Delate;	//指定表 语句
			ret = SQLExecDirect(hstmt, (SQLWCHAR*)Sql_Apply_Table.c_str(), SQL_NTS); //指定表
			error(ret);

			if (ret == SQL_SUCCESS_WITH_INFO || ret == SQL_SUCCESS)//搜索成功
			{
				std::cout << "数据库表搜索成功\n";
				std::cout << "-------------------------------------\n";
				std::wcout << "删除：" << Sql_Table_Delate << std::endl;
				std::cout << "-------------------------------------\n";
				std::cout << "-------------------------------------\n";
				std::cout << "数据库表删除结束,释放DELATE环境\n";
				std::cout << "-------------------------------------\n";
				ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
				ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
				ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
				return true;
			}
			else
			{
				std::cout << "-------------------------------------\n";
				std::cout << "数据库表删除失败,释放环境\n";
				std::cout << "-------------------------------------\n";
				ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
				ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
				ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
			}
			ret = SQLFreeHandle(SQL_HANDLE_STMT, hstmt);//释放语句
			ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
			ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
			return true;
		}
		else
		{
			std::cout << "-------------------------------------\n";
			std::cout << "SQL数据库连接失败\n";
			std::cout << "-------------------------------------\n";
			ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
			ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
			return false;
		}
	}
	catch (...)
	{
		return false;
	}
	std::cout << "-------------------------------------\n";
	std::cout << "数据库连接失败,释放环境\n";
	std::cout << "-------------------------------------\n";
	ret = SQLFreeHandle(SQL_HANDLE_DBC, hdbc);//释放连接
	ret = SQLFreeHandle(SQL_HANDLE_ENV, henv);//释放环境
	return false;
}

int main()
{
//#pragma region INSERT
//
//	std::cout << "-------------------------------------\n";
//	std::cout << "开始执行INSERT函数\n";
//	std::cout << "-------------------------------------\n";
//
//	if (IsInsertSql(_T("[Name],[Num],[Age]"), //插入目标列
//					_T("'QinYuan',7,35")))//插入数据
//	{
//		std::cout << "结束INSERT函数\n";
//		std::cout << "-------------------------------------\n";
//	}
//	else
//	{
//		std::cout << "失败执行INSERT函数\n";
//		std::cout << "-------------------------------------\n";
//	}
//
//#pragma endregion

#pragma region SELECT
	std::cout << "开始执行SELECT函数\n";
	std::cout << "-------------------------------------\n";
	std::wstring Target = L"QinYuan";
	if (IsSelectSql((_bstr_t)Target.c_str()))//指定搜寻的元素
						//_bstr_t 任意类型，自动适配
	{
		std::cout << "结束SELECT函数\n";
		std::cout << "-------------------------------------\n";
	}
	else
	{
		std::cout << "失败执行SELECT函数\n";
		std::cout << "-------------------------------------\n";
	}
#pragma endregion

//#pragma region DELATE
//	std::cout << "开始执行DELATE函数\n";
//	std::cout << "-------------------------------------\n";
//
//	if (IsDelateSql(L"7"))//指定删除的元素ID
//	{
//		std::cout << "结束DELATE函数\n";
//		std::cout << "-------------------------------------\n";
//	}
//	else
//	{
//		std::cout << "失败执行DELATE函数\n";
//		std::cout << "-------------------------------------\n";
//	}
//	/// <summary>
//	/// 查询是否删除
//	/// </summary>
//	/// <returns></returns>
//	std::cout << "开始执行SELECT函数\n";
//	std::cout << "-------------------------------------\n";
//	if (IsSelectSql("QinYuan"))//指定搜寻的元素
//	{
//		std::cout << "结束SELECT函数\n";
//		std::cout << "-------------------------------------\n";
//	}
//	else
//	{
//		std::cout << "失败执行SELECT函数\n";
//		std::cout << "-------------------------------------\n";
//	}
//
//#pragma endregion


}