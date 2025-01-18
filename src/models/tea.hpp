#ifndef TEA_HPP
#define TEA_HPP

#include <string>

/// @brief represents an entry in the database
struct TeaLogEntry {
  int id;
  std::string tea_name;
  std::string local_time;
  std::string utc_time;

  TeaLogEntry(int entryId, const std::string& name, const std::string& local,
              const std::string& utc)
      : id(entryId), tea_name(name), local_time(local), utc_time(utc) {}

  TeaLogEntry() : id(0), tea_name(""), local_time(""), utc_time("") {}
};

#endif