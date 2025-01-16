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

App::~App() = default;

/// @brief populates the TreeView of all current items in the data base or by
/// search terms
/// @param search_Term
void App::PopulateTreeview(const std::string& search_Term) {
  m_refTreeModel->clear();

  try {
    std::vector<TeaLogEntry> query_results =
        teadatabase.get_all_entries(search_Term);
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

App::App()
    : m_logButton("Log Tea"),
      m_deleteButton("Delete Tea"),
      m_searchEntry(),
      teadatabase("tea_log.db") {
  Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
  css_provider->load_from_path("src/style.css");
  Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
  Gtk::StyleContext::add_provider_for_display(
      display, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

  Gtk::Box* sidebar = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 10));
  sidebar->append(m_entry);
  sidebar->append(m_searchEntry);
  sidebar->append(m_logButton);
  sidebar->append(m_deleteButton);

  Gtk::Box* main_content =
      Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 10));

  m_Columns.add(m_colID);
  m_Columns.add(m_colName);
  m_Columns.add(m_colLocal);
  m_Columns.add(m_colUtc);

  m_refTreeModel = Gtk::ListStore::create(m_Columns);
  m_treeView.set_model(m_refTreeModel);
  m_treeView.append_column("ID", m_colID);
  m_treeView.append_column("Name", m_colName);
  m_treeView.append_column("Local Date Logged", m_colLocal);
  m_treeView.append_column("UTC Date Logged", m_colUtc);

  auto scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  scrolledWindow->set_expand(true);
  scrolledWindow->set_child(m_treeView);

  main_content->append(*scrolledWindow);

  Gtk::Box* main_box =
      Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 10));
  main_box->set_margin(10);
  main_box->set_hexpand(true);

  sidebar->set_hexpand(false);
  main_content->set_hexpand(true);
  main_content->set_vexpand(true);

  main_box->append(*sidebar);
  main_box->append(*main_content);

  set_child(*main_box);

  m_logButton.set_margin(10);
  m_searchEntry.set_margin(10);
  m_deleteButton.set_margin(10);
  m_treeView.set_margin(10);
  m_entry.set_margin(10);
  m_entry.set_placeholder_text("Enter tea name...");
  m_searchEntry.set_placeholder_text("Search Term...");

  m_logButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::on_log_button_clicked));
  m_deleteButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::on_delete_button_clicked));
  m_searchEntry.signal_changed().connect(
      sigc::mem_fun(*this, &App::on_search_changed));

  PopulateTreeview("");
  show();
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