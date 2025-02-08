#include "helper.hpp"

void Helper::initialize_window(Gtk::ApplicationWindow& window) {
  window.set_title("Tea Logger");
  window.set_default_size(800, 600);
}

void Helper::setup_layout(Gtk::Paned& mainPaned, Gtk::Box& sidebarBox,
                          Gtk::ScrolledWindow& contentArea) {
  mainPaned.set_start_child(sidebarBox);
  mainPaned.set_end_child(contentArea);
  mainPaned.set_position(200);
}