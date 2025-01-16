#include "tea.hpp"

/// @brief Constructs a TeaLogEntry with the given ID, name, and timestamps
/// @param entryId
/// @param name
/// @param local
/// @param utc
TeaLogEntry::TeaLogEntry(int entryId, const std::string& name,
                         const std::string& local, const std::string& utc)
    : id(entryId), tea_name(name), local_time(local), utc_time(utc) {}

/// @brief constructs a default TeaLogEntry with empty values
TeaLogEntry::TeaLogEntry()
    : id(0), tea_name(""), local_time(""), utc_time("") {}
