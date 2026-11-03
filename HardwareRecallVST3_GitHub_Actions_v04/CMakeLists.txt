cmake_minimum_required(VERSION 3.22)
project(HardwareRecall VERSION 0.1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
include(FetchContent)
FetchContent_Declare(juce GIT_REPOSITORY https://github.com/juce-framework/JUCE.git GIT_TAG 8.0.10 GIT_SHALLOW TRUE)
FetchContent_MakeAvailable(juce)
juce_add_plugin(HardwareRecall
    COMPANY_NAME "Hardware Recall"
    BUNDLE_ID "audio.hardwarerecall.plugin"
    PLUGIN_MANUFACTURER_CODE HdRc
    PLUGIN_CODE Hwr1
    FORMATS VST3
    PRODUCT_NAME "Hardware Recall"
    IS_SYNTH FALSE
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    IS_MIDI_EFFECT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE)
juce_generate_juce_header(HardwareRecall)
target_sources(HardwareRecall PRIVATE Source/PluginProcessor.cpp Source/PluginEditor.cpp)
target_compile_definitions(HardwareRecall PUBLIC JUCE_WEB_BROWSER=0 JUCE_USE_CURL=0 JUCE_VST3_CAN_REPLACE_VST2=0)
target_link_libraries(HardwareRecall PRIVATE juce::juce_audio_utils juce::juce_gui_extra juce::juce_audio_processors juce::juce_recommended_config_flags juce::juce_recommended_warning_flags)
