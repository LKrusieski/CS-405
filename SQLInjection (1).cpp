// SQLInjection.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

//Larry E Krusieski CS-405 SNHU

#include <algorithm>
#include <iostream>
#include <locale>
#include <tuple>
#include <vector>
#include <regex>

#include "sqlite3.h"

using namespace std;

// DO NOT CHANGE
typedef tuple<string, string, string> user_record;
const string str_where = " where ";

// DO NOT CHANGE
static int callback(void* possible_vector, int argc, char** argv, char** azColName)
{
  if (possible_vector == NULL)
  { // no vector passed in, so just display the results
    for (int i = 0; i < argc; i++) 
    {
      cout << azColName[i] << " = " << (argv[i] ? argv[i] : "NULL") << endl;
    }
    cout << endl;
  }
  else
  {
    vector< user_record >* rows =
      static_cast<vector< user_record > *>(possible_vector);

    rows->push_back(make_tuple(argv[0], argv[1], argv[2]));
  }
  return 0;
}

// DO NOT CHANGE
bool initialize_database(sqlite3* db)
{
  char* error_message = NULL;
  string sql = "CREATE TABLE USERS(" \
    "ID INT PRIMARY KEY     NOT NULL," \
    "NAME           TEXT    NOT NULL," \
    "PASSWORD       TEXT    NOT NULL);";

  int result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
  if (result != SQLITE_OK)
  {
    cout << "Failed to create USERS table. ERROR = " << error_message << endl;
    sqlite3_free(error_message);
    return false;
  }
  cout << "USERS table created." << endl;

  // insert some dummy data
  sql = "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (1, 'Fred', 'Flinstone');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (2, 'Barney', 'Rubble');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (3, 'Wilma', 'Flinstone');" \
    "INSERT INTO USERS (ID, NAME, PASSWORD)" \
    "VALUES (4, 'Betty', 'Rubble');";

  result = sqlite3_exec(db, sql.c_str(), callback, NULL, &error_message);
  if (result != SQLITE_OK)
  {
    cout << "Data failed to insert to USERS table. ERROR = " << error_message << endl;
    sqlite3_free(error_message);
    return false;
  }

  return true;
}

bool run_query(sqlite3* db, const string& sql, vector< user_record >& records)
{
    // clear any prior results
    records.clear();

    // downshift the input string to lowercase so that it can be matched
    string normalized = sql;
    transform(normalized.begin(), normalized.end(), normalized.begin(), ::tolower);

    // detect a numeric tautology
    regex numeric_injection(R"(\bor\s+([0-9]+)\s*=\s*\1\s*;?)");

    // detect a string tautology
    regex string_injection(R"(\bor\s+'([^']*)'\s*=\s*'\1'\s*;?)");

    if (regex_search(normalized, numeric_injection) ||
        regex_search(normalized, string_injection))
    {
        cout << "Potential SQL injection attack detected. Query blocked." << endl;
        return false;
    }

    char* error_message = nullptr;

    if (sqlite3_exec(db, sql.c_str(), callback, &records, &error_message) != SQLITE_OK)
    {
        cout << "Data failed to be queried from USERS table. ERROR = " << error_message << endl;
        sqlite3_free(error_message);
        return false;
    }

    return true;
}

// DO NOT CHANGE
bool run_query_injection(sqlite3* db, const string& sql, vector< user_record >& records)
{
  string injectedSQL(sql);
  string localCopy(sql);

  // we work on the local copy because of the const
  transform(localCopy.begin(), localCopy.end(), localCopy.begin(), ::tolower);
  if(localCopy.find_last_of(str_where) >= 0)
  { // this sql has a where clause
    if(localCopy.back() == ';')
    { // smart SQL developer terminated with a semicolon - we can fix that!
      injectedSQL.pop_back();
    }

    switch (rand() % 4)
    {
    case 1:
      injectedSQL.append(" or 2=2;");
      break;
    case 2:
      injectedSQL.append(" or 'hi'='hi';");
      break;
    case 3:
      injectedSQL.append(" or 'hack'='hack';");
      break;
    case 0:
    default:
      injectedSQL.append(" or 1=1;");
      break;
    }
  }
  
  return run_query(db, injectedSQL, records);
}


// DO NOT CHANGE
void dump_results(const string& sql, const vector< user_record >& records)
{
  cout << endl << "SQL: " << sql << " ==> " << records.size() << " records found." << endl;

  for (auto record : records)
  {
    cout << "User: " << get<1>(record) << " [UID=" << get<0>(record) << " PWD=" << get<2>(record) << "]" << endl;
  }
}

// DO NOT CHANGE
void run_queries(sqlite3* db)
{
  char* error_message = NULL;

  vector< user_record > records;

  // query all
  string sql = "SELECT * from USERS";
  if (!run_query(db, sql, records)) return;
  dump_results(sql, records);

  //  query 1
  sql = "SELECT ID, NAME, PASSWORD FROM USERS WHERE NAME='Fred'";
  if (!run_query(db, sql, records)) return;
  dump_results(sql, records);

  //  run query 1 with injection 5 times
  for (auto i = 0; i < 5; ++i)
  {
    if (!run_query_injection(db, sql, records)) continue;
    dump_results(sql, records);
  }

}

// You can change main by adding stuff to it, but all of the existing code must remain, and be in the
// in the order called, and with none of this existing code placed into conditional statements
int main()
{
  // initialize random seed:
  srand(time(nullptr));

  int return_code = 0;
  cout << "SQL Injection Example" << endl;

  // the database handle
  sqlite3* db = NULL;
  char* error_message = NULL;

  // open the database connection
  int result = sqlite3_open(":memory:", &db);

  if(result != SQLITE_OK)
  {
    cout << "Failed to connect to the database and terminating. ERROR=" << sqlite3_errmsg(db) << endl;
    return -1;
  }

  cout << "Connected to the database." << endl;

  // initialize our database
  if(!initialize_database(db))
  {
    cout << "Database Initialization Failed. Terminating." << endl;
    return_code = -1;
  }
  else
  {
    run_queries(db);
  }

  // close the connection if opened
  if(db != NULL)
  {
    sqlite3_close(db);
  }

  return return_code;
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu
