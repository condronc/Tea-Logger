#ifndef UI_STYLE_HPP
#define UI_STYLE_HPP

#include <gdkmm/display.h>
#include <glibmm/refptr.h>
#include <gtkmm/cssprovider.h>
#include <gtkmm/stylecontext.h>

class UiStyle {
 public:
  static void initialize_styling();
};

#endif