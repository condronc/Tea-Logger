#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>

#include "models/tea_list.hpp"

class AppWindow : public Gtk::ApplicationWindow {
 public:
  AppWindow();

 private:
  Gtk::Paned m_mainPaned;
  Gtk::Box m_sidebarBox;
  Gtk::ScrolledWindow m_contentArea;

  Gtk::Button m_profileButton;
  Gtk::Button m_teaButton;
  TeaList m_teaList;

  void on_profile_button_clicked();
  void on_tea_button_clicked();
};

#endif