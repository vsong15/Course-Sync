#pragma once

#include <Windows.h>
#include <string>
#include <libpq-fe.h>
#include <vector>

class DatabaseHelper
{
public:
    static PGconn* ConnectToDatabase(const char* dbname, const char* user, const char* password, const char* host, int port);
    static void CloseDatabaseConnection(PGconn* conn);
    static PGresult* ExecuteQuery(PGconn* conn, const char* query);
    static bool CheckUser(const wchar_t* username, const wchar_t* password);
    static std::string WStringToString(const std::wstring& wstr);
    static std::string GetRole(const wchar_t* username, const wchar_t* password);
    static std::string GetFirstName(const wchar_t* username, const wchar_t* password);
    static std::string GetLastName(const wchar_t* username, const wchar_t* password);
    static int GetUserID(const wchar_t* username, const wchar_t* password);
    static void StoreLoginTimestamp(int user_id);
    static std::vector<std::string> GetLoginTimestamps(int user_id);
};
