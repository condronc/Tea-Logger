#include "helper.hpp"

void Helper::initialize_window(Gtk::ApplicationWindow& window) {
  window.set_title("Tea Logger");
  window.set_default_size(800, 600);
}

void Helper::setup_layout(Gtk::Paned& mainPaned, Gtk::Box& sidebarBox,
                          Gtk::ScrolledWindow& contentArea) {
  mainPaned.set_start_child(sidebarBox);
  mainPaned.set_end_child(contentArea);
  mainPaned.set_position(200);
}

void Helper::connect_signals(Gtk::Button& profileButton, Gtk::Button& teaButton,
                             Gtk::ScrolledWindow& contentArea,
                             TeaList& teaList) {
  profileButton.signal_clicked().connect([&contentArea] {
    auto profileLabel = Gtk::make_managed<Gtk::Label>("Profile Page");
    contentArea.set_child(*profileLabel);
  });

  teaButton.signal_clicked().connect([&contentArea, &teaList] {
    if (contentArea.get_child() != &teaList) {
      contentArea.set_child(teaList);
    }
  });
}
