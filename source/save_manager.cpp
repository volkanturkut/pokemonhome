#include "save_manager.h"
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>

SaveManager::SaveManager() : isMounted(false) {}

SaveManager::~SaveManager() {
    unmountSave();
}

bool SaveManager::mountSave(u64 titleId, AccountUid uid) {
    if (isMounted) {
        unmountSave();
    }

    Result rc = fsdevMountSaveData("save", titleId, uid);
    if (R_SUCCEEDED(rc)) {
        isMounted = true;
        return true;
    } else {
        printf("fsdevMountSaveData() failed: 0x%x\n", rc);
        return false;
    }
}

bool SaveManager::copyFileToSD(const std::string& sourcePath, const std::string& destPath) {
    if (!isMounted) {
        printf("Cannot copy: save is not mounted.\n");
        return false;
    }

    FILE* src = fopen(sourcePath.c_str(), "rb");
    if (!src) {
        printf("Failed to open source file: %s\n", sourcePath.c_str());
        return false;
    }

    FILE* dest = fopen(destPath.c_str(), "wb");
    if (!dest) {
        printf("Failed to open dest file: %s\n", destPath.c_str());
        fclose(src);
        return false;
    }

    char buffer[8192];
    size_t bytesRead;

    while ((bytesRead = fread(buffer, 1, sizeof(buffer), src)) > 0) {
        fwrite(buffer, 1, bytesRead, dest);
    }

    fclose(src);
    fclose(dest);
    return true;
}

void SaveManager::unmountSave() {
    if (isMounted) {
        fsdevUnmountDevice("save");
        isMounted = false;
    }
}
