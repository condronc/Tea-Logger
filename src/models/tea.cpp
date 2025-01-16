#include "tea.hpp"

TeaLogEntry::TeaLogEntry(int entryId, const std::string& name,
                         const std::string& local, const std::string& utc)
    : id(entryId), tea_name(name), local_time(local), utc_time(utc) {}

TeaLogEntry::TeaLogEntry()
    : id(0), tea_name(""), local_time(""), utc_time("") {}
