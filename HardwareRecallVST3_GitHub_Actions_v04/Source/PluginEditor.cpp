#include "PluginEditor.h"
#include <cmath>

HardwareRecallAudioProcessorEditor::HardwareRecallAudioProcessorEditor (HardwareRecallAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p), currentLayout (hardware::forId ("distressor"))
{
    setSize (1140, 515);
    addAndMakeVisible (picker);
    picker.onChange = [this] { if (!loading) { processor.selectMachine (picker.getSelectedItemIndex()); refresh(); } };
    addAndMakeVisible (newName);
    newName.setTextToShowWhenEmpty ("Nom d'une autre machine", juce::Colours::grey);
    addAndMakeVisible (addButton);
    addButton.onClick = [this]
    {
        if (newName.getText().trim().isNotEmpty())
        {
            processor.addMachine (newName.getText());
            newName.clear(); refresh();
        }
    };
    title.setFont (juce::Font (juce::FontOptions (20.0f).withStyle ("Bold")));
    title.setColour (juce::Label::textColourId, juce::Colours::whitesmoke);
    addAndMakeVisible (title);
    info.setText ("Les positions sont enregistrées dans l'état du VST3 avec ton projet REAPER • Audio inchangé", juce::dontSendNotification);
    info.setColour (juce::Label::textColourId, juce::Colour (0xffaab8c5));
    addAndMakeVisible (info);
    for (int i = 0; i < 12; ++i)
    {
        auto& s = knobs[(size_t)i];
        s.setSliderStyle (juce::Slider::RotaryHorizontalVerticalDrag);
        s.setTextBoxStyle (juce::Slider::TextBoxBelow, false, 85, 22);
        s.setColour (juce::Slider::rotarySliderFillColourId, juce::Colour (0xffe9ba6b));
        s.setColour (juce::Slider::rotarySliderOutlineColourId, juce::Colour (0xff596574));
        s.setColour (juce::Slider::thumbColourId, juce::Colours::white);
        s.onValueChange = [this, i]
        {
            if (loading || !juce::isPositiveAndBelow (i, (int)currentLayout.knobs.size())) return;
            const auto& spec = currentLayout.knobs[(size_t)i];
            processor.setKnob (processor.selectedMachine(), i,
                               (float)((knobs[(size_t)i].getValue() - spec.min) / (spec.max - spec.min)));
        };
        addAndMakeVisible (s);
        auto& l = knobLabels[(size_t)i];
        l.setJustificationType (juce::Justification::centred);
        l.setColour (juce::Label::textColourId, juce::Colours::whitesmoke);
        l.setFont (juce::Font (juce::FontOptions (12.0f).withStyle ("Bold")));
        addAndMakeVisible (l);
    }
    for (int i = 0; i < 8; ++i)
    {
        auto& c = choices[(size_t)i];
        c.onChange = [this, i]
        {
            if (!loading) processor.setChoice (processor.selectedMachine(), i, c.getSelectedItemIndex());
        };
        addAndMakeVisible (c);
        auto& l = choiceLabels[(size_t)i];
        l.setJustificationType (juce::Justification::centred);
        l.setColour (juce::Label::textColourId, juce::Colours::whitesmoke);
        l.setFont (juce::Font (juce::FontOptions (11.0f).withStyle ("Bold")));
        addAndMakeVisible (l);
    }
    refresh();
}
void HardwareRecallAudioProcessorEditor::refresh()
{
    juce::ScopedValueSetter<bool> lock (loading, true);
    picker.clear (juce::dontSendNotification);
    for (int i = 0; i < processor.machineCount(); ++i)
        picker.addItem (processor.getMachine(i).name, i+1);
    picker.setSelectedItemIndex (processor.selectedMachine(), juce::dontSendNotification);
    const auto& m = processor.getMachine (processor.selectedMachine());
    currentLayout = hardware::forId (m.id);
    title.setText (m.name + "   /   " + currentLayout.title, juce::dontSendNotification);
    for (int i = 0; i < 12; ++i)
    {
        const bool active = juce::isPositiveAndBelow (i, (int)currentLayout.knobs.size());
        knobs[(size_t)i].setVisible (active);
        knobLabels[(size_t)i].setVisible (active);
        if (!active) continue;
        const auto& spec = currentLayout.knobs[(size_t)i];
        auto& s = knobs[(size_t)i];
        s.setRange (spec.min, spec.max, spec.step);
        s.setTextValueSuffix (spec.suffix);
        s.setValue (spec.min + m.knobs[(size_t)i] * (spec.max-spec.min), juce::dontSendNotification);
        knobLabels[(size_t)i].setText (spec.label, juce::dontSendNotification);
    }
    for (int i = 0; i < 8; ++i)
    {
        const bool active = juce::isPositiveAndBelow (i, (int)currentLayout.selectors.size());
        choices[(size_t)i].setVisible (active);
        choiceLabels[(size_t)i].setVisible (active);
        if (!active) continue;
        auto& c = choices[(size_t)i];
        const auto& spec = currentLayout.selectors[(size_t)i];
        c.clear (juce::dontSendNotification);
        for (int n = 0; n < spec.options.size(); ++n) c.addItem (spec.options[n], n+1);
        c.setSelectedItemIndex (juce::jlimit (0, spec.options.size()-1, m.choices[(size_t)i]), juce::dontSendNotification);
        choiceLabels[(size_t)i].setText (spec.label, juce::dontSendNotification);
    }
    resized(); repaint();
}
void HardwareRecallAudioProcessorEditor::resized()
{
    picker.setBounds (22, 20, 420, 30);
    newName.setBounds (455, 20, 210, 30);
    addButton.setBounds (675, 20, 95, 30);
    title.setBounds (24, 73, 1070, 34);
    info.setBounds (25, 464, 1080, 26);
    for (size_t i = 0; i < currentLayout.knobs.size() && i < knobs.size(); ++i)
    {
        auto& k = currentLayout.knobs[i];
        const int cx = 20 + (int)k.x, cy = 132 + (int)k.y;
        knobLabels[i].setBounds (cx-53, cy-91, 106, 20);
        knobs[i].setBounds (cx-50, cy-66, 100, 116);
    }
    for (size_t i = 0; i < currentLayout.selectors.size() && i < choices.size(); ++i)
    {
        auto& s = currentLayout.selectors[i];
        const int cx = 20 + (int)s.x, cy = 132 + (int)s.y;
        choiceLabels[i].setBounds (cx-64, cy-55, 128, 18);
        choices[i].setBounds (cx-65, cy-32, 130, 27);
    }
}
void HardwareRecallAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colour (0xff121923));
    g.setColour (juce::Colour (0xff263442));
    g.fillRoundedRectangle (juce::Rectangle<float> (16, 124, 1108, 325), 12.f);
    g.setColour (currentLayout.face);
    g.fillRoundedRectangle (juce::Rectangle<float> (20, 132, 1100, 310), 6.f);
    g.setColour (currentLayout.accent);
    g.drawRoundedRectangle (juce::Rectangle<float> (20, 132, 1100, 310), 6.f, 2.f);
    // Rack screws, vector-drawn rather than embedding manufacturer artwork.
    g.setColour (juce::Colour (0xff75808c));
    for (int x : {36, 1104}) for (int y : {148, 425})
    {
        g.fillEllipse ((float)x-5, (float)y-5, 10.f, 10.f);
        g.setColour (juce::Colours::black.withAlpha (.75f));
        g.drawLine ((float)x-3, (float)y+3, (float)x+3, (float)y-3, 1.3f);
        g.setColour (juce::Colour (0xff75808c));
    }
    const bool light = currentLayout.id == "zener";
    auto ink = light ? juce::Colour (0xff49382a) : juce::Colour (0xffced8dd);
    g.setColour (ink);
    g.setFont (juce::Font (juce::FontOptions (13).withStyle ("Bold")));
    g.drawText (currentLayout.title, 68, 146, 950, 23, juce::Justification::centredLeft);
    g.setFont (juce::Font (juce::FontOptions (11)));
    g.drawText (currentLayout.subtitle, 68, 170, 920, 20, juce::Justification::centredLeft);
    if (currentLayout.id == "distressor")
    {
        g.setColour (juce::Colour (0xff293430));
        g.fillRoundedRectangle (juce::Rectangle<float> (84, 208, 510, 41), 4.f);
        for (int n = 0; n < 16; ++n)
        {
            g.setColour (n < 6 ? juce::Colour (0xff66a882) : n < 11 ? juce::Colour (0xffd5af54) : juce::Colour (0xffa95345));
            g.fillEllipse (97.f + n*29.f, 221.f, 7.f, 7.f);
        }
        g.setColour (ink);
        g.drawText ("GAIN REDUCTION  •  schematic LEDs (not live metering)", 88, 232, 505, 13, juce::Justification::centred);
    }
    if (currentLayout.id == "api2500")
    {
        g.setColour (juce::Colour (0xff334452));
        g.fillRoundedRectangle (juce::Rectangle<float> (626, 202, 192, 35), 4.f);
        g.setColour (ink);
        g.drawText ("TONE  /  LINK", 640, 205, 160, 25, juce::Justification::centred);
    }
    if (currentLayout.id == "zener")
    {
        g.setColour (juce::Colour (0xff88775c));
        g.drawLine (565, 202, 565, 420, 2.f);
        g.setFont (juce::Font (juce::FontOptions (12).withStyle ("Bold")));
        g.drawText ("LEFT", 65, 195, 450, 20, juce::Justification::centred);
        g.drawText ("RIGHT", 610, 195, 450, 20, juce::Justification::centred);
    }
}
