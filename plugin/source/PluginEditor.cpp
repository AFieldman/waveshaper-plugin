#include "WaveshaperPlugin/PluginEditor.h"
#include "WaveshaperPlugin/PluginProcessor.h"

namespace waveshaper_plugin {
AudioPluginAudioProcessorEditor::AudioPluginAudioProcessorEditor(
    AudioPluginAudioProcessor& p)
    : AudioProcessorEditor(&p), processorRef(p)
{
  juce::ignoreUnused(processorRef);
  // Make sure that before the constructor has finished, you've set the
  // editor's size to whatever you need it to be.
  setSize(400, 300);

  // Drive
  driveAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
    processorRef.apvts, "drive", driveSlider);
  driveSlider.setSliderStyle(juce::Slider::Rotary);
  driveSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
  driveLabel.setText("Drive", juce::dontSendNotification);
  driveLabel.attachToComponent(&driveSlider, false);
  driveSlider.setTooltip("Amount of gain before shaping (1x to 10x)");


  // Mix
  mixAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(
      processorRef.apvts, "mix", mixSlider);
  mixSlider.setSliderStyle(juce::Slider::LinearHorizontal);
  mixSlider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 60, 20);
  mixLabel.setText("Mix", juce::dontSendNotification);
  mixLabel.attachToComponent(&mixSlider, false);
  mixSlider.setTooltip("Blend between dry and distorted signal");

  // Mode
  modeAttachment = std::make_unique<juce::AudioProcessorValueTreeState::ButtonAttachment>(
      processorRef.apvts, "mode", modeButton);
  modeButton.setButtonText("Hard Clip");
  modeLabel.setText("Clipping Mode", juce::dontSendNotification);
  modeLabel.attachToComponent(&modeButton, true); // true = left of button
  modeButton.addListener(this);
  modeButton.setTooltip("Choose between soft or hard clipping");

  // Add to GUI
  addAndMakeVisible(driveSlider); addAndMakeVisible(driveLabel);
  addAndMakeVisible(mixSlider); addAndMakeVisible(mixLabel);
  addAndMakeVisible(modeButton); addAndMakeVisible(modeLabel);
  updateModeButtonText();
}

AudioPluginAudioProcessorEditor::~AudioPluginAudioProcessorEditor() 
{
  modeButton.removeListener(this);
}

void AudioPluginAudioProcessorEditor::paint(juce::Graphics& g) 
{
  // (Our component is opaque, so we must completely fill the background with a
  // solid colour)
  g.fillAll(
      getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));

  g.setColour(juce::Colours::white);
  g.setFont(15.0f);
}

void AudioPluginAudioProcessorEditor::resized() 
{
  auto bounds = getLocalBounds().reduced(20); // Add 20px padding on all sides
  float totalHeight = static_cast<float>(bounds.getHeight());

  // Compute heights proportionally
  int sliderHeight = static_cast<int>(totalHeight * 0.25f);
  int buttonHeight = static_cast<int>(totalHeight * 0.15f);

  // Layout
  driveSlider.setBounds(bounds.removeFromTop(sliderHeight));
  mixSlider.setBounds(bounds.removeFromTop(sliderHeight).reduced(0, 10));

  modeButton.setBounds(bounds.removeFromTop(buttonHeight).withSizeKeepingCentre(120, buttonHeight));
}

void AudioPluginAudioProcessorEditor::updateModeButtonText()
{
    int mode = static_cast<int>(processorRef.apvts.getRawParameterValue("mode")->load());
    modeButton.setButtonText(mode == 0 ? "Soft Clip" : "Hard Clip");
}

void AudioPluginAudioProcessorEditor::buttonClicked(juce::Button* button)
{
    if (button == &modeButton)
        updateModeButtonText();
}


}  // namespace waveshaper_plugin
