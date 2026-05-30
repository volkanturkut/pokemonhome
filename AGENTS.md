# Open Poke Home NX - Agent Instructions

## Tech Stack
- **Language**: C / C++ (C++20 preferred).
- **Framework**: `libnx` (switchbrew).
- **Build System**: standard devkitPro `Makefile` targeting `.nro`.

## Architecture & Inspirations
1. **Save Management (`/source/fs_utils.cpp`)**: Inspired by JKSV and Checkpoint. Use `fs_dev` to mount SystemSaveData and UserSaveData. 
2. **Pokemon Data (`/source/pkmn/`)**: Inspired by PKHeX (C#) and pkmn-chest (C++). Focus on Gen 8 (SwSh/BDSP/PLA) and Gen 9 (SV) `.pk8` and `.pk9` struct parsing.
3. **Event Injection (`/source/events/`)**: Inspired by Switch-Gift-Data-Manager and EventsGallery. Needs to inject Wondercards into BCAT / save blocks.
4. **RAM/Live Features (`/source/sys/`)**: Inspired by sys-botbase and ldn_mitm. Future scope includes reading/writing active game memory.

## Development Rules
- Use `switch-examples` (like `graphics/simple/`) as the baseline for UI loop and input handling.
- Whenever writing a new module, ensure you add it to the `Makefile` if necessary, and use `#TODO` comments for unimplemented logic.

## Build Instructions (CRITICAL)
Do not attempt to install devkitPro or libnx directly on the host machine using pacman or curl. The host IP is blocked by devkitPro's Cloudflare. 

Instead, you MUST use the official devkitPro Docker image to compile the project. Whenever you need to build the `.nro` file, run the following command in the terminal from the root of the repository:

`sudo docker run --rm -v $(pwd):/src devkitpro/devkita64 make`

If you need to run `make clean`, use:
`sudo docker run --rm -v $(pwd):/src devkitpro/devkita64 make clean`