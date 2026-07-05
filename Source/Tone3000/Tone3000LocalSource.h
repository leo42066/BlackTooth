#pragma once

#include "Tone3000Source.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <vector>
#include <map>

class Tone3000LocalSource : public Tone3000Source
{
public:
    Tone3000LocalSource();
    ~Tone3000LocalSource() = default;

    // Set the local folder containing NAM/IR files
    void setLocalFolder(const juce::File& folder);

    // Tone3000Source implementation
    std::vector<Tone3000Profile> getAvailableProfiles() override;
    std::vector<Tone3000Profile> getProfilesByType(const std::string& type) override;
    bool loadProfile(const Tone3000Profile& profile) override;
    const std::vector<uint8_t>* getProfileData() const override;
    std::string getSourceType() const override { return "local"; }

private:
    void scanFolder();
    std::string detectProfileType(const juce::File& file) const;

    juce::File localFolder;
    std::vector<Tone3000Profile> availableProfiles;
    std::vector<uint8_t> loadedProfileData;
    std::map<std::string, std::vector<Tone3000Profile>> profilesByType;
};
