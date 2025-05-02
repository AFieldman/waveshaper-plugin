#pragma once

#include "PluginProcessor.h"
#include <juce_gui_extra/juce_gui_extra.h> 

namespace audio_plugin {

class AudioPluginAudioProcessorEditor 
  : public juce::AudioProcessorEditor,
    private juce::Button::Listener  
{
public:
  explicit AudioPluginAudioProcessorEditor(AudioPluginAudioProcessor&);
  ~AudioPluginAudioProcessorEditor() override;
  void paint(juce::Graphics&) override;
  void resized() override;

private:
  // === UI Controls ===
  juce::Slider driveSlider;
  juce::Slider mixSlider;
  juce::ToggleButton modeButton;

  juce::Label mixLabel;
  juce::Label driveLabel;
  juce::Label modeLabel;
  juce::Label currentModeLabel;

  // === Attachments (APVTS binding) ===
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> driveAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mixAttachment;
  std::unique_ptr<juce::AudioProcessorValueTreeState::ButtonAttachment> modeAttachment;



  // Methods
  void updateModeButtonText();
  void updateModeLabel();
  void buttonClicked(juce::Button*) override;
  // This reference is provided as a quick way for your editor to
  // access the processor object that created it.
  AudioPluginAudioProcessor& processorRef;
  

  JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioPluginAudioProcessorEditor)
};
}  // namespace audio_plugin
