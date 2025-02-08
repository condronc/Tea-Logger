#include "tea_list.hpp"

#include <iostream>

TeaList::TeaList() : Gtk::Box(Gtk::Orientation::VERTICAL) {
  m_listStore = Gio::ListStore<TeaEntry>::create();
  m_selectionModel = Gtk::SingleSelection::create(m_listStore);

  initialize_ui();
}

void TeaList::initialize_ui() {
  setup_columns();
  load_tea_entries();

  auto list_box = Gtk::Box(Gtk::Orientation::VERTICAL);
  list_box.append(m_scrolledWindow);
  list_box.set_vexpand(true);
  auto button_box = Gtk::Box(Gtk::Orientation::HORIZONTAL);

  auto m_logButton = Gtk::make_managed<Gtk::Button>("Log");
  auto m_addButton = Gtk::make_managed<Gtk::Button>("Add");
  auto m_editButton = Gtk::make_managed<Gtk::Button>("Edit");
  auto m_deleteButton = Gtk::make_managed<Gtk::Button>("Delete");

  button_box.append(*m_logButton);
  button_box.append(*m_addButton);
  button_box.append(*m_editButton);
  button_box.append(*m_deleteButton);

  append(button_box);
  append(list_box);

  m_logButton->signal_clicked().connect(
      sigc::mem_fun(*this, &TeaList::on_log_clicked));
  m_addButton->signal_clicked().connect(
      sigc::mem_fun(*this, &TeaList::on_add_clicked));
  m_editButton->signal_clicked().connect(
      sigc::mem_fun(*this, &TeaList::on_edit_clicked));
  m_deleteButton->signal_clicked().connect(
      sigc::mem_fun(*this, &TeaList::on_delete_clicked));
}

void TeaList::setup_columns() {
  auto name_factory = create_column_factory(
      [](const Glib::RefPtr<TeaEntry>& item) { return item->get_tea_name(); });

  auto date_factory =
      create_column_factory([](const Glib::RefPtr<TeaEntry>& item) {
        return item->get_local_time();
      });

  auto name_column = Gtk::ColumnViewColumn::create("Tea Name", name_factory);
  auto date_column = Gtk::ColumnViewColumn::create("Date Logged", date_factory);

  name_column->set_expand(true);
  date_column->set_expand(true);

  m_columnView.append_column(name_column);
  m_columnView.append_column(date_column);
  m_columnView.set_vexpand(true);
  m_columnView.set_hexpand(true);
  m_columnView.set_model(m_selectionModel);

  m_scrolledWindow.set_child(m_columnView);
  m_scrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC,
                              Gtk::PolicyType::AUTOMATIC);
  m_scrolledWindow.set_vexpand(true);
}

void TeaList::load_tea_entries() {
  m_listStore->splice(0, m_listStore->get_n_items(), {});

  std::vector<Glib::RefPtr<TeaEntry>> test_entries = {
      TeaEntry::create(1, "Green Tea", "2025-01-31 12:00"),
      TeaEntry::create(2, "Black Tea", "2025-01-31 13:00"),
      TeaEntry::create(3, "Oolong Tea", "2025-01-31 14:00")};

  if (!test_entries.empty()) {
    m_listStore->splice(m_listStore->get_n_items(), 0, test_entries);
  }
}

void TeaList::add_tea(const Glib::ustring& name) {
  int new_id = m_listStore->get_n_items() + 1;
  auto new_entry = TeaEntry::create(new_id, name, "2025-02-03");
  m_listStore->append(new_entry);
}

Glib::RefPtr<Gtk::SignalListItemFactory> TeaList::create_column_factory(
    std::function<Glib::ustring(const Glib::RefPtr<TeaEntry>&)> get_text_func) {
  auto factory = Gtk::SignalListItemFactory::create();

  factory->signal_setup().connect(
      [](const Glib::RefPtr<Gtk::ListItem>& list_item) {
        list_item->set_child(*Gtk::make_managed<Gtk::Label>(""));
      });

  factory->signal_bind().connect(
      [get_text_func](const Glib::RefPtr<Gtk::ListItem>& list_item) {
        auto item = std::dynamic_pointer_cast<TeaEntry>(list_item->get_item());
        if (item) {
          auto label = dynamic_cast<Gtk::Label*>(list_item->get_child());
          if (label) {
            label->set_text(get_text_func(item));
          }
        }
      });

  return factory;
}

void TeaList::on_log_clicked() {
  std::cout << "Log button clicked!" << std::endl;
}

void TeaList::on_add_clicked() {
  std::cout << "Add button clicked!" << std::endl;
}

void TeaList::on_edit_clicked() {
  std::cout << "Edit button clicked!" << std::endl;
}

void TeaList::on_delete_clicked() {
  std::cout << "Delete button clicked!" << std::endl;
}
