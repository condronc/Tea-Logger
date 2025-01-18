#include "utility.hpp"

#include <gtkmm/treeview.h>

/// @brief adds entries to the treeview, intended purpose is searching
/// @param entries
/// @param refTreeModel
/// @param colID
/// @param colName
/// @param colLocal
/// @param colUtc
void Utility::AddEntriesToTree(
    const std::vector<TeaLogEntry>& entries,
    Glib::RefPtr<Gtk::ListStore> refTreeModel,
    const Gtk::TreeModelColumn<int>& colID,
    const Gtk::TreeModelColumn<std::string>& colName,
    const Gtk::TreeModelColumn<std::string>& colLocal,
    const Gtk::TreeModelColumn<std::string>& colUtc) {
  for (const auto& entry : entries) {
    Gtk::TreeModel::Row treeRow = *(refTreeModel->append());
    treeRow[colID] = entry.id;
    treeRow[colName] = entry.tea_name;
    treeRow[colLocal] = entry.local_time;
    treeRow[colUtc] = entry.utc_time;
  }
}
