# Audio Plugin Template (JUCE + CMake)
BASED ON [JanWilczek/audio-plugin-template](https://github.com/JanWilczek/audio-plugin-template)

This is a C++ audio plugin project using the [JUCE framework](https://juce.com/), set up with:
- Modern CMake
- Headless unit testing support
- Manual control via `Makefile`
- Linux (Ubuntu) as the primary development environment

---

## Dependencies

Install required system packages:

```bash
sudo apt update
sudo apt install \
    build-essential \
    cmake \
    ninja-build \
    libgtk-3-dev \
    libwebkit2gtk-4.1-dev \
    libcurl4-openssl-dev \
    pkg-config
```

JUCE is vendored in libs/juce/ via CPM.cmake.
No global JUCE install is required.

## Project Structure

```text
audio-plugin-template/
├── plugin/                # Main plugin code (VST3/Standalone)
│   ├── include/           # Processor and editor headers
│   ├── source/            # PluginProcessor.cpp and PluginEditor.cpp
│   └── CMakeLists.txt
├── test/                  # Test-only target for backend logic
│   ├── source/            # Headless tests (e.g. AudioProcessorTest.cpp)
│   └── CMakeLists.txt
├── libs/                  # JUCE and dependencies (via CPM)
├── CMakeLists.txt         # Root build configuration
└── Makefile               # Handy terminal entry points
```

## Make Targets

```bash
make configure   # Setup build system with tests enabled
make build       # Build default target (if any)
make all         # Explicitly build plugin + test targets
make test        # Build and run headless processor test
```

Resulting in files in `build`

## Testing the AudioProcessor

Unit testing is isolated from the plugin GUI/editor using a dual-source setup:

- `PluginProcessor.cpp` is compiled for both the plugin and test target
- `PluginEditor.cpp` is excluded from test builds
- `createEditor()` and `hasEditor()` are moved to `EditorBridge.cpp` and compiled only when `BUILDING_PLUGIN` is defined

To run the test:

```bash
make test
```


## Plugin Definition (CMake)

Plugin configuration is declared in `plugin/CMakeLists.txt` using `juce_add_plugin(...)`:

```cmake
juce_add_plugin(
  ${PROJECT_NAME}
  COMPANY_NAME "AdamBusiness"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  PLUGIN_MANUFACTURER_CODE Adbm
  PLUGIN_CODE Demo
  FORMATS Standalone VST3
  PRODUCT_NAME "MyPlugin"
)
```

You can change any of these flags (e.g., enable MIDI or synth) to reconfigure plugin behavior. JUCE will automatically update the corresponding JucePlugin_* macros.


## Rename project

```bash
chmod +x rename_project.sh
./rename_project.sh NewProductName NewProjectName [--dry-run]
```