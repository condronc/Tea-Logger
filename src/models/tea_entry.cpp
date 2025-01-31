#include "tea_entry.hpp"

TeaEntry::TeaEntry(int id, const Glib::ustring& name, const Glib::ustring& date)
    : Glib::ObjectBase("TeaEntry"),
      m_id(*this, "id", id),
      m_name(*this, "name", name),
      m_date(*this, "date", date) {}