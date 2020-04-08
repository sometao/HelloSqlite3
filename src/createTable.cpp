#include <iostream>
#include <string>

extern "C" {
#include "sqlite3.h"
};

using std::cout;
using std::endl;
using std::string;

static int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  cout << "callback222 :" << endl;
  int i;
  for (i = 0; i < argc; i++) {
    printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
  }
  printf("\n");
  return 0;
}

namespace {
int createTable_1(const string& dbFile) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;

  rc = sqlite3_open(dbFile.c_str(), &db);

  cout << "rc: " << rc << endl;

  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return (1);
  }

  string sql = R"(
         CREATE TABLE COMPANY(
         ID INT PRIMARY KEY     NOT NULL,
         NAME           TEXT    NOT NULL,
         AGE            INT     NOT NULL,
         ADDRESS        CHAR(50),
         SALARY         REAL );
)";

  rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
  cout << "rc: " << rc << endl;

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  sqlite3_close(db);
  return 0;
}

int createTable_2(const string& dbFile) {
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;

  rc = sqlite3_open(dbFile.c_str(), &db);

  cout << "rc: " << rc << endl;

  if (rc) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return (1);
  }

  string sql = R"(
         CREATE TABLE COMPANY(
         ID INT PRIMARY KEY     NOT NULL,
         NAME           TEXT    NOT NULL,
         AGE            INT     NOT NULL,
         ADDRESS        CHAR(50),
         SALARY         REAL );
)";

  rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
  cout << "rc: " << rc << endl;

  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }

  sqlite3_close(db);
  return 0;
}

}  // namespace

int createTable(const string& dbFile) {
  cout << "createTable begin." << endl;
  createTable_1(dbFile);
  cout << "createTable done." << endl;
  return 0;
}
