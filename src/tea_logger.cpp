#include "tea_logger.hpp"

#include <glibmm/refptr.h>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/liststore.h>
#include <sqlite3.h>

#include <iostream>

enum class TeaColumns : int { ID, NAME, DATE, NUM_COLUMNS };

struct Tea {
  int id;
  std::string name;
};

void TeaLogger::PopulateTreeview(const std::string& search_Term) {
  m_refTreeModel->clear();

  std::string sql = "SELECT id, tea_name, date_logged FROM tea_log";
  if (!search_Term.empty()) {
    sql += " WHERE tea_name LIKE ?";
  }
  sql += " ORDER BY tea_name ASC";

  // DEBUG
  std::cout << "SQL Query: " << sql << std::endl;

  sqlite3_stmt* stmt;
  int return_code = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);

  if (return_code != SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db)
              << " (Error code: " << return_code << ")" << std::endl;
    sqlite3_finalize(stmt);
    return;
  }

  if (!search_Term.empty()) {
    std::string likesearch_Term = "%" + search_Term + "%";
    return_code =
        sqlite3_bind_text(stmt, 1, likesearch_Term.c_str(), -1, SQLITE_STATIC);
    if (return_code != SQLITE_OK) {
      std::cerr << "Bind failed: " << sqlite3_errmsg(db)
                << " (Error code: " << return_code << ")" << std::endl;
      sqlite3_finalize(stmt);
      return;
    }
  }

  while ((return_code = sqlite3_step(stmt)) == SQLITE_ROW) {
    Gtk::TreeModel::Row row = *(m_refTreeModel->append());
    row[m_colID] = sqlite3_column_int(stmt, 0);

    const char* name_text =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
    if (name_text) {
      row[m_colName] = std::string(name_text);
    } else {
      row[m_colName] = std::string("");
    }

    const char* date_text =
        reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
    if (date_text) {
      row[m_colDate] = std::string(date_text);
    } else {
      row[m_colDate] = std::string("");
    }
  }

  if (return_code != SQLITE_DONE) {
    std::cerr << "Step failed: " << sqlite3_errmsg(db)
              << " (Error code: " << return_code << ")" << std::endl;
  }
  sqlite3_finalize(stmt);
}

/// @brief Buttons used by the application
TeaLogger::TeaLogger()
    : m_logButton("Log Tea"), m_deleteButton("Delete Tea"), m_searchEntry() {
  m_logButton.set_margin(10);
  m_entry.set_placeholder_text("Enter tea name...");
  m_searchEntry.set_placeholder_text("Search Term...");

  m_Columns.add(m_colID);
  m_Columns.add(m_colName);
  m_Columns.add(m_colDate);

  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_treeView.set_model(m_refTreeModel);

  m_treeView.append_column("ID", m_colID);
  m_treeView.append_column("Name", m_colName);
  m_treeView.append_column("Date Logged", m_colDate);

  auto scrolledWindow = std::make_unique<Gtk::ScrolledWindow>();
  scrolledWindow->set_child(m_treeView);

  auto box = std::make_unique<Gtk::Box>(Gtk::Orientation::VERTICAL);
  box->append(m_entry);
  box->append(m_searchEntry);
  box->append(m_logButton);
  box->append(m_deleteButton);
  box->append(*scrolledWindow);

  set_child(*box);

  m_logButton.signal_clicked().connect(
      sigc::mem_fun(*this, &TeaLogger::on_log_button_clicked));
  m_deleteButton.signal_clicked().connect(
      sigc::mem_fun(*this, &TeaLogger::on_delete_button_clicked));
  m_searchEntry.signal_changed().connect(
      sigc::mem_fun(*this, &TeaLogger::on_search_changed));

  if (!open_db()) {
    std::cerr << "Failed to open the database!" << std::endl;
  }

  PopulateTreeview("");
  show();
}

// this just closes the db when the application closes.
TeaLogger::~TeaLogger() {
  if (db) {
    sqlite3_close(db);
  }
}

/// @brief Opens the SQLite3 database (or it creates one if it doesn't already
/// exist)
/// @return
bool TeaLogger::open_db() {
  int return_code = sqlite3_open("tea_log.db", &db);
  if (return_code) {
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
  return_code = sqlite3_exec(db, create_table_sql, nullptr, nullptr, &err_msg);
  if (return_code != SQLITE_OK) {
    std::cerr << "SQL error: " << err_msg << std::endl;
    sqlite3_free(err_msg);
    return false;
  }
  return true;
}

/// @brief logs an entry
void TeaLogger::on_log_button_clicked() {
  std::string tea_name = m_entry.get_text();
  if (!tea_name.empty()) {
    log_tea(tea_name);
    std::cout << "Logged tea: " << tea_name << std::endl;
    m_entry.set_text("");
  } else {
    std::cerr << "No tea name entered!" << std::endl;
  }
}

void TeaLogger::on_delete_button_clicked() {
  std::string tea_name = m_entry.get_text();
  if (!tea_name.empty()) {
    delete_tea(tea_name);
    std::cout << "Attempted to delete tea: " << tea_name << std::endl;
    PopulateTreeview();
  } else {
    std::cerr << "No tea name entered!" << std::endl;
  }
}

/// @brief logs the tea into the database
/// @param tea_name
bool TeaLogger::log_tea(const std::string& tea_name) {
  const char* sql = "INSERT INTO tea_log (tea_name) VALUES (?);";
  sqlite3_stmt* stmt;
  int return_code = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (return_code != SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
    return false;
  }

  return_code = sqlite3_bind_text(stmt, 1, tea_name.c_str(), -1, SQLITE_STATIC);
  if (return_code != SQLITE_OK) {
    std::cerr << "Bind failed: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  return_code = sqlite3_step(stmt);
  if (return_code != SQLITE_DONE) {
    std::cerr << "Step failed: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

// deletes a tea from the database
bool TeaLogger::delete_tea(const std::string& tea_name) {
  const char* sql = "DELETE FROM tea_log WHERE tea_name = ?;";
  sqlite3_stmt* stmt;
  int return_code = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);

  if (return_code != SQLITE_OK) {
    std::cerr << "Prepare failed: " << sqlite3_errmsg(db) << std::endl;
    return false;
  }

  return_code = sqlite3_bind_text(stmt, 1, tea_name.c_str(), -1, SQLITE_STATIC);
  if (return_code != SQLITE_OK) {
    std::cerr << "Bind failed: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  return_code = sqlite3_step(stmt);

  if (return_code != SQLITE_DONE) {
    std::cerr << "Step failed: " << sqlite3_errmsg(db) << std::endl;
    sqlite3_finalize(stmt);
    return false;
  }

  sqlite3_finalize(stmt);
  return true;
}

/// @brief populates the listbox with the search
void TeaLogger::on_search_changed() {
  std::string search_Term = m_searchEntry.get_text();
  PopulateTreeview(search_Term);
}