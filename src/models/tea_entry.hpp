#ifndef TEA_ENTRY_HPP
#define TEA_ENTRY_HPP

#include <glibmm.h>

class TeaEntry : public Glib::Object {
 public:
  static Glib::RefPtr<TeaEntry> create(int id, const Glib::ustring& name,
                                       const Glib::ustring& date) {
    return Glib::make_refptr_for_instance<TeaEntry>(
        new TeaEntry(id, name, date));
  }

  int get_id() const { return m_id.get_value(); }
  Glib::ustring get_tea_name() const { return m_name.get_value(); }
  Glib::ustring get_local_time() const { return m_date.get_value(); }

 protected:
  TeaEntry(int id, const Glib::ustring& name, const Glib::ustring& date);

 private:
  Glib::Property<int> m_id;
  Glib::Property<Glib::ustring> m_name;
  Glib::Property<Glib::ustring> m_date;
};

#endif