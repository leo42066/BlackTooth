#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <string>

class BlockComponent : public juce::Component
{
public:
    BlockComponent(const std::string& name, juce::Colour color);
    ~BlockComponent() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void mouseDown(const juce::MouseEvent& event) override;
    void mouseDrag(const juce::MouseEvent& event) override;
    void mouseUp(const juce::MouseEvent& event) override;

    void setName(const std::string& n) { blockName = n; }
    void setColor(juce::Colour c) { blockColor = c; }
    void setActive(bool active) { isActive = active; repaint(); }

    std::string getName() const { return blockName; }
    juce::Colour getColor() const { return blockColor; }

private:
    std::string blockName;
    juce::Colour blockColor;
    bool isActive = true;
    bool isDragging = false;
    juce::Point<int> dragStart;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(BlockComponent)
};
