#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treeview.h>

class UiElements {
 public:
  UiElements();

  Gtk::Box* create_sidebar(Gtk::Entry& entry, Gtk::SearchEntry& searchEntry,
                           Gtk::Button& logButton, Gtk::Button& deleteButton);

  Gtk::Box* create_main_content(Gtk::TreeView& treeView);

  Gtk::Box* create_main_box(Gtk::Box* sidebar, Gtk::Box* main_content);

  void setup_treeview(Gtk::TreeView& treeView,
                      Glib::RefPtr<Gtk::ListStore>& refTreeModel,
                      Gtk::TreeModelColumn<int>& colID,
                      Gtk::TreeModelColumn<std::string>& colName,
                      Gtk::TreeModelColumn<std::string>& colLocal,
                      Gtk::TreeModelColumn<std::string>& colUtc);

 private:
  Gtk::TreeModelColumn<int> m_colID;
  Gtk::TreeModelColumn<std::string> m_colName;
  Gtk::TreeModelColumn<std::string> m_colLocal;
  Gtk::TreeModelColumn<std::string> m_colUtc;
  Gtk::TreeModelColumnRecord m_Columns;
};

#endif
