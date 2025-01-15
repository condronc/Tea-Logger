#include <gtkmm/application.h>

#include "tea_logger.hpp"

// Reference from Gtkmm
int main(int argc, char* argv[]) {
  auto app = Gtk::Application::create("org.gtkmm.example");

  // Shows the window and returns when it is closed.
  return app->make_window_and_run<TeaLogger>(argc, argv);
}