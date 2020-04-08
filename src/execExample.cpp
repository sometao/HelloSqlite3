#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include "sqlite3.h"

#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif // !SQLITE_OK


using std::cout;
using std::endl;
using std::string;

namespace {

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  int i;
  cout << "runSql callback rst: ";
  for (i = 0; i < argc; i++) {
    cout << azColName[i] << "=[" << (argv[i] ? argv[i] : "NULL") << "]   ";
  }
  cout << endl;
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
}

int runSql(const string& dbFile, const string& sql) {
  cout << "run sql=[" << sql << "] on db=[" << dbFile << "]" << endl;
  sqlite3* db;
  char* zErrMsg = 0;
  int rc;

  rc = sqlite3_open(dbFile.c_str(), &db);
  cout << "open rc: " << rc << endl;

  if (rc) {
    cout << "Can't open database:" << sqlite3_errmsg(db) << endl;
    sqlite3_close(db);
    return rc;
  }

  rc = sqlite3_exec(db, sql.c_str(), callback, 0, &zErrMsg);
  cout << "run rc: " << rc << endl;

  if (rc != SQLITE_OK) {
    cout << "SQL error:" << zErrMsg << endl;
    sqlite3_free(zErrMsg);
    return rc;
  }

  sqlite3_close(db);
  return SQLITE_OK;
}

const string createTableSql = R"(
  CREATE TABLE students(
         id       INTEGER   PRIMARY KEY AUTOINCREMENT,
         name     TEXT      NOT NULL,
         age      INTEGER   NOT NULL,
         class    CHAR(50),
         score    REAL);
)";

const string listTableSql = R"(
  select name from sqlite_master where type='table' order by name;
)";

const string dropTableSql = R"(
  drop table students;
)";

const string insertSql = R"(
  insert into students (age, name, score, class) values (19, 'Tom', 3.4, 'A');
  insert into students (age, name, score, class) values (20, 'Jack', 2.4, 'B');
  insert into students (age, name, score, class) values (18, 'Lucy', 4.1, 'C');
)";

const string selectAllSql = R"(
  SELECT * FROM students;
)";

const string deleteSql = R"(
  DELETE FROM students WHERE score < 3;
)";

const string updateSql = R"(
  UPDATE students SET class = 'A' WHERE score > 4;
)";



}  // namespace

void execExample(const string& dbFile) {
  cout << "testSqlExec runing." << endl;

  runSql(dbFile, selectAllSql);
  cout << " ------------------ \n" << endl;



  //runSql(dbFile, dropTableSql);
  //cout << " ------------------ \n" << endl;
  //runSql(dbFile, createTableSql);
  //cout << " ------------------ \n" << endl;
  runSql(dbFile, insertSql);
  cout << " ------------------ \n" << endl;
  runSql(dbFile, selectAllSql);
  cout << " ------------------ \n" << endl;
  //runSql(dbFile, updateSql);
  //cout << " ------------------ \n" << endl;
  //runSql(dbFile, selectAllSql);
  cout << "testSqlExec finish." << endl;
}