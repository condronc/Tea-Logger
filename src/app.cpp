
#include "app.hpp"

#include <gtkmm.h>

#include <iostream>
#include <vector>

#include "appwindow.hpp"

App::App()
    : Gtk::Application("tea.logger", Gio::Application::Flags::HANDLES_OPEN) {}
Glib::RefPtr<App> App::create() {
  return Glib::make_refptr_for_instance<App>(new App());
}
AppWindow* App::create_appwindow() {
  auto appwindow = Gtk::make_managed<AppWindow>();
  add_window(*appwindow);

  appwindow->signal_hide().connect(
      [this, appwindow]() { std::cout << "Window is hidden." << std::endl; });
  return appwindow;
}

void App::on_activate() {
  try {
    auto appwindow = create_appwindow();
    appwindow->present();
  } catch (const Glib::Error& e) {
    std::cerr << e.what() << std::endl;
  } catch (const std::exception& e) {
    std::cerr << e.what() << std::endl;
  }
}

void App::on_open(const Gio::Application::type_vec_files& files,
                  const Glib::ustring& hint) {
  AppWindow* appwindow = nullptr;
  auto windows = get_windows();
  if (windows.size() > 0) {
    appwindow = dynamic_cast<AppWindow*>(windows[0]);
  }
  if (!appwindow) {
    appwindow = create_appwindow();
  }
  for (const auto& file : files) {
    appwindow->open_file_view(file);
  }
  appwindow->present();
}