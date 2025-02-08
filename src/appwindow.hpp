#ifndef APPWINDOW_HPP
#define APPWINDOW_HPP

#include <gtkmm.h>

#include "helpers/helper.hpp"
#include "models/tea_list.hpp"

class AppWindow : public Gtk::ApplicationWindow {
 private:
  Gtk::Paned m_mainPaned{Gtk::Orientation::HORIZONTAL};
  Gtk::Box m_sidebarBox{Gtk::Orientation::VERTICAL};
  Gtk::Button m_profileButton;
  Gtk::Button m_teaButton;

  TeaList m_teaList;
  Gtk::ScrolledWindow m_contentArea;

  Gtk::Label* m_profileLabel{nullptr};

  void on_profile_button_clicked();
  void on_tea_button_clicked();
  void setup_signals();

 public:
  AppWindow();
};

#endif