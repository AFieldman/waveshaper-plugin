cmake_minimum_required(VERSION 3.22)
project(AdamWaveshaper VERSION 0.1.0)

# === Globals ===
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include/WaveshaperPlugin")
set(PROJECT_WARNINGS_CXX "-Wall" "-Wextra" "-Wpedantic" "-Wconversion" "-Wsign-conversion" "-Werror")


# === JUCE plugin target ===
juce_add_plugin(
  ${PROJECT_NAME}
  COMPANY_NAME "YourBusiness"
  IS_SYNTH FALSE
  NEEDS_MIDI_INPUT FALSE
  NEEDS_MIDI_OUTPUT FALSE
  PLUGIN_MANUFACTURER_CODE Adbm
  PLUGIN_CODE Demo
  FORMATS Standalone VST3
  PRODUCT_NAME "WaveshaperPlugin"
)

# === DSP static library ===
add_library(waveshaper_dsp STATIC
    source/dsp/Waveshaper.cpp
)

target_include_directories(waveshaper_dsp
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(waveshaper_dsp
    PUBLIC juce::juce_core
)

# === Plugin-specific compile flags and definitions ===
target_compile_definitions(${PROJECT_NAME}
  PRIVATE BUILDING_PLUGIN
  PUBLIC
    JUCE_WEB_BROWSER=0 
    JUCE_USE_CURL=0 
    JUCE_VST3_CAN_REPLACE_VST2=0
)

# === Plugin sources and headers ===
set(SOURCE_FILES
    source/PluginProcessor.cpp
    source/PluginEditor.cpp
    source/EditorBridge.cpp
)

set(HEADER_FILES
    ${INCLUDE_DIR}/PluginEditor.h
    ${INCLUDE_DIR}/PluginProcessor.h
)

target_sources(${PROJECT_NAME} PRIVATE ${SOURCE_FILES} ${HEADER_FILES})

# === Warnings (your code only) ===
set_source_files_properties(${SOURCE_FILES} PROPERTIES COMPILE_OPTIONS "${PROJECT_WARNINGS_CXX}")

# === Plugin includes and linking ===
target_include_directories(${PROJECT_NAME}
    PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        waveshaper_dsp
        juce::juce_audio_utils
        juce::juce_recommended_config_flags
        juce::juce_recommended_lto_flags
        juce::juce_recommended_warning_flags
)

# === Visual Studio project filters ===
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/..)
