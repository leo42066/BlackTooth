#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/SignalChain.h"

class FileBrowser : public juce::Component, public juce::FilenameComponentListener
{
public:
    FileBrowser(SignalChain& chain);
    ~FileBrowser() override;

    void paint(juce::Graphics& g) override;
    void resized() override;
    void filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged) override;

private:
    SignalChain& signalChain;
    std::unique_ptr<juce::FilenameComponent> namBrowser;
    std::unique_ptr<juce::FilenameComponent> irBrowser;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(FileBrowser)
};
