#include "WaveshaperPlugin/dsp/Waveshaper.h"
#include <cmath> 
#include <juce_core/juce_core.h>

namespace waveshaper_plugin::dsp {

float applySoftClip(float x){
    return std::tanh(x);
}

float applyHardClip(float x){
    return juce::jlimit(-1.0f, 1.0f, x);
}

}  // namespace waveshaper_plugin::dsp
