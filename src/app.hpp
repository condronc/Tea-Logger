#ifndef APP_HPP
#define APP_HPP

#include <gtkmm.h>

class AppWindow;

class App : public Gtk::Application {
 protected:
  App();

 public:
  static Glib::RefPtr<App> create();

 protected:
  void on_activate() override;
  void on_open(const Gio::Application::type_vec_files& files,
               const Glib::ustring& hint) override;

 private:
  AppWindow* create_appwindow();
};

#endif