#include "SceneManager.h"

SceneManager::SceneManager()
{
    scenes.resize(8);
    for (int i = 0; i < 8; ++i)
    {
        scenes[i].name = "Scene " + std::to_string(i + 1);
        scenes[i].color = juce::Colour::fromHSV(i / 8.0f, 0.8f, 0.9f, 1.0f);
        scenes[i].active = (i == 0);
    }
}

void SceneManager::prepare(double sampleRate, int blockSize)
{
    this->sampleRate = sampleRate;
    this->blockSize = blockSize;
}

void SceneManager::reset()
{
    crossfadeProgress = 1.0f;
    crossfading = false;
}

void SceneManager::setActiveScene(int sceneIndex)
{
    if (sceneIndex < 0 || sceneIndex >= 8)
        return;

    if (activeSceneIndex != sceneIndex)
    {
        scenes[activeSceneIndex].active = false;
        activeSceneIndex = sceneIndex;
        scenes[sceneIndex].active = true;
        crossfading = true;
        crossfadeProgress = 0.0f;
    }
}

void SceneManager::saveScene(int index, const SignalChain& chain)
{
    if (index < 0 || index >= 8)
        return;

    SceneData& scene = scenes[index];
    scene.namProfilePath = chain.getNam().getModelPath();
    scene.irFilePath = chain.getIrCab().getIRPath();

    // Store parameter values (would be implemented with APVTS)
    // scene.parameters["gate.threshold"] = ...
}

void SceneManager::loadScene(int index, SignalChain& chain)
{
    if (index < 0 || index >= 8)
        return;

    // This would restore all parameters from scene data
    // Implementation depends on parameter management system
}

void SceneManager::copyScene(int sourceIndex, int destinationIndex)
{
    if (sourceIndex < 0 || sourceIndex >= 8 || destinationIndex < 0 || destinationIndex >= 8)
        return;

    scenes[destinationIndex] = scenes[sourceIndex];
}

void SceneManager::clearScene(int index)
{
    if (index < 0 || index >= 8)
        return;

    scenes[index].name = "Scene " + std::to_string(index + 1);
    scenes[index].color = juce::Colour::fromHSV(index / 8.0f, 0.8f, 0.9f, 1.0f);
    scenes[index].parameters.clear();
    scenes[index].namProfilePath.clear();
    scenes[index].irFilePath.clear();
}

void SceneManager::setSceneName(int index, const std::string& name)
{
    if (index >= 0 && index < 8)
        scenes[index].name = name;
}

void SceneManager::setSceneColor(int index, juce::Colour color)
{
    if (index >= 0 && index < 8)
        scenes[index].color = color;
}

void SceneManager::setCrossfadeTime(float timeMs)
{
    crossfadeTime = std::max(5.0f, std::min(timeMs, 100.0f));
}

void SceneManager::updateCrossfade()
{
    if (!crossfading)
        return;

    float increment = (float)blockSize / (sampleRate * crossfadeTime / 1000.0f);
    crossfadeProgress += increment;

    if (crossfadeProgress >= 1.0f)
    {
        crossfadeProgress = 1.0f;
        crossfading = false;
    }
}
