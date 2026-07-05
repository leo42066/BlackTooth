#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <cmath>

class GateProcessor
{
public:
    GateProcessor() = default;
    ~GateProcessor() = default;

    void prepare(double sampleRate, int blockSize)
    {
        this->sampleRate = sampleRate;
        this->blockSize = blockSize;
        updateCoefficients();
    }

    void reset()
    {
        envelope = 0.0f;
    }

    float processSample(float input)
    {
        float absInput = std::abs(input);
        
        // Determine if we're above or below threshold
        if (absInput > threshold)
        {
            // Attack phase
            envelope = envelope + (1.0f - envelope) * attackCoeff;
        }
        else
        {
            // Release phase
            envelope = envelope * releaseCoeff;
        }

        // Apply envelope to signal
        return input * envelope;
    }

    void setThreshold(float thresholdDb)
    {
        threshold = juce::Decibels::decibelsToGain(thresholdDb, -80.0f);
    }

    void setAttack(float attackMs)
    {
        attackTime = attackMs;
        updateCoefficients();
    }

    void setRelease(float releaseMs)
    {
        releaseTime = releaseMs;
        updateCoefficients();
    }

    void setBypass(bool shouldBypass)
    {
        bypass = shouldBypass;
    }

private:
    void updateCoefficients()
    {
        if (sampleRate <= 0.0) return;

        // Calculate exponential attack/release coefficients
        attackCoeff = std::exp(-1.0 / (sampleRate * attackTime / 1000.0));
        releaseCoeff = std::exp(-1.0 / (sampleRate * releaseTime / 1000.0));
    }

    double sampleRate = 44100.0;
    int blockSize = 512;
    
    float threshold = 0.01f;  // Linear gain
    float attackTime = 10.0f;  // ms
    float releaseTime = 100.0f;  // ms
    float attackCoeff = 0.9f;
    float releaseCoeff = 0.99f;
    
    float envelope = 0.0f;
    bool bypass = false;
};
