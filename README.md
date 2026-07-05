# BlackTooth VST3 Guitar Processor

**BlackTooth** is a professional-grade VST3 guitar processing plugin featuring real-time guitar processing with ultra-low latency, inspired by the workflow of the Neural DSP Quad Cortex but with entirely original code, UI, and architecture.

## Features

### Core DSP Signal Chain
- **Noise Gate** - Threshold, attack, release controls
- **Overdrive/Preamp** - Gain, tone, level with analog-style waveshaping
- **NAM Amp Block** - Load and process Tone3000 NAM profiles
- **Cab IR Loader** - FFT convolution with WAV impulse responses
- **Delay** - Time, feedback, mix, stereo ping-pong option
- **Reverb** - Room size, damping, mix controls

### Workflow Features
- **8 Scenes** - Instant scene switching with 5-20ms crossfade
- **Scene Management** - Each scene stores NAM profile, IR, all parameters, routing state
- **Quad Cortex-Style UI** - Dark, modern, high-contrast touch-style block layout
- **Drag-and-Drop Blocks** - Visual signal chain editing
- **Parameter Automation** - All parameters exposed for DAW automation

### Tone3000 Integration
- **Local Mode** - Browse and load NAM profiles and IRs from local folders
- **API-Ready Architecture** - Stubbed for future Tone3000 API integration
- **File Browser** - Easy NAM/IR selection with metadata display

### Preset System
- **JSON-Based Presets** - Save/load complete plugin state
- **Scene Data** - Each preset includes all 8 scenes
- **File Path Resolution** - Automatic resolution of NAM and IR paths

## Technology Stack

- **Language:** C++17
- **Framework:** JUCE (latest)
- **Plugin Format:** VST3
- **Build System:** CMake
- **Platforms:** Windows, macOS

## Project Structure

```
BlackTooth/
├── CMakeLists.txt
├── README.md
├── JUCE/                          # JUCE framework (submodule)
├── Source/
│   ├── DSP/
│   │   ├── GateProcessor.h/.cpp
│   │   ├── OverdriveProcessor.h/.cpp
│   │   ├── NamProcessor.h/.cpp
│   │   ├── IrCabProcessor.h/.cpp
│   │   ├── DelayProcessor.h/.cpp
│   │   ├── ReverbProcessor.h/.cpp
│   │   └── SignalChain.h/.cpp
│   ├── UI/
│   │   ├── MainEditor.h/.cpp
│   │   ├── BlockComponent.h/.cpp
│   │   ├── SceneBar.h/.cpp
│   │   ├── FileBrowser.h/.cpp
│   │   ├── KnobComponent.h/.cpp
│   │   └── LookAndFeel.h/.cpp
│   ├── Tone3000/
│   │   ├── Tone3000Source.h
│   │   ├── Tone3000LocalSource.h/.cpp
│   │   └── Tone3000ApiSource.h/.cpp
│   └── Core/
│       ├── PluginProcessor.h/.cpp
│       ├── PluginEditor.h/.cpp
│       ├── SceneManager.h/.cpp
│       └── PresetManager.h/.cpp
└── build/                         # Build artifacts (generated)
```

## Build Instructions

### Prerequisites
- CMake 3.21 or later
- C++17 compiler (MSVC 2019+, Clang, GCC 9+)
- JUCE framework (will be downloaded via CMake)

### Building

```bash
# Clone the repository
git clone https://github.com/leo42066/BlackTooth.git
cd BlackTooth

# Create build directory
mkdir build
cd build

# Configure with CMake
cmake -DCMAKE_BUILD_TYPE=Release ..

# Build
cmake --build . --config Release
```

### Windows
```bash
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" ..
cmake --build . --config Release
```

### macOS
```bash
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

## Architecture Overview

### DSP Architecture
The plugin uses a modular signal chain architecture where each processor is a self-contained class:

```
Input → GateProcessor → OverdriveProcessor → NamProcessor → IrCabProcessor → DelayProcessor → ReverbProcessor → Output
```

Each processor:
- Operates at the plugin's sample rate
- Maintains internal state for processing
- Exposes parameters for automation
- Is thread-safe for loading/unloading external resources

### Tone3000 Integration
The `Tone3000Source` abstract class defines the interface for loading NAM profiles and IRs:

**Tone3000LocalSource** (implemented):
- Scans local folder for NAM and IR files
- Loads .nam files into NamProcessor
- Loads .wav IRs into IrCabProcessor
- Caches loaded models for fast switching

**Tone3000ApiSource** (stubbed for future):
- Search functionality
- Download management
- Metadata parsing
- Authentication handling

### Scene System
Each of the 8 scenes stores:
- Selected NAM profile path
- Selected IR path
- All DSP parameter values
- Routing configuration
- Scene name and color indicator

Scene switching uses a crossfade envelope (5-20ms) to prevent audio artifacts.

### UI Architecture
The UI follows JUCE best practices:
- **MainEditor** - Root component managing layout
- **BlockComponent** - Visual representation of each DSP block
- **SceneBar** - Scene selection and management
- **FileBrowser** - NAM/IR file selection dialog
- **KnobComponent** - Rotary parameter controls with animations
- **LookAndFeel** - Unified theming (dark, modern, high-contrast)

## DSP Implementation Details

### Noise Gate
- Threshold-based gating with smooth attack/release envelopes
- Prevents low-level noise from triggering processing chain
- Attack: 0.1ms - 100ms
- Release: 10ms - 1000ms

### Overdrive/Preamp
- Analog-style waveshaping using soft clipping
- Tone control for frequency shaping
- Gain staging with input/output levels
- Smooth parameter interpolation for real-time adjustments

### NAM Processor
- Loads Tone3000 NAM neural amp models
- Real-time inference with minimal latency overhead
- Input trim and output level controls
- Automatic buffer management for seamless model switching

### Cab IR Processor
- FFT-based convolution for impulse responses
- Supports standard WAV files from Tone3000
- Mix control for blending dry/wet signals
- Optimized for low-latency processing

### Delay
- Time-based delay with feedback control
- Stereo ping-pong mode option
- Mix control for parallel processing
- 10ms - 2000ms delay time

### Reverb
- Room size, damping, mix parameters
- High-quality reverb algorithm
- Smooth parameter transitions

## Automation Support

All parameters are exposed to the DAW for automation:

- Gate: threshold, attack, release, bypass
- Overdrive: gain, tone, level, bypass
- NAM Amp: input trim, output level, model selection, bypass
- Cab IR: mix, model selection, bypass
- Delay: time, feedback, mix, pingpong toggle, bypass
- Reverb: room size, damping, mix, bypass
- Master: output level
- Scenes: scene selection

## Preset System

Presets are saved as JSON files containing:
- Plugin version
- 8 scenes data
- NAM profile paths
- IR file paths
- All DSP parameter values
- UI state

Example preset structure:
```json
{
  "version": "1.0.0",
  "name": "Heavy Metal Lead",
  "scenes": [
    {
      "name": "Crunch",
      "color": "#FF6B00",
      "namProfile": "Marshall JCM800.nam",
      "irFile": "2x12 Greenback.wav",
      "parameters": { ... }
    },
    ...
  ]
}
```

## Performance Specifications

- **Latency:** < 5ms (depending on IR size)
- **CPU Usage:** Optimized for real-time processing
- **Bit Depth:** 32-bit float
- **Sample Rates:** 44.1kHz - 192kHz
- **Channels:** Mono/Stereo

## Future Enhancements

- Tone3000 API integration
- Additional DSP modules (Compressor, EQ, Modulation)
- Custom IR convolution optimization
- Multi-output routing
- Undo/redo system
- Preset tagging and organization
- Real-time parameter visualization

## License

Proprietary - BlackTooth Audio

## Support

For issues, feature requests, or questions, please visit the repository issues page.
