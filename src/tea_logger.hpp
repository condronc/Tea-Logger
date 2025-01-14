#ifndef GTKMM_TEA_LOGGER_HPP
#define GTKMM_TEA_LOGGER_HPP

#include <glibmm/refptr.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/listbox.h>
#include <gtkmm/liststore.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>
#include <sqlite3.h>

class TeaLogger : public Gtk::Window {
 public:
  TeaLogger();
  ~TeaLogger() override;

 protected:
  bool open_db();

  void on_log_button_clicked();
  void on_search_changed();
  void on_delete_button_clicked();

  void PopulateTreeview(const std::string& searchTerm = "");
  bool log_tea(const std::string& tea_name);
  bool delete_tea(const std::string& tea_name);

  sqlite3* db;

  Gtk::Button m_logButton;
  Gtk::Button m_deleteButton;
  Gtk::Entry m_searchEntry;
  Gtk::Entry m_entry;

  Gtk::TreeView m_treeView;
  Gtk::ListBox m_listBox;
  Glib::RefPtr<Gtk::ListStore> m_refTreeModel;
  Gtk::TreeModelColumn<int> m_colID;
  Gtk::TreeModelColumn<Glib::ustring> m_colName;
  Gtk::TreeModelColumn<Glib::ustring> m_colDate;
  Gtk::TreeModelColumnRecord m_Columns;
};

#endif