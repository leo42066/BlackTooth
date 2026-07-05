#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class BlackToothLookAndFeel : public juce::LookAndFeel_V4
{
public:
    BlackToothLookAndFeel();
    ~BlackToothLookAndFeel() override;

    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
                         float sliderPosProportional, float rotaryStartAngle, float rotaryEndAngle,
                         juce::Slider&) override;

    void drawButtonBackground(juce::Graphics&, juce::Button&, const juce::Colour& backgroundColour,
                             bool shouldDrawButtonAsHighlighted, bool shouldDrawButtonAsDown) override;

    void drawButtonText(juce::Graphics&, juce::TextButton&, bool shouldDrawButtonAsHighlighted,
                       bool shouldDrawButtonAsDown) override;

    juce::Font getTextButtonFont(juce::TextButton&, int buttonHeight) override;

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlackToothLookAndFeel)
};
