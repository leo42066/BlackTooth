#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <cmath>
#include <algorithm>

class OverdriveProcessor
{
public:
    OverdriveProcessor() = default;
    ~OverdriveProcessor() = default;

    void prepare(double sampleRate, int blockSize)
    {
        this->sampleRate = sampleRate;
        this->blockSize = blockSize;
    }

    void reset()
    {
        // No state to reset for basic waveshaping
    }

    float processSample(float input)
    {
        if (bypass)
            return input;

        // Apply input gain
        float gained = input * inputGain;

        // Soft clipping waveshaping
        float shaped = softClip(gained);

        // Tone control (simple low-pass)
        shaped = applyToneControl(shaped);

        // Output level
        return shaped * outputGain;
    }

    void setGain(float gainDb)
    {
        inputGain = juce::Decibels::decibelsToGain(gainDb, -80.0f);
    }

    void setTone(float toneValue)  // 0.0 - 1.0
    {
        toneValue = std::clamp(toneValue, 0.0f, 1.0f);
        // Tone control frequency mapping
        toneFreq = 200.0f + (toneValue * 10000.0f);  // 200Hz - 10kHz
        updateToneCoeff();
    }

    void setLevel(float levelDb)
    {
        outputGain = juce::Decibels::decibelsToGain(levelDb, -80.0f);
    }

    void setBypass(bool shouldBypass)
    {
        bypass = shouldBypass;
    }

private:
    float softClip(float x)
    {
        // Soft clipping using tanh-like function
        if (x > 1.0f)
            return 0.66666f;  // Asymptotic limit
        else if (x < -1.0f)
            return -0.66666f;
        else
            return x - (x * x * x) / 3.0f;  // Polynomial approximation
    }

    float applyToneControl(float input)
    {
        // Simple one-pole low-pass filter for tone control
        toneSample = toneSample + toneCoeff * (input - toneSample);
        return toneSample;
    }

    void updateToneCoeff()
    {
        if (sampleRate <= 0.0) return;
        toneCoeff = 2.0f * 3.14159f * toneFreq / (float)sampleRate;
        toneCoeff = std::clamp(toneCoeff, 0.0f, 1.0f);
    }

    double sampleRate = 44100.0;
    int blockSize = 512;
    
    float inputGain = 1.0f;
    float outputGain = 1.0f;
    float toneFreq = 5000.0f;
    float toneCoeff = 0.1f;
    float toneSample = 0.0f;
    bool bypass = false;
};
