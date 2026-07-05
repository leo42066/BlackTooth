#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include "../Core/SceneManager.h"
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class PresetManager
{
public:
    PresetManager();
    ~PresetManager() = default;

    // Save current state to JSON preset file
    bool savePreset(const juce::File& file, const std::string& name, const SceneManager& sceneManager);

    // Load preset from JSON file
    bool loadPreset(const juce::File& file, SceneManager& sceneManager);

    // Get all presets from a directory
    std::vector<juce::File> getPresetsInFolder(const juce::File& folder);

    // Delete preset file
    bool deletePreset(const juce::File& file);

    // Export preset as JSON string
    std::string exportPresetAsJson(const std::string& name, const SceneManager& sceneManager);

    // Import preset from JSON string
    bool importPresetFromJson(const std::string& jsonString, SceneManager& sceneManager);

private:
    json sceneToJson(const SceneData& scene) const;
    SceneData sceneFromJson(const json& j) const;

    std::string presetVersion = "1.0.0";
};
