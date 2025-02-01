#include "appwindow.hpp"

AppWindow::AppWindow() {
  Helper::initialize_window(*this);

  m_sidebarBox.append(m_profileButton);
  m_sidebarBox.append(m_teaButton);

  Helper::setup_layout(m_mainPaned, m_sidebarBox, m_contentArea);
  set_child(m_mainPaned);

  m_contentArea.set_child(m_teaList);

  Helper::connect_signals(m_profileButton, m_teaButton, m_contentArea,
                          m_teaList);
}

void AppWindow::on_profile_button_clicked() {
  auto profileLabel = Gtk::make_managed<Gtk::Label>("Profile Page");
  m_contentArea.set_child(*profileLabel);
}

void AppWindow::on_tea_button_clicked() {
  if (m_contentArea.get_child() != &m_teaList) {
    m_contentArea.set_child(m_teaList);
  }
}