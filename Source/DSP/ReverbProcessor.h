#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_dsp/juce_dsp.h>

class ReverbProcessor
{
public:
    ReverbProcessor() = default;
    ~ReverbProcessor() = default;

    void prepare(double sampleRate, int blockSize)
    {
        this->sampleRate = sampleRate;
        this->blockSize = blockSize;
        
        // Initialize JUCE reverb processor
        juce::dsp::ProcessSpec spec;
        spec.sampleRate = sampleRate;
        spec.numChannels = 2;
        spec.maximumBlockSize = (juce::uint32)blockSize;
        
        reverb.prepare(spec);
        updateReverbParameters();
    }

    void reset()
    {
        reverb.reset();
    }

    float processSample(float input)
    {
        if (bypass)
            return input * dryGain;

        // Process through reverb (single sample)
        juce::dsp::AudioBlock<float> block(&input, 1, 1);
        juce::dsp::ProcessContextReplacing<float> context(block);
        reverb.process(context);

        // Mix dry/wet
        return (input * dryGain) + (input * wetGain * dryInput);
    }

    void setRoomSize(float roomValue)  // 0.0 - 1.0
    {
        roomSize = std::clamp(roomValue, 0.0f, 1.0f);
        updateReverbParameters();
    }

    void setDamping(float dampValue)  // 0.0 - 1.0
    {
        damping = std::clamp(dampValue, 0.0f, 1.0f);
        updateReverbParameters();
    }

    void setMix(float mixValue)  // 0.0 = dry, 1.0 = wet
    {
        mixValue = std::clamp(mixValue, 0.0f, 1.0f);
        wetGain = mixValue;
        dryGain = 1.0f - mixValue;
    }

    void setBypass(bool shouldBypass)
    {
        bypass = shouldBypass;
    }

private:
    void updateReverbParameters()
    {
        reverb.getParameters().roomSize = roomSize;
        reverb.getParameters().damping = damping;
        reverb.getParameters().wetLevel = 1.0f;
        reverb.getParameters().dryLevel = 0.0f;
        reverb.getParameters().width = 1.0f;
        reverb.getParameters().freezeMode = false;
    }

    double sampleRate = 44100.0;
    int blockSize = 512;
    
    juce::dsp::Reverb reverb;
    
    float roomSize = 0.5f;
    float damping = 0.5f;
    float dryGain = 0.7f;
    float wetGain = 0.3f;
    float dryInput = 0.0f;
    bool bypass = false;
};
