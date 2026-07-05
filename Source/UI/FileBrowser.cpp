#include "FileBrowser.h"

FileBrowser::FileBrowser(SignalChain& chain)
    : signalChain(chain)
{
    // NAM file browser
    namBrowser = std::make_unique<juce::FilenameComponent>(
        "NAM Profile",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        true, false, false, "*.nam", "", "Load NAM profile"
    );
    namBrowser->addListener(this);
    addAndMakeVisible(*namBrowser);

    // IR file browser
    irBrowser = std::make_unique<juce::FilenameComponent>(
        "Cabinet IR",
        juce::File::getSpecialLocation(juce::File::userDocumentsDirectory),
        true, false, false, "*.wav", "", "Load Cabinet IR"
    );
    irBrowser->addListener(this);
    addAndMakeVisible(*irBrowser);

    setSize(1200, 150);
}

FileBrowser::~FileBrowser()
{
}

void FileBrowser::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF0A0A0A));
}

void FileBrowser::resized()
{
    auto bounds = getLocalBounds();
    namBrowser->setBounds(bounds.removeFromTop(75));
    irBrowser->setBounds(bounds);
}

void FileBrowser::filenameComponentChanged(juce::FilenameComponent* fileComponentThatHasChanged)
{
    if (fileComponentThatHasChanged == namBrowser.get())
    {
        juce::File selectedFile = namBrowser->getCurrentFile();
        if (selectedFile.existsAsFile() && selectedFile.getFileExtension() == ".nam")
        {
            signalChain.getNam().loadModel(selectedFile);
        }
    }
    else if (fileComponentThatHasChanged == irBrowser.get())
    {
        juce::File selectedFile = irBrowser->getCurrentFile();
        if (selectedFile.existsAsFile() && selectedFile.getFileExtension() == ".wav")
        {
            signalChain.getIrCab().loadIR(selectedFile);
        }
    }
}
