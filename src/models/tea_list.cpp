#include "tea_list.hpp"

TeaList::TeaList() : Gtk::Box(Gtk::Orientation::VERTICAL) {
  m_listStore = Gio::ListStore<TeaEntry>::create();
  m_selectionModel = Gtk::SingleSelection::create(m_listStore);

  initialize_ui();
}

void TeaList::initialize_ui() {
  setup_columns();
  load_tea_entries();

  m_columnView.set_vexpand(true);
  m_columnView.set_hexpand(true);
  m_columnView.set_model(m_selectionModel);

  m_scrolledWindow.set_child(m_columnView);
  m_scrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC,
                              Gtk::PolicyType::AUTOMATIC);
  m_scrolledWindow.set_vexpand(true);
  append(m_scrolledWindow);
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