#include "SceneBar.h"

SceneBar::SceneBar(SceneManager& manager)
    : sceneManager(manager)
{
    // Create 8 scene buttons
    for (int i = 0; i < 8; ++i)
    {
        auto button = std::make_unique<juce::TextButton>("Scene " + std::to_string(i + 1));
        button->addListener(this);
        addAndMakeVisible(*button);
        sceneButtons.push_back(std::move(button));
    }

    // Scene name editor
    sceneNameEditor = std::make_unique<juce::TextEditor>();
    sceneNameEditor->setText(sceneManager.getScene(0).name);
    addAndMakeVisible(*sceneNameEditor);

    setSize(1200, 80);
}

SceneBar::~SceneBar()
{
}

void SceneBar::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF0A0A0A));
    g.setColour(juce::Colours::white);
    g.setFont(12.0f);
    g.drawText("SCENES", 10, 10, 100, 20, juce::Justification::left);
}

void SceneBar::resized()
{
    auto bounds = getLocalBounds().reduced(10);
    int buttonWidth = 130;
    int buttonHeight = 25;
    int spacing = 5;
    
    int x = 100;
    int y = 25;
    
    // Layout scene buttons
    for (auto& button : sceneButtons)
    {
        button->setBounds(x, y, buttonWidth, buttonHeight);
        x += buttonWidth + spacing;
    }
    
    // Scene name editor
    sceneNameEditor->setBounds(10, 50, 800, 25);
}

void SceneBar::buttonClicked(juce::Button* button)
{
    for (int i = 0; i < (int)sceneButtons.size(); ++i)
    {
        if (sceneButtons[i].get() == button)
        {
            sceneManager.setActiveScene(i);
            selectedScene = i;
            sceneNameEditor->setText(sceneManager.getScene(i).name);
            break;
        }
    }
}
