#include <iostream>
#include <string>

extern "C" {
#include "sqlite3.h"
};

using std::cout;
using std::endl;
using std::string;


extern int createTable(const string& dbFile);
extern int deleteTable(const string& dbFile);
extern void execExample(const string& dbFile);
extern void stmtExample(const string& dbFile);



int main() {
  std::cout << "hello, sqlite3" << std::endl;
  string dbFile = "../../../test1.db";

  //execExample(dbFile);
  stmtExample(dbFile);

  // createTable(dbFile);
  // deleteTable(dbFile);
  return 0;
}