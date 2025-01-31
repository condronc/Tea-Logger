#include "appwindow.hpp"

AppWindow::AppWindow()
    : m_mainPaned(Gtk::Orientation::HORIZONTAL),
      m_sidebarBox(Gtk::Orientation::VERTICAL),
      m_profileButton("Profile"),
      m_teaButton("Tea Log") {
  set_title("Tea Logger");
  set_default_size(800, 600);

  m_sidebarBox.append(m_profileButton);
  m_sidebarBox.append(m_teaButton);

  m_contentArea.set_policy(Gtk::PolicyType::AUTOMATIC,
                           Gtk::PolicyType::AUTOMATIC);
  m_mainPaned.set_start_child(m_sidebarBox);
  m_mainPaned.set_end_child(m_contentArea);
  m_mainPaned.set_position(200);

  set_child(m_mainPaned);

  m_profileButton.signal_clicked().connect(
      sigc::mem_fun(*this, &AppWindow::on_profile_button_clicked));
  m_teaButton.signal_clicked().connect(
      sigc::mem_fun(*this, &AppWindow::on_tea_button_clicked));
}

void AppWindow::on_profile_button_clicked() {
  auto profileLabel = Gtk::make_managed<Gtk::Label>("Profile Page");
  m_contentArea.set_child(*profileLabel);
}

void AppWindow::on_tea_button_clicked() { m_contentArea.set_child(m_teaList); }

void AppWindow::open_file_view(const Glib::RefPtr<Gio::File>& /* file */) {}
