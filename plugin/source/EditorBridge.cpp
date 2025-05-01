#if BUILDING_PLUGIN

#include "MyPlugin/PluginProcessor.h"
#include "MyPlugin/PluginEditor.h"

juce::AudioProcessorEditor*
audio_plugin::AudioPluginAudioProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

bool audio_plugin::AudioPluginAudioProcessor::hasEditor() const {
  return true;
}

#endif  // BUILDING_PLUGIN