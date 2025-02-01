#ifndef APP_HPP
#define APP_HPP

#include <gtkmm.h>

class AppWindow;

class App : public Gtk::Application {
 public:
  static Glib::RefPtr<App> create();

 protected:
  App();
  void on_activate() override;

 private:
  AppWindow* create_appwindow();
};

#endif