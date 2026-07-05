#include "SignalChain.h"

SignalChain::SignalChain()
{
}

void SignalChain::prepare(double sampleRate, int blockSize)
{
    this->sampleRate = sampleRate;
    this->blockSize = blockSize;

    gate.prepare(sampleRate, blockSize);
    overdrive.prepare(sampleRate, blockSize);
    nam.prepare(sampleRate, blockSize);
    irCab.prepare(sampleRate, blockSize);
    delay.prepare(sampleRate, blockSize);
    reverb.prepare(sampleRate, blockSize);
}

void SignalChain::reset()
{
    gate.reset();
    overdrive.reset();
    nam.reset();
    irCab.reset();
    delay.reset();
    reverb.reset();
}

float SignalChain::processSample(float input)
{
    if (masterBypass)
        return input * masterGain;

    // Process through signal chain
    float signal = input;
    signal = gate.processSample(signal);
    signal = overdrive.processSample(signal);
    signal = nam.processSample(signal);
    signal = irCab.processSample(signal);
    signal = delay.processSample(signal);
    signal = reverb.processSample(signal);
    signal = signal * masterGain;

    return signal;
}

void SignalChain::processBlock(juce::AudioBuffer<float>& buffer)
{
    for (int ch = 0; ch < buffer.getNumChannels(); ++ch)
    {
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        {
            float* channelData = buffer.getWritePointer(ch);
            channelData[i] = processSample(channelData[i]);
        }
    }
}

void SignalChain::setMasterLevel(float levelDb)
{
    masterGain = juce::Decibels::decibelsToGain(levelDb, -80.0f);
}

void SignalChain::setMasterBypass(bool bypass)
{
    masterBypass = bypass;
}
