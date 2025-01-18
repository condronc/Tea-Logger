#include "ui_elements.hpp"

/// @brief default constructor
UiElements::UiElements() {}

/// @brief creates a vertical box for containing sidebar elements
/// @param entry
/// @param searchEntry
/// @param logButton
/// @param deleteButton
/// @return pointer to the created sidebar
Gtk::Box* UiElements::create_sidebar(Gtk::Entry& entry,
                                     Gtk::SearchEntry& searchEntry,
                                     Gtk::Button& logButton,
                                     Gtk::Button& deleteButton,
                                     Gtk::Button& editButton) {
  Gtk::Box* sidebar = Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 10));
  entry.set_placeholder_text("Enter tea...");
  logButton.set_label("Log Tea");
  deleteButton.set_label("Delete Tea");
  editButton.set_label("Edit tea");
  searchEntry.set_placeholder_text("Search tea...");
  sidebar->append(entry);
  sidebar->append(searchEntry);
  sidebar->append(logButton);
  sidebar->append(deleteButton);
  sidebar->append(editButton);

  return sidebar;
}

/// @brief creates the main content area containing the tree view
/// @param treeView
/// @return pointer to the created main content
Gtk::Box* UiElements::create_main_content(Gtk::TreeView& treeView) {
  Gtk::Box* main_content =
      Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 10));

  auto scrolledWindow = Gtk::manage(new Gtk::ScrolledWindow());
  scrolledWindow->set_expand(true);
  scrolledWindow->set_child(treeView);

  main_content->append(*scrolledWindow);
  return main_content;
}

/// @brief creates the main horizontal box containing the sidebar and main
/// content
/// @param sidebar
/// @param main_content
/// @return a pointer to the created main box
Gtk::Box* UiElements::create_main_box(Gtk::Box* side_panel, Gtk::Box* sidebar,
                                      Gtk::Box* main_content) {
  Gtk::Box* main_box =
      Gtk::manage(new Gtk::Box(Gtk::Orientation::HORIZONTAL, 10));
  main_box->set_margin(10);
  main_box->set_hexpand(true);

  sidebar->set_hexpand(false);
  main_content->set_hexpand(true);
  main_content->set_vexpand(true);

  main_box->append(*side_panel);
  main_box->append(*sidebar);
  main_box->append(*main_content);
  return main_box;
}

/// @brief sets up the tree view with columns and the refmodel
/// @param treeView
/// @param refTreeModel
/// @param colID
/// @param colName
/// @param colLocal
/// @param colUtc
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

Gtk::Window* UiElements::create_edit_window(
    const std::string& tea_name,
    std::function<void(const std::string&)> on_save) {
  auto edit_window = Gtk::make_managed<Gtk::Window>();
  edit_window->set_title("Edit Tea Name");
  edit_window->set_default_size(300, 150);

  auto vbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::VERTICAL, 10);
  edit_window->set_child(*vbox);

  auto label = Gtk::make_managed<Gtk::Label>("Enter new tea name:");
  vbox->append(*label);

  auto entry = Gtk::make_managed<Gtk::Entry>();
  entry->set_text(tea_name);
  vbox->append(*entry);

  auto hbox = Gtk::make_managed<Gtk::Box>(Gtk::Orientation::HORIZONTAL, 10);
  vbox->append(*hbox);

  auto cancel_button = Gtk::make_managed<Gtk::Button>("Cancel");
  cancel_button->signal_clicked().connect(
      [edit_window]() { edit_window->hide(); });
  hbox->append(*cancel_button);

  auto save_button = Gtk::make_managed<Gtk::Button>("Save");
  save_button->signal_clicked().connect([on_save, entry, edit_window]() {
    on_save(entry->get_text());
    edit_window->hide();
  });
  hbox->append(*save_button);

  return edit_window;
}

Gtk::Box* UiElements::create_side_panel(Gtk::Button& profileButton,
                                        Gtk::Button& cupButton,
                                        Gtk::Button& toggleButton) {
  Gtk::Box* sidePanel =
      Gtk::manage(new Gtk::Box(Gtk::Orientation::VERTICAL, 10));

  profileButton.set_image_from_icon_name("avatar-default-symbolic");

  cupButton.set_image_from_icon_name("emoji-nature-symbolic");
  toggleButton.set_image_from_icon_name("go-next-symbolic");

  sidePanel->append(profileButton);
  sidePanel->append(cupButton);
  sidePanel->append(toggleButton);

  return sidePanel;
}

void UiElements::toggle_side_panel(Gtk::Box& side_panel,
                                   Gtk::Button& toggle_button,
                                   bool& is_expanded) {
  side_panel.get_style_context()->add_class("side-panel");

  if (is_expanded) {
    side_panel.get_style_context()->remove_class("expanded");
    side_panel.get_style_context()->add_class("collapsed");
    toggle_button.set_icon_name("go-next-symbolic");
  } else {
    side_panel.get_style_context()->remove_class("collapsed");
    side_panel.get_style_context()->add_class("expanded");
    toggle_button.set_icon_name("go-previous-symbolic");
  }
  is_expanded = !is_expanded;
}