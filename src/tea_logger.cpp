#include "tea_logger.hpp"

#include <gtkmm.h>

#include <iostream>
#include <map>
#include <vector>

#include "db/db_handler.hpp"

TeaLogger::~TeaLogger() = default;

/// @brief populates the TreeView of all current items in the data base or by
/// search terms
/// @param search_Term
void TeaLogger::PopulateTreeview(const std::string& search_Term) {
  m_refTreeModel->clear();
  std::string sql = "SELECT id, tea_name, date_logged FROM tea_log";
  std::vector<std::string> params;

  if (!search_Term.empty()) {
    sql += " WHERE tea_name LIKE ?";
    params.push_back("%" + search_Term + "%");
  }
  sql += " ORDER BY tea_name ASC";
  try {
    std::vector<std::map<std::string, std::string>> query_results =
        teadatabase.execute_query(sql, params);
    if (query_results.empty()) {
      return;
    }

    for (const auto& row : query_results) {
      Gtk::TreeModel::Row treeRow = *(m_refTreeModel->append());
      try {
        treeRow[m_colID] = std::stoi(row.at("id"));
        treeRow[m_colName] = row.at("tea_name");
        treeRow[m_colDate] = row.at("date_logged");
      } catch (const std::out_of_range& oor) {
        std::cerr << "Error accessing database field: " << oor.what()
                  << std::endl;
        continue;
      } catch (const std::invalid_argument& ia) {
        std::cerr << "Error converting ID: " << ia.what() << std::endl;
        continue;
      }
    }
  } catch (const std::exception& e) {
    std::cerr << "Error executing database query: " << e.what() << std::endl;
  }
}

/// @brief Buttons used by the application
TeaLogger::TeaLogger()
    : m_logButton("Log Tea"),
      m_deleteButton("Delete Tea"),
      m_searchEntry(),
      teadatabase("tea_log.db") {
  m_logButton.set_margin(10);
  m_searchEntry.set_margin(10);
  m_deleteButton.set_margin(10);
  m_treeView.set_margin(10);
  m_entry.set_margin(10);
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
  scrolledWindow->set_expand(true);
  scrolledWindow->set_child(m_treeView);

  auto box = std::make_unique<Gtk::Box>(Gtk::Orientation::VERTICAL);
  box->append(m_entry);
  box->append(m_searchEntry);
  box->append(m_logButton);
  box->append(m_deleteButton);
  box->append(*scrolledWindow);

  box->set_expand(false);

  set_child(*box);

  m_logButton.signal_clicked().connect(
      sigc::mem_fun(*this, &TeaLogger::on_log_button_clicked));
  m_deleteButton.signal_clicked().connect(
      sigc::mem_fun(*this, &TeaLogger::on_delete_button_clicked));
  m_searchEntry.signal_changed().connect(
      sigc::mem_fun(*this, &TeaLogger::on_search_changed));

  TeaDatabase teadatabase("tea_log.db");

  PopulateTreeview("");
  show();
}

/// @brief logs an entry
void TeaLogger::on_log_button_clicked() {
  std::string tea_name = m_entry.get_text();
  if (!tea_name.empty()) {
    teadatabase.log_tea(tea_name);
    std::cout << "Logged tea: " << tea_name << std::endl;
    m_entry.set_text("");
    PopulateTreeview();
  } else {
    std::cerr << "No tea name entered!" << std::endl;
  }
}

/// @brief delete function
void TeaLogger::on_delete_button_clicked() {
  std::string tea_name = m_entry.get_text();
  if (!tea_name.empty()) {
    teadatabase.delete_tea(tea_name);
    std::cout << "Attempted to delete tea: " << tea_name << std::endl;
    PopulateTreeview();
  } else {
    std::cerr << "No tea name entered!" << std::endl;
  }
}

/// @brief populates the listbox with the search
void TeaLogger::on_search_changed() {
  std::string search_Term = m_searchEntry.get_text();
  if (search_Term.empty()) {
    PopulateTreeview("");
  } else {
    PopulateTreeview(search_Term);
  }
}