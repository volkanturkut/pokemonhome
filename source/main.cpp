// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

#include "save_manager.h"
#include "injector/bcat_injector.h"

// Main program entrypoint
int main(int argc, char* argv[])
{
    // This example uses a text console, as a simple way to output text to the screen.
    // If you want to write a software-rendered graphics application,
    //   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
    // If on the other hand you want to write an OpenGL based application,
    //   take a look at the graphics/opengl set of examples, which uses EGL instead.
    consoleInit(NULL);

    // Configure our supported input layout: a single player with standard controller styles
    padConfigureInput(1, HidNpadStyleSet_NpadStandard);

    // Initialize the default gamepad (which reads handheld mode inputs as well as the first connected controller)
    PadState pad;
    padInitializeDefault(&pad);

    // Other initialization goes here. As a demonstration, we print hello world.
    printf("Open Poke Home NX\n");

    Result rc = accountInitialize(AccountServiceType_Application);
    if (R_FAILED(rc)) {
        printf("accountInitialize() failed: 0x%x\n", rc);
    }

    AccountUid uid={0};
    if (R_SUCCEEDED(rc)) {
        rc = accountGetPreselectedUser(&uid);
        if (R_FAILED(rc)) {
            printf("accountGetPreselectedUser() failed: 0x%x\n", rc);
        }
    }

    mkdir("sdmc:/PokeHome", 0777);
    mkdir("sdmc:/PokeHome/backups", 0777);

    SaveManager saveManager;
    u64 titleId = 0x0100ABF008968000; // Pokemon Sword

    printf("Press A to backup Pokemon Sword save.\n");
    printf("Press X to Inject Event.\n");
    printf("Press + to exit.\n");

    // Main loop
    while (appletMainLoop())
    {
        // Scan the gamepad. This should be done once for each frame
        padUpdate(&pad);

        // padGetButtonsDown returns the set of buttons that have been
        // newly pressed in this frame compared to the previous one
        u64 kDown = padGetButtonsDown(&pad);

        if (kDown & HidNpadButton_Plus)
            break; // break in order to return to hbmenu

        if (kDown & HidNpadButton_X) {
            u64 titleIdSV = 0x0100A3D008C5C000;
            printf("Attempting to inject event for Title ID: 0x%016lx\n", titleIdSV);

            if (saveManager.mountSave(titleIdSV, uid)) {
                if (BCATManager::InjectEvent("sdmc:/PokeHome/events/test.wc9", "save")) {
                    printf("Injection successful.\n");
                } else {
                    printf("Injection failed.\n");
                }
                saveManager.unmountSave();
            } else {
                printf("Failed to mount save.\n");
            }
        }

        if (kDown & HidNpadButton_A) {
            printf("Attempting to backup save for Title ID: 0x%016lx\n", titleId);

            if (saveManager.mountSave(titleId, uid)) {
                printf("Save mounted successfully.\n");

                std::string sourceFile = "save:/main";
                std::string destFile = "sdmc:/PokeHome/backups/main";

                if (saveManager.copyFileToSD(sourceFile, destFile)) {
                    printf("Backup successful to %s\n", destFile.c_str());
                } else {
                    printf("Backup failed.\n");
                }

                saveManager.unmountSave();
                printf("Save unmounted.\n");
            } else {
                printf("Failed to mount save.\n");
            }
        }

        // Update the console, sending a new frame to the display
        consoleUpdate(NULL);
    }

    accountExit();

    // Deinitialize and clean up resources used by the console (important!)
    consoleExit(NULL);
    return 0;
}
