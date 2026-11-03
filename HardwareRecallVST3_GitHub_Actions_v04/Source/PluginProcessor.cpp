#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Inventory.h"

HardwareRecallAudioProcessor::HardwareRecallAudioProcessor()
    : AudioProcessor (BusesProperties().withInput ("Input", juce::AudioChannelSet::stereo(), true)
                                        .withOutput ("Output", juce::AudioChannelSet::stereo(), true))
{
    for (const auto& item : inventory::all())
        machines.push_back ({item.layout, juce::String::fromUTF8 (item.name)});
}
bool HardwareRecallAudioProcessor::isBusesLayoutSupported (const BusesLayout& l) const
{
    return l.getMainInputChannelSet() == l.getMainOutputChannelSet()
        && (l.getMainInputChannelSet() == juce::AudioChannelSet::mono()
         || l.getMainInputChannelSet() == juce::AudioChannelSet::stereo());
}
void HardwareRecallAudioProcessor::changed()
{
    updateHostDisplay (juce::AudioProcessor::ChangeDetails().withNonParameterStateChanged (true));
}
void HardwareRecallAudioProcessor::selectMachine (int index)
{
    if (juce::isPositiveAndBelow (index, machineCount())) { selected = index; changed(); }
}
void HardwareRecallAudioProcessor::addMachine (juce::String name)
{
    name = name.trim();
    if (name.isEmpty() || machines.size() >= 1024) return;
    machines.push_back ({name, name});
    selected = machineCount()-1; changed();
}
void HardwareRecallAudioProcessor::setKnob (int m, int i, float v)
{
    if (! juce::isPositiveAndBelow (m, machineCount()) || ! juce::isPositiveAndBelow (i, 12)) return;
    machines[(size_t)m].knobs[(size_t)i] = juce::jlimit (0.0f, 1.0f, v); changed();
}
void HardwareRecallAudioProcessor::setChoice (int m, int i, int v)
{
    if (! juce::isPositiveAndBelow (m, machineCount()) || ! juce::isPositiveAndBelow (i, 8)) return;
    machines[(size_t)m].choices[(size_t)i] = juce::jmax (0, v); changed();
}
void HardwareRecallAudioProcessor::getStateInformation (juce::MemoryBlock& dst)
{
    juce::XmlElement root ("HARDWARE_RECALL");
    root.setAttribute ("version", 2);
    root.setAttribute ("selected", selected);
    for (const auto& m : machines)
    {
        auto* el = root.createNewChildElement ("MACHINE");
        el->setAttribute ("id", m.id); el->setAttribute ("name", m.name);
        for (int i = 0; i < 12; ++i)
        {
            auto* c = el->createNewChildElement ("KNOB");
            c->setAttribute ("value", (double) m.knobs[(size_t)i]);
        }
        for (int i = 0; i < 8; ++i)
        {
            auto* c = el->createNewChildElement ("SWITCH");
            c->setAttribute ("index", m.choices[(size_t)i]);
        }
    }
    copyXmlToBinary (root, dst);
}
void HardwareRecallAudioProcessor::setStateInformation (const void* data, int size)
{
    auto root = getXmlFromBinary (data, size);
    if (root == nullptr || ! root->hasTagName ("HARDWARE_RECALL")) return;
    std::vector<Machine> restored;
    const int version = root->getIntAttribute ("version", 1);
    for (auto* el : root->getChildIterator())
    {
        if (! el->hasTagName ("MACHINE")) continue;
        Machine m;
        m.name = el->getStringAttribute ("name", "Machine");
        m.id = el->getStringAttribute ("id", m.name);
        int ki = 0, si = 0;
        for (auto* c : el->getChildIterator())
        {
            if (c->hasTagName ("KNOB") && ki < 12)
                m.knobs[(size_t)ki++] = juce::jlimit (0.f, 1.f, (float)c->getDoubleAttribute ("value"));
            if (c->hasTagName ("SWITCH") && si < 8)
                m.choices[(size_t)si++] = version >= 2 ? c->getIntAttribute ("index", 0)
                                                                : (c->getBoolAttribute ("value") ? 1 : 0);
        }
        restored.push_back (std::move(m));
        if (restored.size() >= 1024) break;
    }
    if (! restored.empty())
    {
        machines = std::move(restored);
        selected = juce::jlimit (0, machineCount()-1, root->getIntAttribute ("selected"));
    }
}
juce::AudioProcessorEditor* HardwareRecallAudioProcessor::createEditor()
{
    return new HardwareRecallAudioProcessorEditor (*this);
}
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() { return new HardwareRecallAudioProcessor(); }
