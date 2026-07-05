#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "BlockComponent.h"
#include "SceneBar.h"
#include "FileBrowser.h"
#include "LookAndFeel.h"
#include "../Core/PluginProcessor.h"

class MainEditor : public juce::Component
{
public:
    MainEditor(PluginProcessor& processor);
    ~MainEditor() override;

    void paint(juce::Graphics& g) override;
    void resized() override;

    void updateFromProcessor();

private:
    PluginProcessor& processor;
    
    std::unique_ptr<SceneBar> sceneBar;
    std::unique_ptr<BlockComponent> gateBlock;
    std::unique_ptr<BlockComponent> overdriveBlock;
    std::unique_ptr<BlockComponent> namBlock;
    std::unique_ptr<BlockComponent> cabBlock;
    std::unique_ptr<BlockComponent> delayBlock;
    std::unique_ptr<BlockComponent> reverbBlock;
    
    std::unique_ptr<FileBrowser> fileBrowser;
    std::unique_ptr<BlackToothLookAndFeel> lookAndFeel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainEditor)
};
