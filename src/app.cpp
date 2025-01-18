#include "app.hpp"

#include <gdkmm/display.h>
#include <glibmm.h>
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
#include "utility/utility.hpp"

App::~App() = default;

/// @brief constructor for the application
App::App() : teadatabase("tea_database.db"), m_isPanelExpanded(false) {
  // injects css
  UiStyle::initialize_styling();

  UiElements ui_elements;

  m_sidePanel = ui_elements.create_side_panel(m_profileButton, m_teaButton,
                                              m_toggleButton);

  m_sidePanel->get_style_context()->add_class("side-panel");

  if (m_isPanelExpanded) {
    m_sidePanel->get_style_context()->add_class("expanded");
  } else {
    m_sidePanel->get_style_context()->add_class("collapsed");
  }

  ui_elements.setup_treeview(m_treeView, m_refTreeModel, m_colID, m_colName,
                             m_colLocal, m_colUtc);

  Gtk::Box* tea_content =
      ui_elements.create_tea_content(m_entry, m_searchEntry, m_logButton,
                                     m_deleteButton, m_editButton, m_treeView);

  Gtk::Box* main_box = ui_elements.create_main_box(m_sidePanel, tea_content);

  UiLayout::arrange_layout(*this, main_box);
  Utility::connect_signals(*this);
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

    Utility::AddEntriesToTree(query_results, m_refTreeModel, m_colID, m_colName,
                              m_colLocal, m_colUtc);
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
  UiElements ui_elements;

  auto iter = m_treeView.get_selection()->get_selected();
  if (!iter) {
    std::cerr << "No tea selected for editing!" << std::endl;
    return;
  }

  int tea_id = (*iter)[m_colID];
  std::string tea_name = (*iter)[m_colName];

  Gtk::Window* edit_window = ui_elements.create_edit_window(
      tea_name, [this, tea_id, tea_name](const std::string& new_tea_name) {
        if (!new_tea_name.empty() && new_tea_name != tea_name) {
          try {
            teadatabase.update_tea_name(tea_id, new_tea_name);
            PopulateTreeview();
          } catch (const std::exception& e) {
            std::cerr << "Error updating tea name: " << e.what() << std::endl;
          }
        }
      });

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

void App::on_toggle_button_clicked() {
  UiElements ui_elements;
  ui_elements.toggle_side_panel(*m_sidePanel, m_toggleButton,
                                m_isPanelExpanded);
}

void App::replace_main_content(Gtk::Box* new_content) {
  Gtk::Box* main_box = dynamic_cast<Gtk::Box*>(get_children().front());

  if (!main_box) {
    std::cerr << "Error: Main box not found." << std::endl;
    return;
  }

  if (main_box->get_children().empty()) {
    std::cerr << "Error: No child widget found in the main box." << std::endl;
    return;
  }

  Gtk::Widget* old_content_ptr = main_box->get_children().back();

  main_box->remove(*old_content_ptr);

  main_box->append(*new_content);
}

void App::show_tea_content() {
  if (is_tea_content_shown) {
    return;
  }
  UiElements ui_elements;
  Gtk::Box* tea_content =
      ui_elements.create_tea_content(m_entry, m_searchEntry, m_logButton,
                                     m_deleteButton, m_editButton, m_treeView);

  replace_main_content(tea_content);

  current_content = tea_content;
  is_tea_content_shown = true;
}

void App::show_profile_content() {
  UiElements ui_elements;

  Gtk::Box* profile_content = ui_elements.create_profile_content();
  if (current_content != profile_content) {
    replace_main_content(profile_content);
    current_content = profile_content;
    is_tea_content_shown = false;
  }
}
