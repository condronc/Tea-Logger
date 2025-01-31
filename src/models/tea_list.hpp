#ifndef TEA_LIST_HPP
#define TEA_LIST_HPP

#include <gtkmm.h>

#include "tea_entry.hpp"

class TeaList : public Gtk::Box {
 public:
  TeaList();
  void add_tea(int id, const Glib::ustring& name, const Glib::ustring& date);

 private:
  Gtk::ScrolledWindow m_scrolledWindow;
  Gtk::ColumnView m_columnView;

  Glib::RefPtr<Gio::ListStore<TeaEntry>> m_listStore;
  Glib::RefPtr<Gtk::SingleSelection> m_selectionModel;

  Glib::RefPtr<Gtk::SignalListItemFactory> create_factory(
      std::function<void(const Glib::RefPtr<Gtk::ListItem>&)> bind_func);

  void setup_columns();
};

#endif