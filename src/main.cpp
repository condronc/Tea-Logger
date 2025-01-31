#include <gtkmm/application.h>

#include <iostream>

#include "app.hpp"

int main(int argc, char* argv[]) {
  try {
    auto application = App::create();

    return application->run(argc, argv);
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
}