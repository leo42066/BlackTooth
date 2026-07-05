#include "BlockComponent.h"

BlockComponent::BlockComponent(const std::string& name, juce::Colour color)
    : blockName(name), blockColor(color)
{
    setSize(140, 200);
}

BlockComponent::~BlockComponent()
{
}

void BlockComponent::paint(juce::Graphics& g)
{
    auto bounds = getLocalBounds();
    
    // Draw block background
    juce::Colour bgColor = isActive ? blockColor : blockColor.withAlpha(0.3f);
    g.setColour(bgColor);
    g.fillRoundedRectangle(bounds.toFloat(), 8.0f);
    
    // Draw border
    g.setColour(juce::Colours::white);
    g.drawRoundedRectangle(bounds.toFloat().reduced(1), 8.0f, 2.0f);
    
    // Draw text
    g.setColour(juce::Colours::black);
    g.setFont(14.0f);
    g.drawText(blockName, bounds.reduced(5), juce::Justification::topLeft, true);
    
    // Draw bypass indicator
    if (!isActive)
    {
        g.setColour(juce::Colours::red);
        g.setFont(12.0f);
        g.drawText("BYPASS", bounds.reduced(5), juce::Justification::bottom, false);
    }
}

void BlockComponent::resized()
{
}

void BlockComponent::mouseDown(const juce::MouseEvent& event)
{
    isDragging = true;
    dragStart = event.getPosition();
}

void BlockComponent::mouseDrag(const juce::MouseEvent& event)
{
    if (isDragging)
    {
        auto delta = event.getPosition() - dragStart;
        setTopLeftPosition(getX() + delta.x, getY() + delta.y);
    }
}

void BlockComponent::mouseUp(const juce::MouseEvent& event)
{
    isDragging = false;
}
