#include "signal_utility.hpp"

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
}