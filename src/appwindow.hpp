#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>

#include "helpers/helper.hpp"
#include "models/tea_list.hpp"

class AppWindow : public Gtk::ApplicationWindow {
 private:
  Gtk::Paned m_mainPaned{Gtk::Orientation::HORIZONTAL};
  Gtk::Box m_sidebarBox{Gtk::Orientation::VERTICAL};
  Gtk::Button m_profileButton{"Profile"};
  Gtk::Button m_teaButton{"Tea Log"};

  TeaList m_teaList;
  Gtk::ScrolledWindow m_contentArea;

  void on_profile_button_clicked();
  void on_tea_button_clicked();

 public:
  AppWindow();
};

#endif