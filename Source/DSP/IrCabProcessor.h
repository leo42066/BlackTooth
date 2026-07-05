#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>
#include <vector>
#include <string>
#include <complex>
#include <cmath>

class IrCabProcessor
{
public:
    IrCabProcessor() = default;
    ~IrCabProcessor() { unloadIR(); }

    void prepare(double sampleRate, int blockSize)
    {
        this->sampleRate = sampleRate;
        this->blockSize = blockSize;
        partitionSize = 256;
    }

    void reset()
    {
        inputBuffer.clear();
        convolutionBuffer.clear();
    }

    bool loadIR(const juce::File& irFile)
    {
        if (!irFile.existsAsFile())
            return false;

        unloadIR();

        try
        {
            std::unique_ptr<juce::AudioFormatReader> reader(
                formatManager.createReaderFor(irFile)
            );

            if (!reader)
                return false;

            irData = std::make_unique<juce::AudioBuffer<float>>(
                (int)reader->numChannels,
                (int)reader->lengthInSamples
            );
            reader->read(irData.get(), 0, (int)reader->lengthInSamples, 0, true, true);

            currentIrPath = irFile.getFullPathName().toStdString();
            return true;
        }
        catch (...)
        {
            irData.reset();
            return false;
        }
    }

    void unloadIR()
    {
        irData.reset();
        currentIrPath.clear();
        inputBuffer.clear();
        convolutionBuffer.clear();
    }

    float processSample(float input)
    {
        if (bypass || !irData)
            return input * dryGain;

        // Simple convolution using direct method for now
        // In production, this would use FFT-based partitioned convolution
        float output = convolve(input);

        // Mix dry/wet
        return (input * dryGain) + (output * wetGain);
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

    const std::string& getIRPath() const
    {
        return currentIrPath;
    }

    bool isIRLoaded() const
    {
        return irData != nullptr;
    }

private:
    float convolve(float input)
    {
        if (!irData || irData->getNumSamples() == 0)
            return input;

        // Maintain circular buffer for convolution
        inputBuffer.push_back(input);
        if (inputBuffer.size() > (size_t)irData->getNumSamples())
            inputBuffer.erase(inputBuffer.begin());

        // Direct convolution (simplified)
        float output = 0.0f;
        const float* irPtr = irData->getReadPointer(0);
        int irLen = irData->getNumSamples();

        for (int i = 0; i < std::min((int)inputBuffer.size(), irLen); ++i)
        {
            output += inputBuffer[inputBuffer.size() - 1 - i] * irPtr[i];
        }

        return output / (float)irLen;  // Normalize
    }

    double sampleRate = 44100.0;
    int blockSize = 512;
    int partitionSize = 256;
    
    std::unique_ptr<juce::AudioBuffer<float>> irData;
    std::string currentIrPath;
    
    std::vector<float> inputBuffer;
    std::vector<std::complex<float>> convolutionBuffer;
    
    float dryGain = 0.5f;
    float wetGain = 0.5f;
    bool bypass = false;
    
    juce::AudioFormatManager formatManager;
};
