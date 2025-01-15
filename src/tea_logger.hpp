#ifndef GTKMM_TEA_LOGGER_HPP
#define GTKMM_TEA_LOGGER_HPP

#include <glibmm/refptr.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/listbox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treemodelfilter.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>
#include <sqlite3.h>

#include "db/db_handler.hpp"

class TeaLogger : public Gtk::Window {
 public:
  TeaLogger();
  ~TeaLogger() override;

 protected:
  TeaDatabase teadatabase;
  void on_log_button_clicked();
  void on_search_changed();
  void on_delete_button_clicked();

  void PopulateTreeview(const std::string& searchTerm = "");

  Gtk::Button m_logButton, m_deleteButton;
  Gtk::SearchEntry m_searchEntry;
  Gtk::Entry m_entry;

  Gtk::TreeView m_treeView;
  Gtk::ListBox m_listBox;

  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
  Glib::RefPtr<Gtk::TreeModelFilter> m_refTreeFilter;
  Gtk::TreeModelColumn<int> m_colID;
  Gtk::TreeModelColumn<std::string> m_colName, m_colDate;
  Gtk::TreeModelColumnRecord m_Columns;
};

#endif