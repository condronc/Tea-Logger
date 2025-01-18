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
  ui_style.initialize_styling();

  m_sidePanel = ui_elements.create_side_panel(m_profileButton, m_teaButton,
                                              m_toggleButton);
  m_sidePanel->get_style_context()->add_class("side-panel");
  m_sidePanel->get_style_context()->add_class(m_isPanelExpanded ? "expanded"
                                                                : "collapsed");

  ui_elements.setup_treeview(m_treeView, m_refTreeModel, m_colID, m_colName,
                             m_colLocal, m_colUtc);

  Gtk::Box* tea_content =
      ui_elements.create_tea_content(m_entry, m_searchEntry, m_logButton,
                                     m_deleteButton, m_editButton, m_treeView);
  Gtk::Box* main_box = ui_elements.create_main_box(m_sidePanel, tea_content);

  ui_layout.arrange_layout(*this, main_box);
  connect_signals();
  PopulateTreeview("");
}

/// @brief populates the TreeView of all current items in the data base or by
/// search terms
/// @param search_Term
void App::PopulateTreeview(const std::string& searchTerm) {
  m_refTreeModel->clear();

  try {
    auto query_results = teadatabase.find_tea_entries(searchTerm);
    if (!query_results.empty()) {
      utility.AddEntriesToTree(query_results, m_refTreeModel, m_colID,
                               m_colName, m_colLocal, m_colUtc);
    }
  } catch (const std::exception& e) {
    std::cerr << "Error executing database query: " << e.what() << std::endl;
  }
}

/// @brief uses the log_tea function
void App::on_log_button_clicked() {
  const std::string tea_name = m_entry.get_text();
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
  auto iter = m_treeView.get_selection()->get_selected();
  if (!iter) {
    std::cerr << "No tea selected for editing!" << std::endl;
    return;
  }

  const int tea_id = (*iter)[m_colID];
  const std::string tea_name = (*iter)[m_colName];

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
  const std::string tea_name = m_entry.get_text();
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
  const std::string search_term = m_searchEntry.get_text();
  PopulateTreeview(search_term);
}

void App::on_toggle_button_clicked() {
  ui_elements.toggle_side_panel(*m_sidePanel, m_toggleButton,
                                m_isPanelExpanded);
}

void App::replace_main_content(Gtk::Box* new_content) {
  Gtk::Box* main_box = dynamic_cast<Gtk::Box*>(get_children().front());

  if (!main_box || main_box->get_children().empty()) {
    std::cerr << "Error: Main box or its child not found." << std::endl;
    return;
  }

  Gtk::Widget* old_content_ptr = main_box->get_children().back();
  main_box->remove(*old_content_ptr);
  main_box->append(*new_content);
}

void App::show_tea_content() {
  if (is_tea_content_shown) return;
  Gtk::Box* tea_content =
      ui_elements.create_tea_content(m_entry, m_searchEntry, m_logButton,
                                     m_deleteButton, m_editButton, m_treeView);

  replace_main_content(tea_content);
  current_content = tea_content;
  is_tea_content_shown = true;
}

void App::show_profile_content() {
  Gtk::Box* profile_content = ui_elements.create_profile_content();

  if (current_content != profile_content) {
    replace_main_content(profile_content);
    current_content = profile_content;
    is_tea_content_shown = false;
  }
}

void App::connect_signals() {
  m_logButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::on_log_button_clicked));
  m_deleteButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::on_delete_button_clicked));
  m_searchEntry.signal_changed().connect(
      sigc::mem_fun(*this, &App::on_search_changed));
  m_editButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::on_edit_button_clicked));
  m_toggleButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::on_toggle_button_clicked));
  m_profileButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::show_profile_content));
  m_teaButton.signal_clicked().connect(
      sigc::mem_fun(*this, &App::show_tea_content));
}