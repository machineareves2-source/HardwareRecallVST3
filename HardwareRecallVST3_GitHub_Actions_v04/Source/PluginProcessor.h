#pragma once
#include <JuceHeader.h>
#include <array>
#include <vector>

class HardwareRecallAudioProcessor final : public juce::AudioProcessor
{
public:
    struct Machine
    {
        juce::String id, name;
        std::array<float, 12> knobs {};
        std::array<int, 8> choices {};
    };
    HardwareRecallAudioProcessor();
    void prepareToPlay (double, int) override {}
    void releaseResources() override {}
    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override {}
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return "Hardware Recall"; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram (int) override {}
    const juce::String getProgramName (int) override { return {}; }
    void changeProgramName (int, const juce::String&) override {}
    bool isBusesLayoutSupported (const BusesLayout&) const override;
    void getStateInformation (juce::MemoryBlock&) override;
    void setStateInformation (const void*, int) override;
    int machineCount() const { return (int) machines.size(); }
    int selectedMachine() const { return selected; }
    const Machine& getMachine (int i) const { return machines.at ((size_t) i); }
    void selectMachine (int i);
    void addMachine (juce::String name);
    void setKnob (int machine, int index, float value);
    void setChoice (int machine, int index, int value);
private:
    std::vector<Machine> machines;
    int selected = 0;
    void changed();
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HardwareRecallAudioProcessor)
};
