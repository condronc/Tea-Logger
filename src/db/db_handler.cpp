#include "db_handler.hpp"

#include <iostream>

/// @brief Attempts to open the SQLite database
/// @param db_path
SQLiteDB::SQLiteDB(const std::string& db_path) {
  if (sqlite3_open(db_path.c_str(), &db) != SQLITE_OK) {
    throw std::runtime_error("Failed to open database: " +
                             std::string(sqlite3_errmsg(db)));
  }
}

/// @brief Checks for connection if db is not null (only closes if a connection
/// was made)
SQLiteDB::~SQLiteDB() {
  if (db) {
    sqlite3_close(db);
    db = nullptr;
  }
}

// the pointer is not const, but the method is!
// it wont modify the db object.
sqlite3* SQLiteDB::get() const { return db; }

/// @brief Creates a database if one does not exist
/// @param db_path
TeaDatabase::TeaDatabase(const std::string& db_path) : db(db_path) {
  execute_sql(R"(
      CREATE TABLE IF NOT EXISTS tea_database (
          id INTEGER PRIMARY KEY AUTOINCREMENT,
          tea_name TEXT NOT NULL,
          local_time DATE DEFAULT (datetime('now', 'localtime')),
          utc_time DATE DEFAULT (datetime('now', 'utc'))
      );
  )");
}

/// @brief executes SQL against the databse - parameter binding.
/// @param sql
/// @param params
/// @return results
std::vector<TeaLogEntry> TeaDatabase::execute_query(
    const std::string& sql, const std::vector<std::string>& params) {
  std::vector<TeaLogEntry> results;
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
    int id = sqlite3_column_int(stmt, 0);
    const char* tea_name = (const char*)sqlite3_column_text(stmt, 1);
    const char* local_time = (const char*)sqlite3_column_text(stmt, 2);
    const char* utc_time = (const char*)sqlite3_column_text(stmt, 3);

    results.emplace_back(id, tea_name ? tea_name : "",
                         local_time ? local_time : "",
                         utc_time ? utc_time : "");
  }

  sqlite3_finalize(stmt);
  return results;
}

/// @brief executes the SQL against the database - non parameter binding. It is
/// used when the database is first created.
/// @param sql
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

/// @brief Logs a tea
/// @param tea_name
/// @return if the function fails return false, otherwise true
bool TeaDatabase::log_tea(const std::string& tea_name) {
  const char* sql = "INSERT INTO tea_database (tea_name) VALUES (?);";
  sqlite3_stmt* stmt = nullptr;

  if (sqlite3_prepare_v2(db.get(), sql, -1, &stmt, nullptr) != SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db.get()) << std::endl;
    return false;
  }

  sqlite3_bind_text(stmt, 1, tea_name.c_str(), -1, SQLITE_STATIC);

  if (sqlite3_step(stmt) != SQLITE_DONE) {
    std::cerr << "Log failed: " << sqlite3_errstr(sqlite3_errcode(db.get()))
              << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

/// @brief Deletes a tea
/// @param tea_name
/// @return if the function fails return false, otherwise true
bool TeaDatabase::delete_tea(const std::string& tea_name) {
  const char* sql = "DELETE FROM tea_database WHERE tea_name = ?;";
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

/// @brief  finds tea entries based on the parameter
/// @param search_Term
/// @return entries
std::vector<TeaLogEntry> TeaDatabase::find_tea_entries(
    const std::string& search_Term) {
  std::vector<TeaLogEntry> entries;
  std::string sql =
      "SELECT id, tea_name, local_time, utc_time FROM tea_database";
  std::vector<std::string> params;

  if (!search_Term.empty()) {
    sql += " WHERE tea_name LIKE ?";
    params.push_back("%" + search_Term + "%");
  }

  sql += " ORDER BY tea_name ASC";

  auto query_results = execute_query(sql, params);
  for (const auto& row : query_results) {
    TeaLogEntry entry(row.id, row.tea_name, row.local_time, row.utc_time);
    entries.push_back(entry);
  }

  return entries;
}

/// @brief updates the database for editing
/// @param tea_id
/// @param new_name
void TeaDatabase::update_tea_name(int tea_id, const std::string& new_name) {
  try {
    sqlite3_stmt* stmt;
    const char* query = "UPDATE tea_database SET tea_name = ? WHERE id = ?";
    if (sqlite3_prepare_v2(db.get(), query, -1, &stmt, nullptr) != SQLITE_OK) {
      throw std::runtime_error("Failed to prepare SQL statement");
    }

    sqlite3_bind_text(stmt, 1, new_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, tea_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
      throw std::runtime_error("Failed to update tea name");
    }

    sqlite3_finalize(stmt);
  } catch (const std::exception& e) {
    std::cerr << "Error updating tea name in database: " << e.what()
              << std::endl;
  }
}
