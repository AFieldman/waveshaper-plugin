#include <gtest/gtest.h>
#include <cmath>
#include "WaveshaperPlugin/dsp/Waveshaper.h"

using namespace audio_plugin::dsp;

TEST(WaveshaperTest, SoftClipWithinBounds)
{
    EXPECT_NEAR(applySoftClip(0.0f), 0.0f, 0.001f);
    EXPECT_NEAR(applySoftClip(1.0f), std::tanh(1.0f), 0.001f);
    EXPECT_NEAR(applySoftClip(-1.0f), std::tanh(-1.0f), 0.001f);
}

TEST(WaveshaperTest, HardClipClampsCorrectly)
{
    EXPECT_FLOAT_EQ(applyHardClip(0.5f), 0.5f);
    EXPECT_FLOAT_EQ(applyHardClip(-0.5f), -0.5f);
    EXPECT_FLOAT_EQ(applyHardClip(1.5f), 1.0f);
    EXPECT_FLOAT_EQ(applyHardClip(-2.0f), -1.0f);
}