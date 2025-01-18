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

SQLiteDB::~SQLiteDB() {
  if (db) {
    sqlite3_close(db);
  }
}

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

sqlite3_stmt* TeaDatabase::prepare_statement(const std::string& sql) {
  sqlite3_stmt* stmt = nullptr;
  if (sqlite3_prepare_v2(db.get(), sql.c_str(), -1, &stmt, nullptr) !=
      SQLITE_OK) {
    throw std::runtime_error("SQL prepare error: " +
                             std::string(sqlite3_errmsg(db.get())));
  }
  return stmt;
}

void TeaDatabase::finalize_statement(sqlite3_stmt* stmt) {
  if (stmt) {
    sqlite3_finalize(stmt);
  }
}

/// @brief executes SQL against the databse - parameter binding.
/// @param sql
/// @param params
/// @return results
std::vector<TeaLogEntry> TeaDatabase::execute_query(
    const std::string& sql, const std::vector<std::string>& params) {
  std::vector<TeaLogEntry> results;
  sqlite3_stmt* stmt = prepare_statement(sql);

  for (size_t i = 0; i < params.size(); ++i) {
    sqlite3_bind_text(stmt, i + 1, params[i].c_str(), -1, SQLITE_STATIC);
  }

  while (sqlite3_step(stmt) == SQLITE_ROW) {
    results.emplace_back(
        sqlite3_column_int(stmt, 0),
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1)) ?: "",
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)) ?: "",
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3)) ?: "");
  }

  finalize_statement(stmt);
  return results;
}

/// @brief executes the SQL against the database - non parameter binding. It is
/// used when the database is first created.
/// @param sql
void TeaDatabase::execute_sql(const std::string& sql) {
  char* errMessage = nullptr;
  int result =
      sqlite3_exec(db.get(), sql.c_str(), nullptr, nullptr, &errMessage);
  if (result != SQLITE_OK) {
    std::string error_msg =
        "SQL execution failed: " +
        std::string(errMessage ? errMessage : sqlite3_errmsg(db.get()));
    sqlite3_free(errMessage);
    throw std::runtime_error(error_msg);
  }
}

/// @brief Logs a tea
/// @param tea_name
/// @return if the function fails return false, otherwise true
bool TeaDatabase::log_tea(const std::string& tea_name) {
  const std::string sql = "INSERT INTO tea_database (tea_name) VALUES (?);";
  sqlite3_stmt* stmt = prepare_statement(sql);
  sqlite3_bind_text(stmt, 1, tea_name.c_str(), -1, SQLITE_STATIC);

  bool success = sqlite3_step(stmt) == SQLITE_DONE;
  if (!success) {
    std::cerr << "Log failed: " << sqlite3_errmsg(db.get()) << std::endl;
  }
  finalize_statement(stmt);
  return success;
}

/// @brief Deletes a tea
/// @param tea_name
/// @return if the function fails return false, otherwise true
bool TeaDatabase::delete_tea(const std::string& tea_name) {
  const std::string sql = "DELETE FROM tea_database WHERE tea_name = ?;";
  sqlite3_stmt* stmt = prepare_statement(sql);
  sqlite3_bind_text(stmt, 1, tea_name.c_str(), -1, SQLITE_STATIC);

  bool success = sqlite3_step(stmt) == SQLITE_DONE;
  if (!success) {
    std::cerr << "Delete failed: " << sqlite3_errmsg(db.get()) << std::endl;
  }
  finalize_statement(stmt);
  return success;
}

/// @brief updates the database for editing
/// @param tea_id
/// @param new_name
void TeaDatabase::update_tea_name(int tea_id, const std::string& new_name) {
  try {
    const std::string sql = "UPDATE tea_database SET tea_name = ? WHERE id = ?";
    sqlite3_stmt* stmt = prepare_statement(sql);
    sqlite3_bind_text(stmt, 1, new_name.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, tea_id);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
      throw std::runtime_error("Failed to update tea name");
    }
    finalize_statement(stmt);
  } catch (const std::exception& e) {
    std::cerr << "Error updating tea name: " << e.what() << std::endl;
  }
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