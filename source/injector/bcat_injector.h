#pragma once

#include <string>

class BCATManager {
public:
    static bool InjectEvent(const std::string& wc9_path, const std::string& save_mount_name);
};
