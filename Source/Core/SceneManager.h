#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../DSP/SignalChain.h"
#include <vector>
#include <memory>
#include <string>

struct SceneData
{
    std::string name;
    juce::Colour color;
    std::string namProfilePath;
    std::string irFilePath;
    std::map<std::string, float> parameters;
    bool active = false;
};

class SceneManager
{
public:
    SceneManager();
    ~SceneManager() = default;

    void prepare(double sampleRate, int blockSize);
    void reset();

    // Scene management
    void setActiveScene(int sceneIndex);
    int getActiveScene() const { return activeSceneIndex; }
    
    SceneData& getScene(int index) { return scenes[index]; }
    const SceneData& getScene(int index) const { return scenes[index]; }

    // Scene operations
    void saveScene(int index, const SignalChain& chain);
    void loadScene(int index, SignalChain& chain);
    void copyScene(int sourceIndex, int destinationIndex);
    void clearScene(int index);

    // Scene naming and appearance
    void setSceneName(int index, const std::string& name);
    void setSceneColor(int index, juce::Colour color);

    // Crossfade control
    void setCrossfadeTime(float timeMs);
    bool isCrossfading() const { return crossfading; }

    // Get all scenes
    const std::vector<SceneData>& getAllScenes() const { return scenes; }

private:
    void updateCrossfade();

    std::vector<SceneData> scenes;
    int activeSceneIndex = 0;
    
    float crossfadeTime = 10.0f;  // ms
    float crossfadeProgress = 1.0f;
    bool crossfading = false;
    
    double sampleRate = 44100.0;
    int blockSize = 512;
};
