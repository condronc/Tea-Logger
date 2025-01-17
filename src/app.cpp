#include "app.hpp"

#include <gdkmm/display.h>
#include <gtkmm.h>
#include <gtkmm/box.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/dialog.h>
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

/// @brief constructor for the application
App::App()
    : m_logButton("Log Tea"),
      m_deleteButton("Delete Tea"),
      m_searchEntry(),
      m_editButton("Edit Tea"),
      teadatabase("tea_database.db") {
  // injects css
  UiStyle::initialize_styling();

  UiElements ui_elements;
  // sets up the tree view
  ui_elements.setup_treeview(m_treeView, m_refTreeModel, m_colID, m_colName,
                             m_colLocal, m_colUtc);
  // creates ui elements
  Gtk::Box* sidebar = ui_elements.create_sidebar(
      m_entry, m_searchEntry, m_logButton, m_deleteButton, m_editButton);
  Gtk::Box* main_content = ui_elements.create_main_content(m_treeView);
  Gtk::Box* main_box = ui_elements.create_main_box(sidebar, main_content);

  // sets up layout
  UiLayout::arrange_layout(*this, main_box);

  // connect signals
  Utility::connect_signals(*this);

  // populate the tree view with all tea entries shown
  PopulateTreeview("");
}

/// @brief populates the TreeView of all current items in the data base or by
/// search terms
/// @param search_Term
void App::PopulateTreeview(const std::string& searchTerm) {
  m_refTreeModel->clear();
  try {
    std::vector<TeaLogEntry> query_results =
        teadatabase.find_tea_entries(searchTerm);
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

/// @brief edits an entry based on selecting an item
void App::on_edit_button_clicked() {
  // get row from the treeview
  Gtk::TreeModel::iterator iter = m_treeView.get_selection()->get_selected();
  if (!iter) {
    std::cerr << "No tea selected for editing!" << std::endl;
    return;
  }

  // get the id from the row and the name
  int tea_id = (*iter)[m_colID];
  std::string tea_name = (*iter)[m_colName];

  Gtk::Window* edit_window = new Gtk::Window();
  edit_window->set_title("Edit Tea Name");
  edit_window->set_modal(true);  // not needed?

  edit_window->set_default_size(300, 150);

  // make a container for packing the widgets into a single row
  Gtk::Box* vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL);
  edit_window->set_child(*vbox);

  // lavel for the vbox
  Gtk::Label* label = Gtk::make_managed<Gtk::Label>("Enter new tea name:");
  vbox->append(*label);

  // fill the entry of the vbox with the selected entry
  Gtk::Entry* entry = Gtk::make_managed<Gtk::Entry>();
  entry->set_text(tea_name);
  vbox->append(*entry);

  // container for the bottons
  Gtk::Box* hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL);
  vbox->append(*hbox);

  Gtk::Button* cancel_button = Gtk::make_managed<Gtk::Button>("Cancel");
  cancel_button->signal_clicked().connect(
      [edit_window]() { edit_window->close(); });
  hbox->append(*cancel_button);

  Gtk::Button* save_button = Gtk::make_managed<Gtk::Button>("Save");
  save_button->signal_clicked().connect(
      [this, tea_id, tea_name, entry, edit_window]() {
        std::string new_tea_name = entry->get_text();
        if (!new_tea_name.empty() && new_tea_name != tea_name) {
          try {
            teadatabase.update_tea_name(tea_id, new_tea_name);
            std::cout << "Updated tea name: " << new_tea_name << std::endl;
            std::cout << "Editing tea with ID: " << tea_id
                      << ", current name: " << tea_name << std::endl;
            PopulateTreeview();  // Refresh tree view
          } catch (const std::exception& e) {
            std::cerr << "Error updating tea name: " << e.what() << std::endl;
          }
        }
        edit_window->close();
      });
  hbox->append(*save_button);

  edit_window->show();
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