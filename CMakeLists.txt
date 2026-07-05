cmake_minimum_required(VERSION 3.21)

project(BlackTooth VERSION 1.0.0 LANGUAGES CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add JUCE as subdirectory (assumes JUCE is cloned to JUCE/ directory)
add_subdirectory(JUCE)

# Create the plugin target
juce_add_plugin(BlackTooth
    COMPANY_NAME "BlackTooth Audio"
    COMPANY_WEBSITE "https://blacktooth.audio"
    COMPANY_EMAIL "info@blacktooth.audio"
    PLUGIN_MANUFACTURER_CODE "BlTo"
    PLUGIN_CODE "BlT0"
    FORMATS VST3
    PRODUCT_NAME "BlackTooth"
    DESCRIPTION "Professional VST3 guitar processor plugin with NAM amp modeling and impulse response convolution."
    VST3_CATEGORIES "Instrument" "Plugin" "Guitar"
    LV2_URI "https://blacktooth.audio/blacktooth"
    AU_MAIN_TYPE "kAudioUnitType_Effect"
    PLUGIN_LAUNCH_NAME "BlackTooth"
)

# Add source files
target_sources(BlackTooth PRIVATE
    # DSP
    Source/DSP/GateProcessor.cpp
    Source/DSP/OverdriveProcessor.cpp
    Source/DSP/NamProcessor.cpp
    Source/DSP/IrCabProcessor.cpp
    Source/DSP/DelayProcessor.cpp
    Source/DSP/ReverbProcessor.cpp
    Source/DSP/SignalChain.cpp
    
    # UI
    Source/UI/MainEditor.cpp
    Source/UI/BlockComponent.cpp
    Source/UI/SceneBar.cpp
    Source/UI/FileBrowser.cpp
    Source/UI/KnobComponent.cpp
    Source/UI/LookAndFeel.cpp
    
    # Tone3000
    Source/Tone3000/Tone3000LocalSource.cpp
    Source/Tone3000/Tone3000ApiSource.cpp
    
    # Core
    Source/Core/PluginProcessor.cpp
    Source/Core/PluginEditor.cpp
    Source/Core/SceneManager.cpp
    Source/Core/PresetManager.cpp
)

# Link JUCE
target_link_libraries(BlackTooth
    PRIVATE
    juce::juce_audio_processors
    juce::juce_dsp
    juce::juce_gui_extra
    juce::juce_gui_basics
)

# Include directories
target_include_directories(BlackTooth PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/Source
)

# Platform-specific settings
if(MSVC)
    target_compile_options(BlackTooth PRIVATE /W4 /WX)
else()
    target_compile_options(BlackTooth PRIVATE -Wall -Wextra -Werror)
endif()

# Enable optimizations for release builds
target_compile_options(BlackTooth PRIVATE
    $<$<CONFIG:Release>:-O3>
)
