#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <cmath>
#include <vector>
#include <algorithm>

class DelayProcessor
{
public:
    DelayProcessor() = default;
    ~DelayProcessor() = default;

    void prepare(double sampleRate, int blockSize)
    {
        this->sampleRate = sampleRate;
        this->blockSize = blockSize;
        
        // Initialize delay buffer (max 2 seconds)
        int maxDelaySamples = (int)(sampleRate * 2.0);
        delayBuffer.resize(maxDelaySamples, 0.0f);
        delayWritePos = 0;
    }

    void reset()
    {
        std::fill(delayBuffer.begin(), delayBuffer.end(), 0.0f);
        delayWritePos = 0;
        lastOutput = 0.0f;
    }

    float processSample(float input)
    {
        if (bypass || delayBuffer.empty())
            return input * dryGain;

        // Calculate read position
        int delaySamples = (int)(delayTime * sampleRate / 1000.0);
        delaySamples = std::clamp(delaySamples, 1, (int)delayBuffer.size() - 1);
        
        int readPos = delayWritePos - delaySamples;
        if (readPos < 0)
            readPos += delayBuffer.size();

        // Read from delay buffer
        float delayed = delayBuffer[readPos];

        // Calculate feedback with ping-pong if enabled
        float feedbackSample = delayed * feedback;
        
        if (pingpong && lastOutput != 0.0f)
        {
            feedbackSample = lastOutput * feedback;
        }

        // Write to delay buffer
        delayBuffer[delayWritePos] = input + feedbackSample;
        delayWritePos = (delayWritePos + 1) % delayBuffer.size();

        lastOutput = delayed;

        // Mix dry/wet
        return (input * dryGain) + (delayed * wetGain);
    }

    void setTime(float delayMs)
    {
        delayTime = std::clamp(delayMs, 10.0f, 2000.0f);
    }

    void setFeedback(float feedbackValue)
    {
        feedback = std::clamp(feedbackValue, 0.0f, 0.95f);
    }

    void setMix(float mixValue)  // 0.0 = dry, 1.0 = wet
    {
        mixValue = std::clamp(mixValue, 0.0f, 1.0f);
        wetGain = mixValue;
        dryGain = 1.0f - mixValue;
    }

    void setPingpong(bool enable)
    {
        pingpong = enable;
    }

    void setBypass(bool shouldBypass)
    {
        bypass = shouldBypass;
    }

private:
    double sampleRate = 44100.0;
    int blockSize = 512;
    
    std::vector<float> delayBuffer;
    int delayWritePos = 0;
    float delayTime = 250.0f;  // ms
    float feedback = 0.5f;  // 0.0 - 0.95
    float dryGain = 0.7f;
    float wetGain = 0.3f;
    float lastOutput = 0.0f;
    bool pingpong = false;
    bool bypass = false;
};
