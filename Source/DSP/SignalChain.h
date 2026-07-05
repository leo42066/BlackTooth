#pragma once

#include "GateProcessor.h"
#include "OverdriveProcessor.h"
#include "NamProcessor.h"
#include "IrCabProcessor.h"
#include "DelayProcessor.h"
#include "ReverbProcessor.h"
#include <juce_audio_processors/juce_audio_processors.h>
#include <memory>

class SignalChain
{
public:
    SignalChain();
    ~SignalChain() = default;

    void prepare(double sampleRate, int blockSize);
    void reset();

    float processSample(float input);
    void processBlock(juce::AudioBuffer<float>& buffer);

    // Processor access
    GateProcessor& getGate() { return gate; }
    OverdriveProcessor& getOverdrive() { return overdrive; }
    NamProcessor& getNam() { return nam; }
    IrCabProcessor& getIrCab() { return irCab; }
    DelayProcessor& getDelay() { return delay; }
    ReverbProcessor& getReverb() { return reverb; }

    // Master control
    void setMasterLevel(float levelDb);
    void setMasterBypass(bool bypass);

private:
    GateProcessor gate;
    OverdriveProcessor overdrive;
    NamProcessor nam;
    IrCabProcessor irCab;
    DelayProcessor delay;
    ReverbProcessor reverb;

    float masterGain = 1.0f;
    bool masterBypass = false;
    
    double sampleRate = 44100.0;
    int blockSize = 512;
};
