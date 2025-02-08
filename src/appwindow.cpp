#include "appwindow.hpp"

AppWindow::AppWindow() : m_profileButton("Profile"), m_teaButton("Tea Log") {
  Helper::initialize_window(*this);

  m_sidebarBox.append(m_profileButton);
  m_sidebarBox.append(m_teaButton);

  Helper::setup_layout(m_mainPaned, m_sidebarBox, m_contentArea);
  set_child(m_mainPaned);

  m_contentArea.set_child(m_teaList);
  setup_signals();
}

void AppWindow::on_profile_button_clicked() {
  m_profileLabel = Gtk::make_managed<Gtk::Label>("Profile Page");
  m_contentArea.set_child(*m_profileLabel);
}

void AppWindow::on_tea_button_clicked() {
  if (m_contentArea.get_child() != &m_teaList) {
    m_contentArea.set_child(m_teaList);
  }
}

void AppWindow::setup_signals() {
  auto connect = [this](auto& widget, auto signal, auto handler) {
    (widget.*signal)().connect(sigc::mem_fun(*this, handler));
  };
  connect(m_profileButton, &Gtk::Button::signal_clicked,
          &AppWindow::on_profile_button_clicked);
  connect(m_teaButton, &Gtk::Button::signal_clicked,
          &AppWindow::on_tea_button_clicked);
}