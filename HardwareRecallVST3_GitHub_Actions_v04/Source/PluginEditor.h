#pragma once
#include "PluginProcessor.h"
#include "HardwareLayouts.h"

class HardwareRecallAudioProcessorEditor final : public juce::AudioProcessorEditor
{
public:
    explicit HardwareRecallAudioProcessorEditor (HardwareRecallAudioProcessor&);
    void paint (juce::Graphics&) override;
    void resized() override;
private:
    HardwareRecallAudioProcessor& processor;
    juce::ComboBox picker;
    juce::TextEditor newName;
    juce::TextButton addButton {"Ajouter"};
    juce::Label title, info;
    std::array<juce::Slider, 12> knobs;
    std::array<juce::Label, 12> knobLabels;
    std::array<juce::ComboBox, 8> choices;
    std::array<juce::Label, 8> choiceLabels;
    bool loading = false;
    hardware::Layout currentLayout;
    void refresh();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HardwareRecallAudioProcessorEditor)
};
