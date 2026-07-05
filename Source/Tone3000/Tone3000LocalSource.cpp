#include "Tone3000LocalSource.h"

Tone3000LocalSource::Tone3000LocalSource()
{
}

void Tone3000LocalSource::setLocalFolder(const juce::File& folder)
{
    if (!folder.isDirectory())
        return;

    localFolder = folder;
    availableProfiles.clear();
    profilesByType.clear();
    loadedProfileData.clear();
    scanFolder();
}

void Tone3000LocalSource::scanFolder()
{
    if (!localFolder.exists())
        return;

    for (const auto& entry : juce::RangedDirectoryIterator(localFolder, false, "*.nam", juce::File::findFiles))
    {
        Tone3000Profile profile;
        profile.file = entry.getFile();
        profile.filePath = entry.getFile().getFullPathName().toStdString();
        profile.name = entry.getFile().getFileNameWithoutExtension().toStdString();
        profile.type = detectProfileType(entry.getFile());

        availableProfiles.push_back(profile);
        profilesByType[profile.type].push_back(profile);
    }

    // Also scan for IR files
    for (const auto& entry : juce::RangedDirectoryIterator(localFolder, false, "*.wav", juce::File::findFiles))
    {
        Tone3000Profile profile;
        profile.file = entry.getFile();
        profile.filePath = entry.getFile().getFullPathName().toStdString();
        profile.name = entry.getFile().getFileNameWithoutExtension().toStdString();
        profile.type = "cab";  // Assume WAV files are impulse responses

        availableProfiles.push_back(profile);
        profilesByType[profile.type].push_back(profile);
    }
}

std::vector<Tone3000Profile> Tone3000LocalSource::getAvailableProfiles()
{
    return availableProfiles;
}

std::vector<Tone3000Profile> Tone3000LocalSource::getProfilesByType(const std::string& type)
{
    auto it = profilesByType.find(type);
    if (it != profilesByType.end())
        return it->second;
    return {};
}

bool Tone3000LocalSource::loadProfile(const Tone3000Profile& profile)
{
    if (!profile.file.existsAsFile())
        return false;

    try
    {
        auto stream = profile.file.createInputStream();
        if (!stream)
            return false;

        auto fileSize = stream->getTotalLength();
        loadedProfileData.clear();
        loadedProfileData.resize(fileSize);
        stream->read(loadedProfileData.data(), fileSize);
        return true;
    }
    catch (...)
    {
        loadedProfileData.clear();
        return false;
    }
}

const std::vector<uint8_t>* Tone3000LocalSource::getProfileData() const
{
    if (loadedProfileData.empty())
        return nullptr;
    return &loadedProfileData;
}

std::string Tone3000LocalSource::detectProfileType(const juce::File& file) const
{
    auto ext = file.getFileExtension().toStdString();
    if (ext == ".nam")
        return "amp";  // NAM files are typically amp models
    if (ext == ".wav")
        return "cab";  // WAV files are cabinet IRs
    return "unknown";
}
