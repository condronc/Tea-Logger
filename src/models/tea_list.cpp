#include "tea_list.hpp"

TeaList::TeaList()
    : Gtk::Box(Gtk::Orientation::VERTICAL),
      m_listStore(Gio::ListStore<TeaEntry>::create()),
      m_selectionModel(Gtk::SingleSelection::create(m_listStore)) {
  setup_columns();

  m_columnView.set_model(m_selectionModel);
  m_scrolledWindow.set_child(m_columnView);
  m_scrolledWindow.set_policy(Gtk::PolicyType::AUTOMATIC,
                              Gtk::PolicyType::AUTOMATIC);
  append(m_scrolledWindow);
}

void TeaList::setup_columns() {
  auto name_factory =
      create_factory([](const Glib::RefPtr<Gtk::ListItem>& list_item) {
        auto item = std::dynamic_pointer_cast<TeaEntry>(list_item->get_item());
        if (item) {
          auto label = Gtk::make_managed<Gtk::Label>(item->get_name());
          list_item->set_child(*label);
        }
      });

  auto date_factory =
      create_factory([](const Glib::RefPtr<Gtk::ListItem>& list_item) {
        auto item = std::dynamic_pointer_cast<TeaEntry>(list_item->get_item());
        if (item) {
          auto label = Gtk::make_managed<Gtk::Label>(item->get_date());
          list_item->set_child(*label);
        }
      });

  auto name_column = Gtk::ColumnViewColumn::create("Tea Name", name_factory);
  auto date_column = Gtk::ColumnViewColumn::create("Date Logged", date_factory);

  m_columnView.append_column(name_column);
  m_columnView.append_column(date_column);
}

void TeaList::add_tea(int id, const Glib::ustring& name,
                      const Glib::ustring& date) {
  auto tea_entry =
      Glib::make_refptr_for_instance<TeaEntry>(new TeaEntry(id, name, date));
  m_listStore->append(tea_entry);
}

Glib::RefPtr<Gtk::SignalListItemFactory> TeaList::create_factory(
    std::function<void(const Glib::RefPtr<Gtk::ListItem>&)> bind_func) {
  auto factory = Gtk::SignalListItemFactory::create();
  factory->signal_bind().connect(bind_func);
  return factory;
}
