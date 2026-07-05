#include "LookAndFeel.h"

BlackToothLookAndFeel::BlackToothLookAndFeel()
{
    // Set dark theme colors
    setColour(juce::TextButton::buttonColourId, juce::Colour(0xFF404040));
    setColour(juce::TextButton::buttonOnColourId, juce::Colour(0xFF606060));
    setColour(juce::TextButton::textColourOnId, juce::Colours::white);
    setColour(juce::TextButton::textColourOffId, juce::Colours::white);
    
    setColour(juce::Slider::backgroundColourId, juce::Colour(0xFF1A1A1A));
    setColour(juce::Slider::thumbColourId, juce::Colour(0xFFFF6B00));
    setColour(juce::Slider::trackColourId, juce::Colour(0xFF303030));
    
    setColour(juce::TextEditor::backgroundColourId, juce::Colour(0xFF1A1A1A));
    setColour(juce::TextEditor::textColourId, juce::Colours::white);
    setColour(juce::TextEditor::highlightColourId, juce::Colour(0xFFFF6B00));
}

BlackToothLookAndFeel::~BlackToothLookAndFeel()
{
}

void BlackToothLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
                                             float sliderPosProportional, float rotaryStartAngle,
                                             float rotaryEndAngle, juce::Slider& slider)
{
    auto bounds = juce::Rectangle<float>(x, y, width, height).reduced(5.0f);
    auto centre = bounds.getCentre();
    auto radius = std::min(bounds.getWidth(), bounds.getHeight()) / 2.0f;
    auto angle = rotaryStartAngle + sliderPosProportional * (rotaryEndAngle - rotaryStartAngle);

    // Draw background circle
    g.setColour(juce::Colour(0xFF303030));
    g.fillEllipse(bounds);

    // Draw track
    g.setColour(juce::Colour(0xFF505050));
    juce::Path trackPath;
    trackPath.addCentredArc(centre.x, centre.y, radius, radius, 0.0f,
                           rotaryStartAngle, rotaryEndAngle, true);
    g.strokePath(trackPath, juce::PathStrokeType(3.0f));

    // Draw fill
    g.setColour(juce::Colour(0xFFFF6B00));
    juce::Path fillPath;
    fillPath.addCentredArc(centre.x, centre.y, radius, radius, 0.0f,
                          rotaryStartAngle, angle, true);
    g.strokePath(fillPath, juce::PathStrokeType(4.0f));

    // Draw thumb
    g.setColour(juce::Colours::white);
    auto thumbPos = centre.getPointOnCircumference(radius * 0.7f, angle);
    g.fillEllipse(juce::Rectangle<float>(6, 6).withCentre(thumbPos));
}

void BlackToothLookAndFeel::drawButtonBackground(juce::Graphics& g, juce::Button& button,
                                                const juce::Colour& backgroundColour,
                                                bool shouldDrawButtonAsHighlighted,
                                                bool shouldDrawButtonAsDown)
{
    auto bounds = button.getLocalBounds().toFloat();
    
    juce::Colour bgColor = shouldDrawButtonAsDown ? backgroundColour.brighter(0.2f)
                                                 : backgroundColour;
    if (shouldDrawButtonAsHighlighted)
        bgColor = bgColor.brighter(0.1f);

    g.setColour(bgColor);
    g.fillRoundedRectangle(bounds, 4.0f);

    g.setColour(juce::Colours::white.withAlpha(0.3f));
    g.drawRoundedRectangle(bounds, 4.0f, 1.0f);
}

void BlackToothLookAndFeel::drawButtonText(juce::Graphics& g, juce::TextButton& button,
                                          bool shouldDrawButtonAsHighlighted,
                                          bool shouldDrawButtonAsDown)
{
    auto font = getTextButtonFont(button, button.getHeight());
    g.setFont(font);
    g.setColour(button.findColour(shouldDrawButtonAsDown ? juce::TextButton::textColourOnId
                                                         : juce::TextButton::textColourOffId));
    
    auto yIndent = juce::jlimit(4, button.getHeight() - 4, (int)(button.getHeight() * 0.4f));
    auto cornerSize = juce::jmin(button.getHeight(), button.getWidth()) / 2;
    
    auto textArea = button.getLocalBounds().reduced(juce::jmax(0, cornerSize - yIndent), yIndent);
    g.drawFittedText(button.getButtonText(), textArea, juce::Justification::centred, 2);
}

juce::Font BlackToothLookAndFeel::getTextButtonFont(juce::TextButton& button, int buttonHeight)
{
    return juce::Font(14.0f, juce::Font::bold);
}
