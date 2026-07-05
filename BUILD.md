# BlackTooth VST3 Guitar Processor - Build Instructions

## Quick Start

### Prerequisites

**All Platforms:**
- CMake 3.21 or later
- C++17 compatible compiler
- Git

**Windows:**
- Visual Studio 2019 or later
- Windows SDK

**macOS:**
- Xcode 12 or later
- macOS 10.13 or later

**Linux (experimental):**
- GCC 9+ or Clang 10+
- ALSA development files

### Step 1: Clone the Repository

```bash
git clone https://github.com/leo42066/BlackTooth.git
cd BlackTooth
```

### Step 2: Initialize JUCE Submodule

The project requires JUCE as a subdirectory. You can either:

**Option A: Add JUCE as a submodule**
```bash
git submodule add https://github.com/juce-framework/JUCE.git JUCE
git submodule update --init --recursive
```

**Option B: Clone JUCE separately**
```bash
git clone https://github.com/juce-framework/JUCE.git
```

### Step 3: Create Build Directory

```bash
mkdir build
cd build
```

## Platform-Specific Build Instructions

### Windows (Visual Studio)

```bash
# Configure for Visual Studio 2022 (64-bit)
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 17 2022" -A x64 ..

# Build
cmake --build . --config Release

# Built plugin location:
# build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3
```

**For Visual Studio 2019:**
```bash
cmake -DCMAKE_BUILD_TYPE=Release -G "Visual Studio 16 2019" -A x64 ..
cmake --build . --config Release
```

### macOS (Xcode)

```bash
# Configure
cmake -DCMAKE_BUILD_TYPE=Release -G "Xcode" ..

# Build
cmake --build . --config Release

# Built plugin location:
# build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3
```

**Note:** On macOS, you may need to code-sign the plugin:
```bash
codesign --force --deep --sign - build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3
```

### Linux

```bash
# Configure with GCC
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=g++ ..

# Or with Clang
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_COMPILER=clang++ ..

# Build
cmake --build . --config Release -j$(nproc)

# Built plugin location:
# build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3
```

## Installation

### Windows
```bash
# Copy to common VST3 location
copy build\BlackTooth_artefacts\Release\VST3\BlackTooth.vst3 "C:\Program Files\Common Files\VST3\"
```

### macOS
```bash
# Copy to user VST3 location
cp -r build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3 ~/Library/Audio/Plug-Ins/VST3/

# Or system-wide
sudo cp -r build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3 /Library/Audio/Plug-Ins/VST3/
```

### Linux
```bash
# Copy to user VST3 location
mkdir -p ~/.vst3
cp -r build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3 ~/.vst3/

# Or system-wide
sudo mkdir -p /usr/local/lib/vst3
sudo cp -r build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3 /usr/local/lib/vst3/
```

## Development Build

For development with debug symbols:

```bash
mkdir build_debug
cd build_debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```

## Troubleshooting

### CMake Configuration Issues

**Issue:** `Could not find JUCE`

**Solution:** Ensure JUCE is in the `JUCE/` subdirectory:
```bash
ls JUCE/CMakeLists.txt  # Should exist
```

**Issue:** `JUCE compiler not found`

**Solution:** Set C++ compiler explicitly:
```bash
cmake -DCMAKE_CXX_COMPILER=/path/to/compiler ..
```

### Compilation Errors

**Issue:** C++17 not supported

**Solution:** Ensure your compiler is C++17 compatible:
- Windows: Visual Studio 2019+
- macOS: Xcode 10.2+
- Linux: GCC 7+ or Clang 5+

**Issue:** Missing JUCE dependencies on Linux

**Solution:**
```bash
# Ubuntu/Debian
sudo apt-get install libx11-dev libxrandr-dev libxcursor-dev libxinerama-dev \
    libxi-dev libxext-dev libxcomposite-dev libfreetype6-dev libasound2-dev \
    libjack-dev libgl1-mesa-dev libwebkit2gtk-4.0-dev

# Fedora
sudo dnf install xorg-x11-devel libXinerama-devel libXcursor-devel \
    libXrandr-devel libXcomposite-devel freetype-devel alsa-lib-devel \
    jack-audio-connection-kit-devel mesa-libGL-devel webkit2-gtk3-devel
```

### Plugin Not Loading

**Issue:** Plugin not recognized by DAW

**Solution:**
1. Verify plugin location is in DAW's plugin search path
2. Rescan plugin cache in your DAW
3. Check plugin file permissions: `chmod +x BlackTooth.vst3`
4. Verify plugin architecture matches DAW (32-bit/64-bit)

**Issue:** "Cannot load plugin - architecture mismatch"

**Solution:** Ensure you built for the same architecture as your DAW:
```bash
# For 64-bit only
cmake -A x64 ...  # Windows
# macOS/Linux build 64-bit by default
```

## Testing the Build

### Command-Line Test

After building, verify the plugin binary exists:

```bash
# Windows
dir build\BlackTooth_artefacts\Release\VST3\BlackTooth.vst3

# macOS/Linux
ls -la build/BlackTooth_artefacts/Release/VST3/BlackTooth.vst3
```

### DAW Testing

1. Launch your DAW
2. Rescan plugins or navigate to the BlackTooth installation directory
3. Add BlackTooth to a track
4. Verify GUI loads without errors

## Advanced Options

### Building with Optimizations

```bash
cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_CXX_FLAGS="-O3 -march=native" ..
```

### Building with Debug Symbols (Release)

```bash
cmake -DCMAKE_BUILD_TYPE=RelWithDebInfo ..
```

### Building for Specific Processor

```bash
# AVX2 optimizations
cmake -DCMAKE_CXX_FLAGS="-march=haswell" ..

# Generic (maximum compatibility)
cmake -DCMAKE_CXX_FLAGS="-march=x86-64" ..
```

## Clean Build

To completely clean and rebuild:

```bash
rm -rf build
mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```

## Next Steps

1. **Configure Tone3000 folder:** In the UI, set the local Tone3000 folder containing NAM profiles and IR files
2. **Load NAM profiles:** Use the file browser to load neural amp models
3. **Load Cabinet IRs:** Select impulse response files for cabinet simulation
4. **Create Scenes:** Save your favorite configurations as scenes for instant recall
5. **Export Presets:** Save complete plugin states as JSON presets

## Support

For build issues, feature requests, or bug reports, visit:
https://github.com/leo42066/BlackTooth/issues

## License

BlackTooth - Professional VST3 Guitar Processor
Proprietary © BlackTooth Audio
