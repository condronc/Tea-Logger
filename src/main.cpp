#include <gtkmm/application.h>

#include <iostream>

#include "app.hpp"
// Reference from Gtkmm
int main(int argc, char* argv[]) {
  try {
    auto app = Gtk::Application::create("tea.logger");

    return app->make_window_and_run<App>(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}