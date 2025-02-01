#ifndef HELPER_HPP
#define HELPER_HPP

#include <gtkmm.h>

#include "../models/tea_list.hpp"

class Helper {
 public:
  static void initialize_window(Gtk::ApplicationWindow& window);
  static void setup_layout(Gtk::Paned& mainPaned, Gtk::Box& sidebarBox,
                           Gtk::ScrolledWindow& contentArea);
  static void connect_signals(Gtk::Button& profileButton,
                              Gtk::Button& teaButton,
                              Gtk::ScrolledWindow& contentArea,
                              TeaList& teaList);
};

#endif