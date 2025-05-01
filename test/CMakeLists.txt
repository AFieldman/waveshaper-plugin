file(GLOB TEST_SOURCES "source/*.cpp")
add_executable(AudioProcessorTest ${TEST_SOURCES})

# === Required system packages (needed for juce_gui_extra) ===
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK REQUIRED gtk+-3.0)
pkg_check_modules(WEBKIT REQUIRED webkit2gtk-4.1)
add_definitions(${GTK_CFLAGS_OTHER})

target_include_directories(AudioProcessorTest
  PRIVATE
    ${CMAKE_SOURCE_DIR}/plugin/include
    ${GTK_INCLUDE_DIRS}
    ${WEBKIT_INCLUDE_DIRS}
)


# === Plugin macro definitions (shared between plugin and test) ===
set(PLUGIN_MACRO_DEFS
    JucePlugin_Name="WaveshaperPlugin"
    JucePlugin_ManufacturerCode=Adbm
    JucePlugin_Code=Demo
    JucePlugin_IsSynth=0
    JucePlugin_WantsMidiInput=0
    JucePlugin_ProducesMidiOutput=0
    JucePlugin_IsMidiEffect=0
)
target_compile_definitions(AudioProcessorTest PRIVATE ${PLUGIN_MACRO_DEFS})

target_sources(AudioProcessorTest
  PRIVATE ${CMAKE_SOURCE_DIR}/plugin/source/PluginProcessor.cpp
)

target_link_libraries(
  AudioProcessorTest
  PRIVATE juce::juce_recommended_config_flags
          juce::juce_audio_processors
          ${GTK_LIBRARIES}
          ${WEBKIT_LIBRARIES}
          juce::juce_gui_extra
          curl
          gtest
          gtest_main
          waveshaper_dsp
)

set_target_properties(AudioProcessorTest PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test)
