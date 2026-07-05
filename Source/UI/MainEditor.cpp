#include "MainEditor.h"

MainEditor::MainEditor(PluginProcessor& p)
    : processor(p)
{
    lookAndFeel = std::make_unique<BlackToothLookAndFeel>();
    setLookAndFeel(lookAndFeel.get());

    // Create scene bar
    sceneBar = std::make_unique<SceneBar>(processor.getSceneManager());
    addAndMakeVisible(*sceneBar);

    // Create DSP block components
    gateBlock = std::make_unique<BlockComponent>("Gate", juce::Colours::red);
    addAndMakeVisible(*gateBlock);

    overdriveBlock = std::make_unique<BlockComponent>("Overdrive", juce::Colours::orange);
    addAndMakeVisible(*overdriveBlock);

    namBlock = std::make_unique<BlockComponent>("NAM Amp", juce::Colours::yellow);
    addAndMakeVisible(*namBlock);

    cabBlock = std::make_unique<BlockComponent>("Cab IR", juce::Colours::green);
    addAndMakeVisible(*cabBlock);

    delayBlock = std::make_unique<BlockComponent>("Delay", juce::Colours::cyan);
    addAndMakeVisible(*delayBlock);

    reverbBlock = std::make_unique<BlockComponent>("Reverb", juce::Colours::magenta);
    addAndMakeVisible(*reverbBlock);

    fileBrowser = std::make_unique<FileBrowser>(processor.getSignalChain());
    addAndMakeVisible(*fileBrowser);

    setSize(1200, 800);
}

MainEditor::~MainEditor()
{
    setLookAndFeel(nullptr);
}

void MainEditor::paint(juce::Graphics& g)
{
    g.fillAll(juce::Colour(0xFF1A1A1A));
    
    // Draw signal flow visualization
    g.setColour(juce::Colour(0xFF404040));
    g.drawLine(50, 400, 1150, 400, 2.0f);
}

void MainEditor::resized()
{
    auto bounds = getLocalBounds();
    
    // Scene bar at top
    sceneBar->setBounds(bounds.removeFromTop(80));
    
    // Main processing area
    auto processingArea = bounds.reduced(20);
    int blockWidth = 140;
    int blockHeight = 200;
    int spacingX = 20;
    
    int y = processingArea.getY();
    int x = processingArea.getX();
    
    // Layout DSP blocks horizontally
    gateBlock->setBounds(x, y, blockWidth, blockHeight);
    x += blockWidth + spacingX;
    
    overdriveBlock->setBounds(x, y, blockWidth, blockHeight);
    x += blockWidth + spacingX;
    
    namBlock->setBounds(x, y, blockWidth, blockHeight);
    x += blockWidth + spacingX;
    
    cabBlock->setBounds(x, y, blockWidth, blockHeight);
    x += blockWidth + spacingX;
    
    delayBlock->setBounds(x, y, blockWidth, blockHeight);
    x += blockWidth + spacingX;
    
    reverbBlock->setBounds(x, y, blockWidth, blockHeight);
    
    // File browser at bottom
    fileBrowser->setBounds(processingArea.removeFromBottom(150));
}

void MainEditor::updateFromProcessor()
{
    // Update UI from processor state
}
