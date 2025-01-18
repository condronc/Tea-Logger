#ifndef APP_HPP
#define APP_HPP

#include <glibmm/refptr.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/liststore.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>
#include <sqlite3.h>

#include "db/db_handler.hpp"

class App : public Gtk::Window {
 public:
  App();
  ~App() override;

 protected:
  TeaDatabase teadatabase;

  // Adds tea entries to the treeview model.
  // void AddEntriesToTree(const std::vector<TeaLogEntry>& entries);

  void on_toggle_button_clicked();
  void on_log_button_clicked();
  void on_edit_button_clicked();
  void on_search_changed();
  void on_delete_button_clicked();

  void PopulateTreeview(const std::string& searchTerm = "");

  friend class Utility;
  Gtk::Box* m_sidePanel;
  Gtk::Box* m_teaContent;
  Gtk::Box* m_profileContent;

  Gtk::Button m_logButton, m_deleteButton, m_editButton, m_profileButton,
      m_teaButton, m_toggleButton;

  bool m_isPanelExpanded = true;

  Gtk::SearchEntry m_searchEntry;
  Gtk::Entry m_entry;

  Gtk::TreeView m_treeView;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;

  Gtk::TreeModelColumn<int> m_colID;
  Gtk::TreeModelColumn<std::string> m_colName, m_colLocal, m_colUtc;

  Gtk::TreeModelColumnRecord m_Columns;
};

#endif