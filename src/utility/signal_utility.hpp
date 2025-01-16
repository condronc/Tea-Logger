#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <gtkmm/button.h>
#include <gtkmm/searchentry.h>

#include "../app.hpp"

/// @brief utility class for common helper functions
class Utility {
 public:
  static void connect_signals(App& app);
};

#endif
