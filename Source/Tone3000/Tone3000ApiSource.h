#pragma once

#include "Tone3000Source.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <string>
#include <vector>

class Tone3000ApiSource : public Tone3000Source
{
public:
    Tone3000ApiSource();
    ~Tone3000ApiSource() = default;

    // Set API credentials
    void setApiKey(const std::string& key);
    void setApiUrl(const std::string& url);

    // Tone3000Source implementation
    std::vector<Tone3000Profile> getAvailableProfiles() override;
    std::vector<Tone3000Profile> getProfilesByType(const std::string& type) override;
    bool loadProfile(const Tone3000Profile& profile) override;
    const std::vector<uint8_t>* getProfileData() const override;
    std::string getSourceType() const override { return "api"; }

    // API-specific methods
    std::vector<Tone3000Profile> searchProfiles(const std::string& query) override;
    bool downloadProfile(const Tone3000Profile& profile, const juce::File& destination);
    bool authenticate(const std::string& username, const std::string& password);

private:
    bool performApiRequest(const std::string& endpoint, std::string& responseData);
    std::vector<Tone3000Profile> parseProfilesFromJson(const std::string& jsonData);

    std::string apiKey;
    std::string apiUrl = "https://api.tone3000.com";  // Placeholder
    bool authenticated = false;
    std::vector<uint8_t> loadedProfileData;
    std::vector<Tone3000Profile> cachedProfiles;
};
