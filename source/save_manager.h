#pragma once

#include <switch.h>
#include <string>

class SaveManager {
public:
    SaveManager();
    ~SaveManager();

    bool mountSave(u64 titleId, AccountUid uid);
    bool copyFileToSD(const std::string& sourcePath, const std::string& destPath);
    void unmountSave();

private:
    bool isMounted;
};
