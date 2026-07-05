#include "PluginEditor.h"
#include "PluginProcessor.h"

PluginEditor::PluginEditor(PluginProcessor& p)
    : juce::AudioProcessorEditor(&p), processor(p)
{
    setSize(1200, 800);
    setOpaque(true);
}

PluginEditor::~PluginEditor()
{
}

void PluginEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
    g.setColour(juce::Colours::white);
    g.setFont(15.0f);
    g.drawText("BlackTooth VST3 Plugin", getLocalBounds(), juce::Justification::centred, true);
}

void PluginEditor::resized()
{
    // Layout UI components here
}
