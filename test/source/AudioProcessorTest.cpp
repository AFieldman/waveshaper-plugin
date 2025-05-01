#include <iostream>
#include "MyPlugin/PluginProcessor.h"

int main() {
  audio_plugin::AudioPluginAudioProcessor processor;
  std::cout << "Processor created successfully!\n";
  return 0;
}