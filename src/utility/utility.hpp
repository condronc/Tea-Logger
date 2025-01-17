#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <gtkmm/button.h>
#include <gtkmm/liststore.h>
#include <gtkmm/searchentry.h>
#include <gtkmm/treeview.h>

#include <vector>

#include "../app.hpp"
#include "../db/db_handler.hpp"

/// @brief utility class for common helper functions
class Utility {
 public:
  static void connect_signals(App& app);

  static void AddEntriesToTree(
      const std::vector<TeaLogEntry>& entries,
      Glib::RefPtr<Gtk::ListStore> refTreeModel,
      const Gtk::TreeModelColumn<int>& colID,
      const Gtk::TreeModelColumn<std::string>& colName,
      const Gtk::TreeModelColumn<std::string>& colLocal,
      const Gtk::TreeModelColumn<std::string>& colUtc);
};

#endif
