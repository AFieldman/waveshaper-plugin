#include <gtest/gtest.h>
#include "WaveshaperPlugin/PluginProcessor.h"

using namespace waveshaper_plugin;

TEST(AudioProcessorTest, CanConstructProcessor)
{
    AudioPluginAudioProcessor processor;
    SUCCEED(); // If construction didn't throw, test passes
}

TEST(AudioProcessorTest, HasCorrectNumberOfParameters)
{
    AudioPluginAudioProcessor processor;
    auto& apvts = processor.apvts;

    EXPECT_NE(apvts.getParameter("drive"), nullptr);
    EXPECT_NE(apvts.getParameter("mix"), nullptr);
    EXPECT_NE(apvts.getParameter("mode"), nullptr);
}
