#ifndef GTKMM_TEA_LOGGER_HPP
#define GTKMM_TEA_LOGGER_HPP

#include <gtkmm/button.h>
#include <gtkmm/window.h>

class TeaLogger : public Gtk::Window {
 public:
  TeaLogger();
  ~TeaLogger() override;

 protected:
  void on_button_clicked();
  Gtk::Button m_button;
};

#endif