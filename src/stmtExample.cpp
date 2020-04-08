#include <iostream>
#include <thread>
#include <string>
#include <chrono>

#include "sqlite3.h"

#ifndef SQLITE_OK
#define SQLITE_OK 0
#endif  // !SQLITE_OK

using std::cout;
using std::endl;
using std::string;

namespace {

int callback(void* NotUsed, int argc, char** argv, char** azColName) {
  int i;
  cout << "stmt callback rst: ";
  for (i = 0; i < argc; i++) {
    cout << azColName[i] << "=[" << (argv[i] ? argv[i] : "NULL") << "]   ";
  }
  cout << endl;
  // std::this_thread::sleep_for(std::chrono::milliseconds(1000));
  return 0;
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

void seleteAllTable(sqlite3* db) {
  cout << "run stmt sql: " << selectAllSql << endl;

  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db, selectAllSql.c_str(), -1, &stmt, nullptr)) {
    cout << "error: Prepare stmt failed: " << sqlite3_errmsg(db) << endl;
    return;
  }

  int rc = 0;
  rc = sqlite3_step(stmt);
  cout << "sqlite3_step rc: " << rc << endl;

  while (rc == SQLITE_ROW) {
    auto id = sqlite3_column_int64(stmt, 0);
    auto name = sqlite3_column_text(stmt, 1);
    auto age = sqlite3_column_int64(stmt, 2);
    auto classValue = sqlite3_column_text(stmt, 3);
    auto score = sqlite3_column_double(stmt, 4);

    cout << "row: id=[" << id << "], name=[" << name << "], age=[" << age;
    cout << "], class=[" << classValue << "], score=[" << score << "]" << endl;

    rc = sqlite3_step(stmt);
    cout << "sqlite3_step rc: " << rc << endl;
  }

  cout << "seleteAllTable done." << endl;

  sqlite3_finalize(stmt);
}

void deleteTable(sqlite3* db) {
  cout << "run stmt sql: " << dropTableSql << endl;

  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db, dropTableSql.c_str(), -1, &stmt, nullptr)) {
    cout << "error: Prepare stmt failed: " << sqlite3_errmsg(db) << endl;
    return;
  }

  int rc = 0;
  rc = sqlite3_step(stmt);
  cout << "sqlite3_step rc: " << rc << endl;

  if (SQLITE_DONE != rc) {
    fprintf(stderr, "statement didn't return DONE [%i]: %s\n", rc, sqlite3_errmsg(db));
  } else {
    printf("statement completed\n\n");
  }

  sqlite3_finalize(stmt);
}

void createTable(sqlite3* db) {
  cout << "run stmt sql: " << createTableSql << endl;

  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db, createTableSql.c_str(), -1, &stmt, nullptr)) {
    cout << "error: Prepare stmt failed: " << sqlite3_errmsg(db) << endl;
    return;
  }

  int rc = 0;
  rc = sqlite3_step(stmt);
  cout << "sqlite3_step rc: " << rc << endl;

  if (SQLITE_DONE != rc) {
    fprintf(stderr, "statement didn't return DONE [%i]: %s\n", rc, sqlite3_errmsg(db));
  } else {
    printf("statement completed\n\n");
  }

  sqlite3_finalize(stmt);
}

void insertTable(sqlite3* db) {
  sqlite3_stmt* stmt = nullptr;

  const string insertRow =
      R"(insert into students (age, name, score, class) values (?, ?, ?, ?);)";

  cout << "run stmt sql: " << insertRow << endl;

  if (sqlite3_prepare_v2(db, insertRow.c_str(), -1, &stmt, nullptr)) {
    cout << "error: Prepare stmt failed: " << sqlite3_errmsg(db) << endl;
    return;
  }

  int rc = 0;
  rc = sqlite3_bind_int64(stmt, 1, 23);
  rc = sqlite3_bind_text(stmt, 2, "Mike2", -1, SQLITE_STATIC);
  rc = sqlite3_bind_double(stmt, 3, 23);
  rc = sqlite3_bind_text(stmt, 4, "C", -1, SQLITE_STATIC);
  rc = sqlite3_step(stmt);

  cout << "sqlite3_step rc: " << rc << endl;

  if (SQLITE_DONE != rc) {
    fprintf(stderr, "statement didn't return DONE [%i]: %s\n", rc, sqlite3_errmsg(db));
  } else {
    printf("statement completed\n\n");
  }

  sqlite3_finalize(stmt);
}



int runStmt(const string& dbFile) {
  cout << "run sql=["
       << "] on db=[" << dbFile << "]" << endl;
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

  // deleteTable(db);
  // createTable(db);
  insertTable(db);
  seleteAllTable(db);

  sqlite3_close(db);
  return SQLITE_OK;
}

}  // namespace

void stmtExample(const string& dbFile) {
  std::cout << "stmtExample, begin" << std::endl;

  runStmt(dbFile);
  std::cout << "stmtExample, end" << std::endl;
}