#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../Core/SceneManager.h"

class SceneBar : public juce::Component, public juce::Button::Listener
{
public:
    SceneBar(SceneManager& manager);
    ~SceneBar() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void buttonClicked(juce::Button* button) override;

private:
    SceneManager& sceneManager;
    std::vector<std::unique_ptr<juce::TextButton>> sceneButtons;
    std::unique_ptr<juce::TextEditor> sceneNameEditor;
    int selectedScene = 0;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SceneBar)
};
