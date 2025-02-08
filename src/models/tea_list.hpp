#ifndef TEA_LIST_HPP
#define TEA_LIST_HPP

#include <gtkmm.h>

#include "tea_entry.hpp"

/// @brief TeaList contains a ScrolledWindow, Columnview, and two reference
/// points of a ListStore and a Single Selection.
/// It shows all the teas.
class TeaList : public Gtk::Box {
 private:
  Gtk::ScrolledWindow m_scrolledWindow;
  Gtk::ColumnView m_columnView;
  Glib::RefPtr<Gio::ListStore<TeaEntry>> m_listStore;
  Glib::RefPtr<Gtk::SingleSelection> m_selectionModel;
  void initialize_ui();
  void setup_columns();
  void load_tea_entries();
  void on_log_clicked();
  void on_add_clicked();
  void on_edit_clicked();
  void on_delete_clicked();
  Glib::RefPtr<Gtk::SignalListItemFactory> create_column_factory(
      std::function<Glib::ustring(const Glib::RefPtr<TeaEntry>&)>
          get_text_func);

 public:
  TeaList();

  void add_tea(const Glib::ustring& name);
};

#endif