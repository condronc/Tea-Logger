#include "ui_style.hpp"

/// @brief injects css into the layout
void UiStyle::initialize_styling() {
  Glib::RefPtr<Gtk::CssProvider> css_provider = Gtk::CssProvider::create();
  css_provider->load_from_path("src/style.css");
  Glib::RefPtr<Gdk::Display> display = Gdk::Display::get_default();
  Gtk::StyleContext::add_provider_for_display(
      display, css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
}