#ifndef UI_ELEMENTS_HPP
#define UI_ELEMENTS_HPP

#include <glibmm.h>
#include <gtkmm/box.h>
#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/image.h>
#include <gtkmm/label.h>
#include <gtkmm/liststore.h>
#include <gtkmm/scrolledwindow.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treeview.h>
#include <gtkmm/window.h>

#include "../models/tea.hpp"

/// @brief class for creating and managing ui elements
class UiElements {
 public:
  UiElements();

  void toggle_side_panel(Gtk::Box& side_panel, Gtk::Button& toggle_button,
                         bool& is_expanded);

  Gtk::Box* create_side_panel(Gtk::Button& profileButton,
                              Gtk::Button& cupButton,
                              Gtk::Button& toggleButton);

  Gtk::Box* create_sidebar(Gtk::Entry& entry, Gtk::SearchEntry& searchEntry,
                           Gtk::Button& logButton, Gtk::Button& deleteButtonm,
                           Gtk::Button& editButton);

  Gtk::Box* create_main_content(Gtk::TreeView& treeView);

  Gtk::Box* create_main_box(Gtk::Box* side_panel, Gtk::Box* sidebar,
                            Gtk::Box* main_content);

  Gtk::Window* create_edit_window(
      const std::string& tea_name,
      std::function<void(const std::string&)> on_save);

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
