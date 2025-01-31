#ifndef TEA_LIST_HPP
#define TEA_LIST_HPP

#include <gtkmm.h>

#include "tea_entry.hpp"

class TeaList : public Gtk::Box {
 public:
  TeaList();
  void add_tea(const Glib::ustring& name);

 private:
  Gtk::ScrolledWindow m_scrolledWindow;
  Gtk::ColumnView m_columnView;
  Glib::RefPtr<Gio::ListStore<TeaEntry>> m_listStore;
  Glib::RefPtr<Gtk::SingleSelection> m_selectionModel;

  void setup_columns();
  void load_tea_entries();
};

#endif