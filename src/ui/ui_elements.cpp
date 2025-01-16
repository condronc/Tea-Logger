#include "ui_elements.hpp"

UiElements::UiElements() {}

Gtk::Box* UiElements::create_sidebar(Gtk::Entry& entry,
                                     Gtk::SearchEntry& searchEntry,
                                     Gtk::Button& logButton,
                                     Gtk::Button& deleteButton) {
  Gtk::Box* sidebar = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 10));
  sidebar->append(entry);
  sidebar->append(searchEntry);
  sidebar->append(logButton);
  sidebar->append(deleteButton);
  return sidebar;
}

Gtk::Box* UiElements::create_main_content(Gtk::TreeView& treeView) {
  Gtk::Box* main_content =
      Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 10));

  auto scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  scrolledWindow->set_expand(true);
  scrolledWindow->set_child(treeView);

  main_content->append(*scrolledWindow);
  return main_content;
}

Gtk::Box* UiElements::create_main_box(Gtk::Box* sidebar,
                                      Gtk::Box* main_content) {
  Gtk::Box* main_box =
      Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 10));
  main_box->set_margin(10);
  main_box->set_hexpand(true);

  sidebar->set_hexpand(false);
  main_content->set_hexpand(true);
  main_content->set_vexpand(true);

  main_box->append(*sidebar);
  main_box->append(*main_content);
  return main_box;
}

void UiElements::setup_treeview(Gtk::TreeView& treeView,
                                Glib::RefPtr<Gtk::ListStore>& refTreeModel,
                                Gtk::TreeModelColumn<int>& colID,
                                Gtk::TreeModelColumn<std::string>& colName,
                                Gtk::TreeModelColumn<std::string>& colLocal,
                                Gtk::TreeModelColumn<std::string>& colUtc) {
  m_Columns.add(colID);
  m_Columns.add(colName);
  m_Columns.add(colLocal);
  m_Columns.add(colUtc);

  refTreeModel = Gtk::ListStore::create(m_Columns);

  treeView.set_model(refTreeModel);

  treeView.append_column("ID", colID);
  treeView.append_column("Name", colName);
  treeView.append_column("Local Time", colLocal);
  treeView.append_column("UTC Time", colUtc);
}