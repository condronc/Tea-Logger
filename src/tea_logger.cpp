#include "tea_logger.hpp"

#include <gtkmm.h>
#include <gtkmm/box.h>
#include <sqlite3.h>

#include <iostream>

TeaLogger::TeaLogger() : m_button("Log Tea") {
  m_button.set_margin(10);
  m_entry.set_placeholder_text("Enter tea name...");

  auto box = std::make_unique<Gtk::Box>(Gtk::Orientation::VERTICAL);
  box->append(m_entry);
  box->append(m_button);

  set_child(*box);

  m_button.signal_clicked().connect(
      sigc::mem_fun(*this, &TeaLogger::on_button_clicked));

  if (!open_db()) {
    std::cerr << "Failed to open the database!" << std::endl;
  }
}

TeaLogger::~TeaLogger() {
  if (db) {
    sqlite3_close(db);
  }
}

/// @brief Opens the SQLite3 database (or it creates one if it doesn't already
/// exist)
/// @return true/false
bool TeaLogger::open_db() {
  int rc = sqlite3_open("tea_log.db", &db);
  if (rc) {
    return false;
  }

  const char* create_table_sql = R"(
    CREATE TABLE IF NOT EXISTS tea_log (
      id INTEGER PRIMARY KEY AUTOINCREMENT,
      tea_name TEXT NOT NULL,
      date_logged TIMESTAMP DEFAULT CURRENT_TIMESTAMP
    );
  )";

  char* err_msg = nullptr;
  rc = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << err_msg << std::endl;
    sqlite3_free(err_msg);
    return false;
  }
  return true;
}

void TeaLogger::on_button_clicked() {
  std::string tea_name = m_entry.get_text();
  if (!tea_name.empty()) {
    log_tea(tea_name);
    std::cout << "Logged tea: " << tea_name << std::endl;
    m_entry.set_text("");  // Clear the input field
  } else {
    std::cerr << "No tea name entered!" << std::endl;
  }
}

/// @brief logs the tea into the database
/// @param tea_name
void TeaLogger::log_tea(const std::string& tea_name) {
  std::string sql =
      "INSERT INTO tea_log (tea_name) VALUES ('" + tea_name + "');";

  char* err_msg = nullptr;
  int rc = sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &err_msg);
  if (rc != SQLITE_OK) {
    std::cerr << "SQL error: " << err_msg << std::endl;
    sqlite3_free(err_msg);
  }
}