#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

class KnobComponent : public juce::Component, public juce::Slider::Listener
{
public:
    KnobComponent(const std::string& name, float minValue, float maxValue, float defaultValue);
    ~KnobComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void sliderValueChanged(juce::Slider* slider) override;

    float getValue() const { return knobSlider->getValue(); }
    void setValue(float value) { knobSlider->setValue(value, juce::dontSendNotification); }

    std::function<void(float)> onValueChanged;

private:
    std::string knobName;
    std::unique_ptr<juce::Slider> knobSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(KnobComponent)
};
