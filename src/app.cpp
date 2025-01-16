#include "app.hpp"

#include <gdkmm/display.h>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stacksidebar.h>

#include <iostream>
#include <map>
#include <vector>

#include "db/db_handler.hpp"
#include "models/tea.hpp"
#include "ui/ui_elements.hpp"
#include "ui/ui_layout.hpp"
#include "ui/ui_style.hpp"
#include "utility/signal_utility.hpp"

App::~App() = default;

App::App()
    : m_logButton("Log Tea"),
      m_deleteButton("Delete Tea"),
      m_searchEntry(),
      teadatabase("tea_log.db") {
  UiStyle::initialize_styling();

  UiElements ui_elements;

  ui_elements.setup_treeview(m_treeView, m_refTreeModel, m_colID, m_colName,
                             m_colLocal, m_colUtc);
  Gtk::Box* sidebar = ui_elements.create_sidebar(m_entry, m_searchEntry,
                                                 m_logButton, m_deleteButton);
  Gtk::Box* main_content = ui_elements.create_main_content(m_treeView);
  Gtk::Box* main_box = ui_elements.create_main_box(sidebar, main_content);

  // sets up layout
  UiLayout::arrange_layout(*this, main_box);

  // connect signals
  Utility::connect_signals(*this);

  PopulateTreeview("");
  show();
}

/// @brief populates the TreeView of all current items in the data base or by
/// search terms
/// @param search_Term
void App::PopulateTreeview(const std::string& searchTerm) {
  m_refTreeModel->clear();
  try {
    std::vector<TeaLogEntry> query_results =
        teadatabase.get_all_entries(searchTerm);
    if (query_results.empty()) {
      return;
    }
    for (const auto& entry : query_results) {
      Gtk::TreeModel::Row treeRow = *(m_refTreeModel->append());
      treeRow[m_colID] = entry.id;
      treeRow[m_colName] = entry.tea_name;
      treeRow[m_colLocal] = entry.local_time;
      treeRow[m_colUtc] = entry.utc_time;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error executing database query: " << e.what() << std::endl;
  }
}

/// @brief uses the log_tea function
void App::on_log_button_clicked() {
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

/// @brief uses the delete_tea function
void App::on_delete_button_clicked() {
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
void App::on_search_changed() {
  std::string search_Term = m_searchEntry.get_text();
  if (search_Term.empty()) {
    PopulateTreeview("");
  } else {
    PopulateTreeview(search_Term);
  }
}