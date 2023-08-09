#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <libpq-fe.h>
#include <string>
#include "DatabaseHelper.h"

PGconn* DatabaseHelper::ConnectToDatabase(const char* dbname, const char* user, const char* password, const char* host, int port) {
    // Convert the port integer to a string
    std::string portStr = std::to_string(port);

    PGconn* conn = PQsetdbLogin(host, portStr.c_str(), NULL, NULL, dbname, user, password);
    if (PQstatus(conn) != CONNECTION_OK) {
        fprintf(stderr, "Connection to database failed: %s\n", PQerrorMessage(conn));
        PQfinish(conn);
        return NULL;
    }

    return conn;
}

void DatabaseHelper::CloseDatabaseConnection(PGconn* conn) {
    PQfinish(conn);
}

PGresult* DatabaseHelper::ExecuteQuery(PGconn* conn, const char* query) {
    PGresult* result = PQexec(conn, query);
    if (PQresultStatus(result) != PGRES_TUPLES_OK) {
        fprintf(stderr, "Query execution failed: %s\n", PQerrorMessage(conn));
        PQclear(result);
        return NULL;
    }

    return result;
}

bool DatabaseHelper::CheckUser(const wchar_t* username, const wchar_t* password) {
    std::string usernameUtf8 = WStringToString(username);
    std::string passwordUtf8 = WStringToString(password);

    if (usernameUtf8.empty() || passwordUtf8.empty())
    {
        // Error handling if the conversion fails or input is empty
        return false;
    }
    // Replace 'your_db_name', 'your_db_user', 'your_db_password', 'your_db_host', and your_db_port'
    // with the appropriate values for your PostgreSQL database configuration.
    PGconn* conn = DatabaseHelper::ConnectToDatabase("coursesyncdb", "postgres", "password", "localhost", 5432);
    if (!conn) {
        fprintf(stderr, "Failed to connect to the database\n");
        return false;
    }

    // Create the query to check if the user exists
    char query[256];
    snprintf(query, sizeof(query), "SELECT COUNT(*) FROM users WHERE username='%s' AND password='%s'", usernameUtf8.c_str(), passwordUtf8.c_str());

    PGresult* result = DatabaseHelper::ExecuteQuery(conn, query);
    if (!result) {
        DatabaseHelper::CloseDatabaseConnection(conn);
        return false;
    }

    int count = atoi(PQgetvalue(result, 0, 0));
    PQclear(result);
    DatabaseHelper::CloseDatabaseConnection(conn);

    return count > 0;
}

std::string DatabaseHelper::WStringToString(const std::wstring& wstr)
{
    int bufferSize = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
    if (bufferSize == 0)
    {
        // Error handling if the conversion fails
        return "";
    }

    std::string result(bufferSize - 1, '\0'); // Subtract 1 to exclude the null terminator
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), -1, &result[0], bufferSize, nullptr, nullptr);

    return result;
}

std::string DatabaseHelper::GetRole(const wchar_t* username, const wchar_t* password) {
    std::string usernameUtf8 = WStringToString(username);
    std::string passwordUtf8 = WStringToString(password);

    if (usernameUtf8.empty() || passwordUtf8.empty()) {
        // Error handling if the conversion fails or input is empty
        return "";
    }

    // Replace 'your_db_name', 'your_db_user', 'your_db_password', 'your_db_host', and your_db_port'
    // with the appropriate values for your PostgreSQL database configuration.
    PGconn* conn = DatabaseHelper::ConnectToDatabase("coursesyncdb", "postgres", "password", "localhost", 5432);
    if (!conn) {
        fprintf(stderr, "Failed to connect to the database\n");
        return "";
    }

    // Create the query to get the role of the user
    char query[256];
    snprintf(query, sizeof(query), "SELECT roles.Role_Name FROM users INNER JOIN roles ON users.Role_ID = roles.Role_ID WHERE Username='%s' AND Password='%s'", usernameUtf8.c_str(), passwordUtf8.c_str());

    PGresult* result = DatabaseHelper::ExecuteQuery(conn, query);
    if (!result) {
        DatabaseHelper::CloseDatabaseConnection(conn);
        return "";
    }

    std::string role = "";
    int rowCount = PQntuples(result);
    if (rowCount > 0) {
        role = PQgetvalue(result, 0, 0);
    }

    PQclear(result);
    DatabaseHelper::CloseDatabaseConnection(conn);

    return role;
}

std::string DatabaseHelper::GetFirstName(const wchar_t* username, const wchar_t* password) {
    std::string usernameUtf8 = WStringToString(username);
    std::string passwordUtf8 = WStringToString(password);

    if (usernameUtf8.empty() || passwordUtf8.empty()) {
        // Error handling if the conversion fails or input is empty
        return "";
    }

    PGconn* conn = DatabaseHelper::ConnectToDatabase("coursesyncdb", "postgres", "password", "localhost", 5432);
    if (!conn) {
        fprintf(stderr, "Failed to connect to the database\n");
        return "";
    }

    // Create the query to get the first name of the user
    char query[256];
    snprintf(query, sizeof(query), "SELECT First_Name FROM users WHERE Username='%s' AND Password='%s'", usernameUtf8.c_str(), passwordUtf8.c_str());

    PGresult* result = DatabaseHelper::ExecuteQuery(conn, query);
    if (!result) {
        DatabaseHelper::CloseDatabaseConnection(conn);
        return "";
    }

    std::string firstName = "";
    int rowCount = PQntuples(result);
    if (rowCount > 0) {
        firstName = PQgetvalue(result, 0, 0);
    }

    PQclear(result);
    DatabaseHelper::CloseDatabaseConnection(conn);

    return firstName;
}

std::string DatabaseHelper::GetLastName(const wchar_t* username, const wchar_t* password) {
    std::string usernameUtf8 = WStringToString(username);
    std::string passwordUtf8 = WStringToString(password);

    if (usernameUtf8.empty() || passwordUtf8.empty()) {
        // Error handling if the conversion fails or input is empty
        return "";
    }

    PGconn* conn = DatabaseHelper::ConnectToDatabase("coursesyncdb", "postgres", "password", "localhost", 5432);
    if (!conn) {
        fprintf(stderr, "Failed to connect to the database\n");
        return "";
    }

    // Create the query to get the last name of the user
    char query[256];
    snprintf(query, sizeof(query), "SELECT Last_Name FROM users WHERE Username='%s' AND Password='%s'", usernameUtf8.c_str(), passwordUtf8.c_str());

    PGresult* result = DatabaseHelper::ExecuteQuery(conn, query);
    if (!result) {
        DatabaseHelper::CloseDatabaseConnection(conn);
        return "";
    }

    std::string lastName = "";
    int rowCount = PQntuples(result);
    if (rowCount > 0) {
        lastName = PQgetvalue(result, 0, 0);
    }

    PQclear(result);
    DatabaseHelper::CloseDatabaseConnection(conn);

    return lastName;
}

int DatabaseHelper::GetUserID(const std::string& username, const std::string& password) {
    PGconn* conn = ConnectToDatabase("coursesyncdb", "postgres", "password", "localhost", 5432);
    if (!conn) {
        fprintf(stderr, "Failed to connect to the database\n");
        return -1;  // Return a sentinel value to indicate an error
    }

    char query[256];
    snprintf(query, sizeof(query), "SELECT User_ID FROM users WHERE Username='%s' AND Password='%s'", username.c_str(), password.c_str());

    PGresult* result = ExecuteQuery(conn, query);
    if (!result) {
        fprintf(stderr, "Failed to retrieve user ID\n");
        CloseDatabaseConnection(conn);
        return -1;
    }

    int user_id = -1;  // Initialize with a sentinel value
    int rowCount = PQntuples(result);
    if (rowCount > 0) {
        user_id = atoi(PQgetvalue(result, 0, 0));
    }

    PQclear(result);
    CloseDatabaseConnection(conn);

    return user_id;
}

void DatabaseHelper::StoreLoginTimestamp(int user_id) {
    PGconn* conn = ConnectToDatabase("coursesyncdb", "postgres", "password", "localhost", 5432);
    if (!conn) {
        fprintf(stderr, "Failed to connect to the database\n");
        return;
    }

    char query[256];
    snprintf(query, sizeof(query), "INSERT INTO login_timestamps (User_ID, Login_Timestamp) VALUES (%d, NOW())", user_id);

    PGresult* result = ExecuteQuery(conn, query);
    if (!result) {
        fprintf(stderr, "Failed to store login timestamp\n");
    }

    CloseDatabaseConnection(conn);
}

std::vector<std::string> DatabaseHelper::GetLoginTimestamps(int user_id) {
    PGconn* conn = ConnectToDatabase("coursesyncdb", "postgres", "password", "localhost", 5432);
    if (!conn) {
        fprintf(stderr, "Failed to connect to the database\n");
        return std::vector<std::string>();
    }

    char query[256];
    snprintf(query, sizeof(query), "SELECT Login_Timestamp FROM login_timestamps WHERE User_ID = %d ORDER BY Login_Timestamp DESC", user_id);

    PGresult* result = ExecuteQuery(conn, query);
    if (!result) {
        fprintf(stderr, "Failed to retrieve login timestamps\n");
        CloseDatabaseConnection(conn);
        return std::vector<std::string>();
    }

    std::vector<std::string> login_timestamps;
    int rowCount = PQntuples(result);
    for (int i = 0; i < rowCount; ++i) {
        login_timestamps.push_back(PQgetvalue(result, i, 0));
    }

    PQclear(result);
    CloseDatabaseConnection(conn);

    return login_timestamps;
}
