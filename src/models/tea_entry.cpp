#include "tea_entry.hpp"

TeaEntry::TeaEntry(int id, const Glib::ustring& name, const Glib::ustring& date)
    : Glib::ObjectBase("TeaEntry"),
      m_id(*this, "id", id),
      m_name(*this, "name", name),
      m_date(*this, "date", date) {}

Glib::RefPtr<TeaEntry> TeaEntry::create(int id, const Glib::ustring& name,
                                        const Glib::ustring& date) {
  return Glib::make_refptr_for_instance<TeaEntry>(new TeaEntry(id, name, date));
}