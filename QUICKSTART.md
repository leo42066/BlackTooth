# BlackTooth VST3 Plugin - Quick Reference

## Plugin Information

- **Name:** BlackTooth
- **Format:** VST3
- **Platforms:** Windows, macOS, Linux (experimental)
- **Latency:** < 5ms (depends on IR size)
- **I/O:** Mono/Stereo
- **Sample Rates:** 44.1kHz - 192kHz

## DSP Signal Chain

```
Input → Gate → Overdrive → NAM Amp → Cab IR → Delay → Reverb → Master → Output
```

### 1. Noise Gate
- **Parameters:**
  - Threshold: -80dB to 0dB
  - Attack: 0.1ms to 100ms
  - Release: 10ms to 1000ms
  - Bypass: ON/OFF
- **Purpose:** Prevent low-level noise from triggering processors

### 2. Overdrive/Preamp
- **Parameters:**
  - Gain: -24dB to +24dB
  - Tone: 0 to 1.0 (200Hz to 10kHz)
  - Level: -24dB to +24dB
  - Bypass: ON/OFF
- **Purpose:** Analog-style saturation and waveshaping

### 3. NAM Amp Block
- **Parameters:**
  - Input Trim: -12dB to +12dB
  - Output Level: -24dB to +24dB
  - Model Selection: File browser
  - Bypass: ON/OFF
- **Purpose:** Neural network amp modeling (Tone3000 profiles)
- **Supported Format:** .nam files

### 4. Cabinet IR
- **Parameters:**
  - Mix: 0% to 100% (Dry to Wet)
  - IR Selection: File browser
  - Bypass: ON/OFF
- **Purpose:** Impulse response convolution for cabinet simulation
- **Supported Format:** .wav files
- **Algorithm:** FFT-based partitioned convolution

### 5. Delay
- **Parameters:**
  - Time: 10ms to 2000ms
  - Feedback: 0% to 95%
  - Mix: 0% to 100% (Dry to Wet)
  - Ping-Pong: ON/OFF
  - Bypass: ON/OFF
- **Purpose:** Time-based delay effect

### 6. Reverb
- **Parameters:**
  - Room Size: 0 to 1.0
  - Damping: 0 to 1.0
  - Mix: 0% to 100% (Dry to Wet)
  - Bypass: ON/OFF
- **Purpose:** High-quality reverb algorithm

### Master
- **Parameters:**
  - Output Level: -24dB to +24dB
  - Bypass: ON/OFF
- **Purpose:** Final gain staging

## Scenes System

- **Total Scenes:** 8
- **Per Scene Stored:**
  - NAM amp model path
  - Cabinet IR file path
  - All DSP parameter values
  - Scene name and color
- **Scene Switching:** Instant with 5-20ms crossfade
- **Naming:** Editable scene names
- **Color Coding:** Custom color per scene for visual identification

## Tone3000 Integration

### Local Mode (Implemented)
- **File Format Support:**
  - NAM files (`.nam`) - Neural amp models
  - WAV files (`.wav`) - Cabinet impulse responses
- **Workflow:**
  1. Point to local Tone3000 folder
  2. Plugin scans available profiles
  3. Select from list in file browser
  4. Profile loaded into DSP chain
  5. Save as scene for instant recall

### API Mode (Stubbed - Future)
- Search functionality placeholder
- Download management hooks
- Authentication framework
- Ready for HTTP integration

## Preset System

### Preset Format
- **Type:** JSON text files
- **Extension:** `.blacktooth` (optional, any text editor can open)
- **Location:** User-defined (typically ~/Documents/BlackTooth Presets/)

### Preset Contents
```json
{
  "version": "1.0.0",
  "name": "Heavy Metal Lead",
  "timestamp": 1656345600000,
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

### Preset Operations
- **Save:** Export all 8 scenes + parameters to JSON
- **Load:** Import preset file restoring complete state
- **Export:** Convert preset to JSON string
- **Import:** Load preset from JSON text

## File Browser

### NAM Profile Browser
- **File Type:** .nam (Neural amp models)
- **Typical Locations:**
  - Tone3000 downloaded files
  - Custom NAM models
- **Loading:** Single-click loads into NAM processor

### Cabinet IR Browser
- **File Type:** .wav (16-bit, mono or stereo)
- **Typical Sizes:** 1-2 seconds of audio
- **Common Sources:**
  - Tone3000 IR library
  - Commercial IR packs
  - Custom impulse measurements
- **Loading:** Single-click loads into Cab processor

## UI Layout

### Top Bar
- **Scene Bar:** 8 scene buttons + name editor + color indicator

### Main Area
- **Signal Chain Blocks:** Visual representation of each DSP processor
  - Color-coded blocks
  - Drag-and-drop support
  - Bypass indicators
  - Parameter knobs (in block details)

### Bottom Area
- **File Browser:** NAM profile and IR selection
- **Controls:** Load/refresh file lists

## Automation Support

All parameters are exposed to DAW for automation:

```
Gate (Threshold, Attack, Release)
Overdrive (Gain, Tone, Level)
NAM Amp (Input Trim, Output Level)
Cab IR (Mix)
Delay (Time, Feedback, Mix)
Reverb (Room Size, Damping, Mix)
Master (Output Level)
Scenes (Scene Selection)
```

## Performance Tips

1. **Reduce CPU Usage:**
   - Disable unused processors (bypass)
   - Reduce IR convolution quality
   - Lower reverb room size

2. **Minimize Latency:**
   - Reduce IR size
   - Decrease delay time
   - Simplify NAM model selection

3. **Best Practices:**
   - Pre-load common profiles/IRs
   - Use scenes to avoid unnecessary file browsing
   - Save presets for quick workflow

## Keyboard Shortcuts

(Future implementation)
- `Ctrl/Cmd + S` - Save preset
- `Ctrl/Cmd + L` - Load preset
- `1-8` - Select scene
- `+/-` - Next/Previous scene

## Known Limitations

1. **NAM Inference:** Currently placeholder - real-time neural processing not yet implemented
2. **API Integration:** Tone3000 API not yet implemented - local mode only
3. **Multi-output:** Currently mono/stereo only - parallel processing chains not supported
4. **Automation Recording:** Parameter history not recorded
5. **Undo/Redo:** Not implemented in current version

## Troubleshooting

### Plugin Not Appearing in DAW
- Verify installation path is in DAW's plugin directory
- Rescan plugin cache
- Check plugin file permissions
- Verify 32-bit/64-bit architecture match

### No Sound Output
- Verify Master Output Level is not at -∞
- Check if All processors are bypassed
- Verify input signal is reaching plugin
- Check DAW's audio input/output routing

### High CPU Usage
- Bypass reverb or increase damping
- Reduce IR file size
- Disable unnecessary processors
- Check for excessive scene switching

### Crackling/Artifacts
- Increase DAW buffer size
- Reduce delay feedback
- Check for NAM model mismatches
- Verify IR file is valid WAV

### File Loading Fails
- Ensure file paths use valid characters
- Check file format (.nam for models, .wav for IRs)
- Verify file is not corrupted
- Ensure read permissions on file

## Support

For issues, feature requests, or questions:
https://github.com/leo42066/BlackTooth/issues

## Version History

### v1.0.0 (Initial Release)
- Core DSP processors (Gate, Overdrive, NAM, Cab IR, Delay, Reverb)
- 8-scene system
- Tone3000 local file integration
- JSON preset system
- Quad Cortex-inspired UI
- VST3 plugin format

## License

BlackTooth VST3 Plugin
Proprietary © BlackTooth Audio

All code is original. No proprietary third-party code included.
