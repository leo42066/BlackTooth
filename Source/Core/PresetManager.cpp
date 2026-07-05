#include "PresetManager.h"

PresetManager::PresetManager()
{
}

bool PresetManager::savePreset(const juce::File& file, const std::string& name, const SceneManager& sceneManager)
{
    try
    {
        json preset;
        preset["version"] = presetVersion;
        preset["name"] = name;
        preset["timestamp"] = juce::Time::getCurrentTime().getMillisecondsSinceEpoch();
        preset["scenes"] = json::array();

        for (int i = 0; i < 8; ++i)
        {
            preset["scenes"].push_back(sceneToJson(sceneManager.getScene(i)));
        }

        std::string jsonString = preset.dump(4);
        return file.replaceWithText(jsonString);
    }
    catch (...)
    {
        return false;
    }
}

bool PresetManager::loadPreset(const juce::File& file, SceneManager& sceneManager)
{
    try
    {
        if (!file.existsAsFile())
            return false;

        auto jsonString = file.loadFileAsString().toStdString();
        auto preset = json::parse(jsonString);

        if (!preset.contains("version") || !preset.contains("scenes"))
            return false;

        auto scenes = preset["scenes"];
        if (scenes.size() != 8)
            return false;

        for (int i = 0; i < 8; ++i)
        {
            sceneManager.getScene(i) = sceneFromJson(scenes[i]);
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

std::vector<juce::File> PresetManager::getPresetsInFolder(const juce::File& folder)
{
    std::vector<juce::File> presets;

    if (!folder.isDirectory())
        return presets;

    juce::RangedDirectoryIterator iter(folder, false, "*.blacktooth", juce::File::findFiles);
    for (const auto& entry : iter)
    {
        presets.push_back(entry.getFile());
    }

    return presets;
}

bool PresetManager::deletePreset(const juce::File& file)
{
    if (!file.existsAsFile())
        return false;
    return file.deleteFile();
}

std::string PresetManager::exportPresetAsJson(const std::string& name, const SceneManager& sceneManager)
{
    try
    {
        json preset;
        preset["version"] = presetVersion;
        preset["name"] = name;
        preset["timestamp"] = juce::Time::getCurrentTime().getMillisecondsSinceEpoch();
        preset["scenes"] = json::array();

        for (int i = 0; i < 8; ++i)
        {
            preset["scenes"].push_back(sceneToJson(sceneManager.getScene(i)));
        }

        return preset.dump(4);
    }
    catch (...)
    {
        return "";
    }
}

bool PresetManager::importPresetFromJson(const std::string& jsonString, SceneManager& sceneManager)
{
    try
    {
        auto preset = json::parse(jsonString);

        if (!preset.contains("version") || !preset.contains("scenes"))
            return false;

        auto scenes = preset["scenes"];
        if (scenes.size() != 8)
            return false;

        for (int i = 0; i < 8; ++i)
        {
            sceneManager.getScene(i) = sceneFromJson(scenes[i]);
        }

        return true;
    }
    catch (...)
    {
        return false;
    }
}

json PresetManager::sceneToJson(const SceneData& scene) const
{
    json j;
    j["name"] = scene.name;
    j["color"] = scene.color.toDisplayString(true).toStdString();
    j["namProfile"] = scene.namProfilePath;
    j["irFile"] = scene.irFilePath;
    j["parameters"] = scene.parameters;
    return j;
}

SceneData PresetManager::sceneFromJson(const json& j) const
{
    SceneData scene;
    if (j.contains("name"))
        scene.name = j["name"].get<std::string>();
    if (j.contains("color"))
        scene.color = juce::Colour::fromString(juce::String(j["color"].get<std::string>()));
    if (j.contains("namProfile"))
        scene.namProfilePath = j["namProfile"].get<std::string>();
    if (j.contains("irFile"))
        scene.irFilePath = j["irFile"].get<std::string>();
    if (j.contains("parameters"))
        scene.parameters = j["parameters"].get<std::map<std::string, float>>();
    return scene;
}
