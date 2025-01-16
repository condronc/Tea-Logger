#include "ui_layout.hpp"

void UiLayout::arrange_layout(Gtk::Window& window, Gtk::Box* main_box) {
  main_box->set_margin(10);
  main_box->set_hexpand(true);

  window.set_child(*main_box);
}