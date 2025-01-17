#ifndef DB_HANDLER_HPP
#define DB_HANDLER_HPP

#include <sqlite3.h>

#include <vector>

#include "../models/tea.hpp"

/// @brief Handles the database connection
class SQLiteDB {
 public:
  SQLiteDB(const std::string& db_path);
  ~SQLiteDB();
  sqlite3* get() const;

 private:
  sqlite3* db = nullptr;
};

/// @brief Provides methods for interacting with the sqlite database
class TeaDatabase {
 public:
  TeaDatabase(const std::string& db_path);
  void execute_sql(const std::string& sql);
  bool log_tea(const std::string& tea_name);
  void update_tea_name(int tea_id, const std::string& new_name);
  bool delete_tea(const std::string& tea_name);

  std::vector<TeaLogEntry> execute_query(
      const std::string& sql, const std::vector<std::string>& params);
  std::vector<TeaLogEntry> find_tea_entries(const std::string& search_Term);

 private:
  SQLiteDB db;
};

#endif