#ifndef GTKMM_TEA_LOGGER_HPP
#define GTKMM_TEA_LOGGER_HPP

#include <gtkmm/button.h>
#include <gtkmm/entry.h>
#include <gtkmm/window.h>
#include <sqlite3.h>

class TeaLogger : public Gtk::Window {
 public:
  TeaLogger();
  ~TeaLogger() override;

 protected:
  void on_button_clicked();
  bool open_db();
  void log_tea(const std::string& tea_name);
  Gtk::Button m_button;
  Gtk::Entry m_entry;
  sqlite3* db;
};

#endif