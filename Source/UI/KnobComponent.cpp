#include "KnobComponent.h"

KnobComponent::KnobComponent(const std::string& name, float minValue, float maxValue, float defaultValue)
    : knobName(name)
{
    knobSlider = std::make_unique<juce::Slider>(juce::Slider::RotaryVerticalDrag, juce::Slider::TextBoxBelow);
    knobSlider->setRange(minValue, maxValue);
    knobSlider->setValue(defaultValue);
    knobSlider->addListener(this);
    addAndMakeVisible(*knobSlider);

    setSize(80, 100);
}

KnobComponent::~KnobComponent()
{
}

void KnobComponent::paint(juce::Graphics& g)
{
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText(knobName, getLocalBounds().removeFromTop(15), juce::Justification::centred, false);
}

void KnobComponent::resized()
{
    knobSlider->setBounds(getLocalBounds().reduced(5));
}

void KnobComponent::sliderValueChanged(juce::Slider* slider)
{
    if (onValueChanged)
        onValueChanged((float)slider->getValue());
}
