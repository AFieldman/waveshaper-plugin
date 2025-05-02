#include "WaveshaperPlugin/PluginProcessor.h"
#include "WaveshaperPlugin/PluginEditor.h"
#include "WaveshaperPlugin/dsp/Waveshaper.h"
#include <fstream>


namespace audio_plugin {

static std::ofstream logFile("/tmp/waveshaper_debug.log", std::ios::app);

void logDebug(float inSample, float driven, float outSample)
{
    logFile << "inSample=" << inSample << " driven=" << driven << " outSample=" << outSample << "\n";
}

AudioPluginAudioProcessor::AudioPluginAudioProcessor()
    : AudioProcessor(
          BusesProperties()
              .withInput("Input", juce::AudioChannelSet::stereo(), true)
              .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
          apvts(*this, nullptr, "Parameters", createParameterLayout()) 
{}

AudioPluginAudioProcessor::~AudioPluginAudioProcessor() 
{}

const juce::String AudioPluginAudioProcessor::getName() const {
  return JucePlugin_Name;
}

bool AudioPluginAudioProcessor::acceptsMidi() const {
  return JucePlugin_WantsMidiInput;
}

bool AudioPluginAudioProcessor::producesMidi() const {
  return JucePlugin_ProducesMidiOutput;
}

double AudioPluginAudioProcessor::getTailLengthSeconds() const {
  return 0.0;
}

int AudioPluginAudioProcessor::getNumPrograms() {
  return 1;  // NB: some hosts don't cope very well if you tell them there are 0
             // programs, so this should be at least 1, even if you're not
             // really implementing programs.
}

int AudioPluginAudioProcessor::getCurrentProgram() {
  return 0;
}

void AudioPluginAudioProcessor::setCurrentProgram(int index) {
  juce::ignoreUnused(index);
}

const juce::String AudioPluginAudioProcessor::getProgramName(int index) {
  juce::ignoreUnused(index);
  return {};
}

void AudioPluginAudioProcessor::changeProgramName(int index,
                                                  const juce::String& newName) {
  juce::ignoreUnused(index, newName);
}

void AudioPluginAudioProcessor::prepareToPlay(double sampleRate,
                                              int samplesPerBlock) {
  // Use this method as the place to do any pre-playback
  // initialisation that you need..
  juce::ignoreUnused(sampleRate, samplesPerBlock);
}

void AudioPluginAudioProcessor::releaseResources() {
  // When playback stops, you can use this as an opportunity to free up any
  // spare memory, etc.
}

bool AudioPluginAudioProcessor::isBusesLayoutSupported(
    const BusesLayout& layouts) const {
#if JucePlugin_IsMidiEffect
  juce::ignoreUnused(layouts);
  return true;
#else
  // Tell the host if plugin doesnt support its format
  if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono() &&
      layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
    return false;

  // This checks if the input layout matches the output layout
#if !JucePlugin_IsSynth
  if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
    return false;
#endif

  return true;
#endif
}

//*
// PROCESS BLOCK ALERT
// 
// 
// PROCESS BLOCK ALERT
//  */

void AudioPluginAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer,
                                             juce::MidiBuffer& midiMessages) {
  juce::ignoreUnused(midiMessages);
  juce::ScopedNoDenormals noDenormals;

  auto totalNumInputChannels = getTotalNumInputChannels();
  auto totalNumOutputChannels = getTotalNumOutputChannels();

  // Clear unused output channels
  for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i) {
    buffer.clear(i, 0, buffer.getNumSamples());
  }
  
  // Retrieve data from the ValueTree
  float drive = apvts.getRawParameterValue("drive")->load();
  float mix   = apvts.getRawParameterValue("mix")->load();
  int mode    = static_cast<int>(apvts.getRawParameterValue("mode")->load());

  // DSP!
  for (int channel = 0; channel < totalNumInputChannels; ++channel){
        auto* samples = buffer.getWritePointer(channel);

        for (int i = 0; i < buffer.getNumSamples(); ++i){
            float inSample = samples[i];
            float driven = inSample * drive;

            float shaped = (mode == 0)
                ? dsp::applySoftClip(driven)
                : dsp::applyHardClip(driven);
            
            samples[i] = (1.0f - mix) * inSample + mix * shaped;
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout AudioPluginAudioProcessor::createParameterLayout(){
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "drive", "Drive", juce::NormalisableRange<float>(1.0f, 10.0f, 0.01f), 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterFloat>(
        "mix", "Mix", juce::NormalisableRange<float>(0.0f, 1.0f, 0.01f), 1.0f));

    params.push_back(std::make_unique<juce::AudioParameterChoice>(
        "mode", "Mode", juce::StringArray { "Soft Clip", "Hard Clip" }, 0));

    return { params.begin(), params.end() };
}


void AudioPluginAudioProcessor::getStateInformation(juce::MemoryBlock& destData){
    auto state = apvts.copyState();                      // copy current state tree
    std::unique_ptr<juce::XmlElement> xml = state.createXml();  // serialize to XML
    copyXmlToBinary(*xml, destData);                     // write to binary block
}


void AudioPluginAudioProcessor::setStateInformation(const void* data, int sizeInBytes){
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));
    
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(apvts.state.getType()))
            apvts.replaceState(juce::ValueTree::fromXml(*xmlState));
}
}  // namespace audio_plugin

// This creates new instances of the plugin.
// This function definition must be in the global namespace.
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
  return new audio_plugin::AudioPluginAudioProcessor();
}
