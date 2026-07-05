# BlackTooth VST3 Plugin - Architecture & Development Guide

## Architecture Overview

BlackTooth follows a modular, layered architecture for maintainability and extensibility:

```
┌─────────────────────────────────────────────────────────────────┐
│                     JUCE Plugin Framework                       │
├─────────────────────────────────────────────────────────────────┤
│                                                                 │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │              PluginProcessor (Audio Thread)             │   │
│  │  - Audio I/O management                                 │   │
│  │  - Parameter automation                                 │   │
│  │  - State management                                     │   │
│  └─────────────────────────────────────────────────────────┘   │
│                          │                                      │
│                          ▼                                      │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │                 SignalChain (DSP)                        │   │
│  │  Processes audio through processor chain:               │   │
│  │  1. GateProcessor       (Noise gating)                   │   │
│  │  2. OverdriveProcessor  (Waveshaping)                    │   │
│  │  3. NamProcessor        (Neural amp model)               │   │
│  │  4. IrCabProcessor      (Impulse response convolution)   │   │
│  │  5. DelayProcessor      (Time-based delay)               │   │
│  │  6. ReverbProcessor     (Reverb)                         │   │
│  └──────��──────────────────────────────────────────────────┘   │
│                          │                                      │
│                          ▼                                      │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │           SceneManager & PresetManager                  │   │
│  │  - 8 scenes with instant switching                      │   │
│  │  - JSON-based preset system                             │   │
│  │  - State persistence                                    │   │
│  └─────────────────────────────────────────────────────────┘   │
│                          │                                      │
│                          ▼                                      │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │            Tone3000 Integration Layer                   │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │  Tone3000LocalSource (Implemented)              │   │   │
│  │  │  - File system scanning                          │   │   │
│  │  │  - NAM/IR loading                                │   │   │
│  │  │  - Local caching                                 │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │  Tone3000ApiSource (Stubbed for future)         │   │   │
│  │  │  - API communication                             │   │   │
│  │  │  - Remote profile search                         │   │   │
│  │  │  - Download management                           │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  └─────────────────────────────────────────────────────────┘   │
│                          │                                      │
│                          ▼                                      │
│  ┌─────────────────────────────────────────────────────────┐   │
│  │                PluginEditor (GUI Thread)                │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │  MainEditor                                      │   │   │
│  │  │  - Layout management                             │   │   │
│  │  │  - Component coordination                        │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │  BlockComponent (×6)                             │   │   │
│  │  │  - Visual DSP block representation               │   │   │
│  │  │  - Drag-and-drop support                         │   │   │
│  │  │  - Parameter display                             │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │  SceneBar                                        │   │   │
│  │  │  - 8 scene buttons                               │   │   │
│  │  │  - Scene naming                                  │   │   │
│  │  │  - Color indicators                              │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │  FileBrowser                                     │   │   │
│  │  │  - NAM profile selection                         │   │   │
│  │  │  - IR file loading                               │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  │  ┌──────────────────────────────────────────────────┐   │   │
│  │  │  BlackToothLookAndFeel                           │   │   │
│  │  │  - Dark theme styling                            │   │   │
│  │  │  - Custom knob rendering                         │   │   │
│  │  │  - Component appearance                          │   │   │
│  │  └──────────────────────────────────────────────────┘   │   │
│  └─────────────────────────────────────────────────────────┘   │
│                                                                 │
└─────────────────────────────────────────────────────────────────┘
```

## Module Descriptions

### DSP Modules (`Source/DSP/`)

Each DSP processor is a self-contained class with:
- `prepare(sampleRate, blockSize)` - Initialize for audio processing
- `reset()` - Clear internal state
- `processSample(input)` - Process single audio sample
- Parameter setters for automation
- Thread-safe design

**GateProcessor:**
- Envelope follower with attack/release
- Threshold-based triggering
- Smooth gain transitions

**OverdriveProcessor:**
- Soft clipping waveshaper
- Tone control (low-pass filter)
- Analog-style saturation

**NamProcessor:**
- Neural amp model loader
- Supports Tone3000 NAM format
- Real-time inference placeholder
- Thread-safe model switching

**IrCabProcessor:**
- FFT-based convolution engine
- WAV file impulse response loading
- Efficient partitioned convolution architecture
- Dry/wet mixing

**DelayProcessor:**
- Circular buffer delay
- Feedback control
- Stereo ping-pong mode
- Tempo-sync ready

**ReverbProcessor:**
- JUCE reverb algorithm wrapper
- Room size and damping parameters
- High-quality spatial processing

**SignalChain:**
- Orchestrates all DSP processors
- Implements audio routing
- Manages master level and bypass
- Block-level processing coordination

### Tone3000 Integration (`Source/Tone3000/`)

**Tone3000Source (Abstract):**
- Interface definition for profile sources
- Pure virtual methods for implementers
- Polymorphic loading system

**Tone3000LocalSource (Implemented):**
- Scans local folders for NAM/IR files
- Caches available profiles
- Handles file I/O and loading
- Type detection (amp/cab)

**Tone3000ApiSource (Stubbed):**
- API endpoint stubs
- Authentication placeholder
- Search functionality skeleton
- Download management hooks
- Ready for future HTTP integration

### Core Modules (`Source/Core/`)

**PluginProcessor:**
- JUCE AudioProcessor implementation
- Audio thread management
- Parameter automation integration
- State serialization

**PluginEditor:**
- JUCE AudioProcessorEditor implementation
- Hosts MainEditor component
- GUI lifecycle management

**SceneManager:**
- 8 scene storage and management
- Instant scene switching with crossfade
- Parameter recall per scene
- Scene naming and appearance customization

**PresetManager:**
- JSON-based preset serialization
- Complete state export/import
- File system preset management
- Tone3000 path resolution

### UI Components (`Source/UI/`)

**MainEditor:**
- Root component coordinator
- Layout manager
- Integrates all UI subcomponents
- Update coordination

**BlockComponent:**
- Visual representation of DSP block
- Color-coded by processor type
- Drag-and-drop support (extensible)
- Bypass state indication
- Parameter display area

**SceneBar:**
- 8 scene selection buttons
- Active scene highlighting
- Scene name editor
- Color indicators

**FileBrowser:**
- NAM file selection component
- IR file selection component
- Direct loader integration
- Folder browsing

**KnobComponent:**
- Rotary parameter control
- Smooth value animation
- Text box display
- Callback integration

**BlackToothLookAndFeel:**
- Dark, modern visual theme
- Custom knob rendering
- Button styling
- Color scheme definition
- High-contrast design

## Data Flow

### Audio Processing Thread

```
DAW Audio Input
    ↓
PluginProcessor::processBlock()
    ↓
SignalChain::processBlock()
    ↓
GateProcessor → OverdriveProcessor → NamProcessor 
    ↓                                      ↓
    └──→ IrCabProcessor → DelayProcessor → ReverbProcessor
    ↓
SignalChain Master Gain
    ↓
DAW Audio Output
```

### UI Update Thread (Message Thread)

```
User Interaction
    ↓
UI Component (Button, Slider, etc.)
    ↓
Parameter Value Changed
    ↓
PluginProcessor::apvts (AudioProcessorValueTreeState)
    ↓
DSP Processor Parameter Setter
    ↓
Next Audio Block Uses New Value
```

### Scene Management Flow

```
User Clicks Scene Button
    ↓
SceneBar::buttonClicked()
    ↓
SceneManager::setActiveScene()
    ↓
Crossfade Envelope Trigger
    ↓
SignalChain Processes with Crossfade
    ↓
Scene Parameters Applied
```

## Thread Safety

BlackTooth uses these thread-safety mechanisms:

1. **Audio Thread (Real-Time):**
   - No allocations
   - No locks (lock-free where possible)
   - Minimal data structures

2. **Message Thread (UI):**
   - JUCE MessageManager coordination
   - Safe DSP parameter updates via atomic values
   - Deferred resource loading

3. **File Loading:**
   - Off-thread loading for NAM/IR files
   - Atomic pointer swapping for real-time safety
   - No blocking in audio thread

## Performance Considerations

### Latency Budget

- Target: < 5ms total plugin latency
- Gate processor: < 0.1ms
- Overdrive processor: < 0.1ms
- NAM inference: 1-2ms (model dependent)
- IR convolution: 2-3ms (partition size dependent)
- Delay: configurable 10-2000ms
- Reverb: < 0.5ms (mixing only)

### CPU Optimization

1. **SIMD Instructions:** Use JUCE DSP for vectorized operations
2. **Caching:** Pre-compute filter coefficients
3. **Partitioned Convolution:** Distribute IR processing
4. **Efficient Waveshaping:** Polynomial approximations

## Extending BlackTooth

### Adding a New DSP Processor

1. Create new header in `Source/DSP/`:
```cpp
class MyProcessor {
public:
    void prepare(double sampleRate, int blockSize);
    void reset();
    float processSample(float input);
    void setParameter(float value);
};
```

2. Add to SignalChain:
```cpp
MyProcessor myProcessor;
float processSample(float input) {
    // Add to chain
    signal = myProcessor.processSample(signal);
    return signal;
}
```

3. Create UI component in `Source/UI/`
4. Integrate with SceneManager for preset support

### Adding API Integration

1. Implement `Tone3000ApiSource::performApiRequest()`
2. Use JUCE URL class or libcurl wrapper
3. Parse JSON responses
4. Implement search, download, caching

### Adding Parameters for Automation

1. Add to `PluginProcessor::apvts` parameter layout
2. Create parameter ID and range
3. Connect to DSP processor via setter
4. Update UI to reflect parameter

## Testing

### Unit Testing

Each DSP processor can be tested independently:
```cpp
GateProcessor gate;
gate.prepare(44100, 512);
gat.setThreshold(-40.0f);
float output = gate.processSample(0.5f);
```

### Integration Testing

1. Load test NAM profiles
2. Load test IR files
3. Verify scene switching
4. Test preset save/load

### Audio Testing

1. Use spectral analyzer to verify frequency response
2. Test latency with impulse response measurement
3. Verify no audio dropouts during scene switching
4. Monitor CPU usage during processing

## Compilation Warnings

The project compiles with strict warnings enabled (`/W4` on MSVC, `-Wall -Wextra` on GCC/Clang). All warnings should be treated as errors.

## Code Style

BlackTooth follows JUCE conventions:
- CamelCase for class names
- camelCase for method/variable names
- 4-space indentation
- JUCE smart pointers (std::unique_ptr, std::shared_ptr)
- RAII for resource management

## Future Enhancements

1. **Real-time NAM Inference:** Integrate TensorFlow Lite or ONNX Runtime
2. **Multi-output Routing:** Parallel processing chains
3. **Compressor/EQ:** Additional DSP blocks
4. **Automation Recording:** Parameter recording and playback
5. **Undo/Redo System:** State history management
6. **Custom IR Creation:** Built-in convolution designer
7. **A/B Comparison:** Side-by-side parameter testing
8. **Mobile Companion:** iOS remote control app

## Performance Profiling

To profile BlackTooth:

**macOS:**
```bash
Instruments -t "System Trace" build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3
```

**Windows:**
Use Windows Performance Analyzer or Visual Studio Profiler

**Linux:**
```bash
perf record -p <plugin_pid> -- <daw_name>
perf report
```

## Contributing

When contributing to BlackTooth:

1. Follow the code style guidelines
2. Ensure all code compiles warning-free
3. Add appropriate comments for complex logic
4. Test thoroughly on all target platforms
5. Update documentation for new features
6. Submit pull requests with clear descriptions

## License & Attribution

BlackTooth is proprietary software. All code is original and does not incorporate any proprietary Tone3000, Neural DSP, or other third-party plugin code.

JUCE framework used under license agreement.
