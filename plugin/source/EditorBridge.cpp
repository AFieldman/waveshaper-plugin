#if BUILDING_PLUGIN

#include "WaveshaperPlugin/PluginProcessor.h"
#include "WaveshaperPlugin/PluginEditor.h"

juce::AudioProcessorEditor*
audio_plugin::AudioPluginAudioProcessor::createEditor() {
  return new AudioPluginAudioProcessorEditor(*this);
}

bool audio_plugin::AudioPluginAudioProcessor::hasEditor() const {
  return true;
}

#endif  // BUILDING_PLUGIN