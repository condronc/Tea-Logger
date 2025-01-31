#ifndef TEA_ENTRY_HPP
#define TEA_ENTRY_HPP

#include <glibmm.h>

class TeaEntry : public Glib::Object {
 public:
  TeaEntry(int id, const Glib::ustring& name, const Glib::ustring& date);

  int get_id() const { return m_id.get_value(); }
  Glib::ustring get_name() const { return m_name.get_value(); }
  Glib::ustring get_date() const { return m_date.get_value(); }

  Glib::Property<int> m_id;
  Glib::Property<Glib::ustring> m_name;
  Glib::Property<Glib::ustring> m_date;

 protected:
  TeaEntry();
};

#endif