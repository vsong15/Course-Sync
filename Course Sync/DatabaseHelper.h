#pragma once

#include <Windows.h>
#include <string>
#include <libpq-fe.h>

class DatabaseHelper
{
public:
    static PGconn* ConnectToDatabase(const char* dbname, const char* user, const char* password, const char* host, int port);
    static void CloseDatabaseConnection(PGconn* conn);
    static PGresult* ExecuteQuery(PGconn* conn, const char* query);
    static bool CheckUser(const wchar_t* username, const wchar_t* password);
    static std::string WStringToString(const std::wstring& wstr);
    static std::string GetRole(const wchar_t* username, const wchar_t* password);
};
