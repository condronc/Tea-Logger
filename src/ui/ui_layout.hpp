#ifndef UI_LAYOUT_HPP
#define UI_LAYOUT_HPP

#include <gtkmm/box.h>
#include <gtkmm/window.h>

class UiLayout {
 public:
  static void arrange_layout(Gtk::Window& window, Gtk::Box* main_box);
};

#endif