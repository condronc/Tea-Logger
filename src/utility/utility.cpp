#include "utility.hpp"

#include <gtkmm/treeview.h>

#include "../app.hpp"

/// @brief connects signals for ui elements in the app class
/// @param app the app object to conenct signals for
void Utility::connect_signals(App& app) {
  app.m_logButton.signal_clicked().connect(
      sigc::mem_fun(app, &App::on_log_button_clicked));
  app.m_deleteButton.signal_clicked().connect(
      sigc::mem_fun(app, &App::on_delete_button_clicked));
  app.m_searchEntry.signal_changed().connect(
      sigc::mem_fun(app, &App::on_search_changed));
  app.m_editButton.signal_clicked().connect(
      sigc::mem_fun(app, &App::on_edit_button_clicked));

  app.m_toggleButton.signal_clicked().connect(
      sigc::mem_fun(app, &App::on_toggle_button_clicked));
}

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
