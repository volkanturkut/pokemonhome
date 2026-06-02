#include "bcat_injector.h"
#include <fstream>
#include <vector>
#include <switch.h>

bool BCATManager::InjectEvent(const std::string& wc9_path, const std::string& save_mount_name) {
    std::ifstream file(wc9_path, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        return false;
    }

    std::streamsize size = file.tellg();
    file.seekg(0, std::ios::beg);

    std::vector<char> buffer(size);
    if (!file.read(buffer.data(), size)) {
        return false;
    }

    FsFileSystem* fs = fsdevGetDeviceFileSystem(save_mount_name.c_str());
    if (fs == nullptr) {
        return false;
    }

    fsFsCreateDirectory(fs, "/pass");
    fsFsCreateDirectory(fs, "/pass/normal");

    fsFsDeleteFile(fs, "/pass/normal/distribution_internet");

    Result rc = fsFsCreateFile(fs, "/pass/normal/distribution_internet", buffer.size(), 0);
    if (R_FAILED(rc)) {
        return false;
    }

    FsFile fsFile;
    rc = fsFsOpenFile(fs, "/pass/normal/distribution_internet", FsOpenMode_Write, &fsFile);
    if (R_FAILED(rc)) {
        return false;
    }

    rc = fsFileWrite(&fsFile, 0, buffer.data(), buffer.size(), FsWriteOption_Flush);
    fsFileClose(&fsFile);

    return R_SUCCEEDED(rc);
}
