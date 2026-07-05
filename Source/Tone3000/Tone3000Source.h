#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <string>
#include <vector>
#include <memory>

struct Tone3000Profile
{
    std::string name;
    std::string filePath;
    std::string type;  // "amp", "cab", "gear"
    juce::File file;
};

class Tone3000Source
{
public:
    virtual ~Tone3000Source() = default;

    // Browse and list available profiles
    virtual std::vector<Tone3000Profile> getAvailableProfiles() = 0;

    // Get profiles filtered by type
    virtual std::vector<Tone3000Profile> getProfilesByType(const std::string& type) = 0;

    // Load a profile into memory
    virtual bool loadProfile(const Tone3000Profile& profile) = 0;

    // Get loaded profile data
    virtual const std::vector<uint8_t>* getProfileData() const = 0;

    // Search for profiles (API feature)
    virtual std::vector<Tone3000Profile> searchProfiles(const std::string& query) { return {}; }

    // Get source type
    virtual std::string getSourceType() const = 0;
};
