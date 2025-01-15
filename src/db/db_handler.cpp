#include "db_handler.hpp"

#include <iostream>

SQLiteDB::SQLiteDB(const std::string& db_path) {
  if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
    throw std::runtime_error("Failed to open database: " +
                             std::string(sqlite3_errmsg(db)));
  }
}

SQLiteDB::~SQLiteDB() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;
  }
}

// the pointer is not const, but the method is!
// it wont modify the db object.
sqlite3* SQLiteDB::get() const { return db; }

TeaDatabase::TeaDatabase(const std::string& db_path) : db(db_path) {
  execute_sql(R"(
      CREATE TABLE IF NOT EXISTS tea_log (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          tea_name TEXT NOT NULL,
          local_time DATE DEFAULT (datetime('now', 'localtime')),
          utc_time DATE DEFAULT (datetime('now', 'utc'))
      );
  )");
}

std::vector<std::map<std::string, std::string>> TeaDatabase::execute_query(
    const std::string& sql, const std::vector<std::string>& params) {
  std::vector<std::map<std::string, std::string>> results;
  sqlite3_stmt* stmt;

  if (sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &stmt, nullptr) !=
      SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db.get()) << std::endl;
    return results;
  }

  for (size_t i = 0; i < params.size(); ++i) {
    sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    std::map<std::string, std::string> row;
    int cols = sqlite3_column_count(stmt);
    for (int i = 0; i < cols; ++i) {
      const char* name = sqlite3_column_name(stmt, i);
      const char* value = (const char*)sqlite3_column_text(stmt, i);
      row[name] = value ? value : "";
    }
    results.push_back(row);
  }

  sqlite3_finalize(stmt);
  return results;
}

void TeaDatabase::execute_sql(const std::string& sql) {
  char* errMessage = nullptr;
  int return_code =
      sqlite3_exec(db.get(), sql.c_str(), nullptr, nullptr, &errMessage);
  if (return_code != SQLITE_OK) {
    std::string error_msg = "SQL execution failed: ";
    if (errMessage) {
      error_msg += errMessage;
      sqlite3_free(errMessage);
    } else {
      error_msg += std::string(sqlite3_errmsg(db.get()));
    }
    throw std::runtime_error(error_msg);
  }
}

bool TeaDatabase::log_tea(const std::string& tea_name) {
  const char* sql = "INSERT INTO tea_log (tea_name) VALUES (?);";
  sqlite3_stmt* stmt = nullptr;

  int return_code = sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr);
  if (return_code != SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db.get()) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, tea_name.c_str(), -1, SQLITE_STATIC);

  int step_result = sqlite3_step(stmt);
  if (step_result != SQLITE_DONE && step_result != SQLITE_ROW) {
    std::cerr << "Step failed: " << sqlite3_errstr(sqlite3_errcode(db.get()))
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

bool TeaDatabase::delete_tea(const std::string& tea_name) {
  const char* sql = "DELETE FROM tea_log WHERE tea_name = ?;";
  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db.get()) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, tea_name.c_str(), -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Delete failed: " << sqlite3_errstr(sqlite3_errcode(db.get()))
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}