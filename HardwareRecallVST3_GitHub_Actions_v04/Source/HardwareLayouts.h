#pragma once
#include <JuceHeader.h>
#include <vector>

// Coordinates refer to the 1100 x 310 front-panel drawing area.
namespace hardware
{
struct Knob { juce::String label; float x, y; double min, max, step; juce::String suffix; };
struct Selector { juce::String label; float x, y; juce::StringArray options; };
struct Layout { juce::String id, title, subtitle; juce::Colour face, accent; std::vector<Knob> knobs; std::vector<Selector> selectors; };
inline Layout forId (const juce::String& id)
{
    using K = Knob; using S = Selector;
    if (id == "distressor")
        return { id, "EMPIRICAL LABS  /  DISTRESSOR EL8", "Single-channel compressor  •  two independent units in the library", juce::Colour (0xff171b1c), juce::Colour (0xffefbd52),
            { K {"INPUT", 655, 171, 0, 10, .1, ""}, K {"ATTACK", 790, 171, 0, 10, .1, ""}, K {"RELEASE", 925, 171, 0, 10, .1, ""}, K {"OUTPUT", 1050, 171, 0, 10, .1, ""} },
            { S {"RATIO", 155, 184, {"1:1", "2:1", "3:1", "4:1", "6:1", "10:1", "20:1", "NUKE"}},
              S {"DETECTOR", 305, 184, {"NORMAL", "HP", "BAND EMPH", "LINK", "HP + LINK"}},
              S {"AUDIO", 455, 184, {"CLEAN", "HP", "DIST 2", "DIST 3"}},
              S {"BYPASS", 540, 265, {"OUT", "IN"}} } };
    if (id == "api2500")
        return { id, "API 2500  /  STEREO BUS COMPRESSOR", "Original 2500 layout (not 2500+): verify exact hardware revision", juce::Colour (0xff1b242b), juce::Colour (0xffc8e1f3),
            { K {"THRESHOLD", 105, 168, -20, 20, .5, " dBu"}, K {"ATTACK", 225, 168, 0, 6, 1, ""},
              K {"RATIO", 345, 168, 0, 6, 1, ""}, K {"RELEASE", 465, 168, 0, 6, 1, ""},
              K {"VARIABLE", 565, 168, 50, 3000, 10, " ms"}, K {"L/R LINK", 765, 168, 0, 100, 20, " %"},
              K {"OUTPUT", 925, 168, 0, 24, .5, " dB"} },
            { S {"KNEE", 145, 264, {"HARD", "MEDIUM", "SOFT"}}, S {"THRUST", 295, 264, {"NORM", "MED", "LOUD"}},
              S {"TYPE", 445, 264, {"OLD", "NEW"}}, S {"LINK SHAPE", 645, 264, {"FAST", "SLOW"}},
              S {"MAKEUP", 830, 264, {"AUTO", "MANUAL"}}, S {"COMP", 945, 264, {"OUT", "IN"}},
              S {"BYPASS", 1050, 264, {"OUT", "IN"}} } };
    if (id == "zener")
        return { id, "CHANDLER LIMITED  /  ZENER LIMITER", "Dual channel  •  independent LEFT and RIGHT sections", juce::Colour (0xffddd5bc), juce::Colour (0xff684b26),
            { K {"L INPUT", 95, 165, -10, 10, 1, ""}, K {"L OUTPUT", 220, 165, -10, 10, 1, " dB"},
              K {"L ATTACK", 345, 165, 0, 10, 1, ""}, K {"L RELEASE", 470, 165, 0, 20, 1, ""},
              K {"R INPUT", 635, 165, -10, 10, 1, ""}, K {"R OUTPUT", 760, 165, -10, 10, 1, " dB"},
              K {"R ATTACK", 885, 165, 0, 10, 1, ""}, K {"R RELEASE", 1010, 165, 0, 20, 1, ""} },
            { S {"L GAIN", 110, 268, {"LOW", "HIGH"}}, S {"L MODE", 260, 268, {"COMP 1", "COMP 2", "LIMIT", "THD"}},
              S {"L SIDECHAIN", 430, 268, {"OFF", "ON"}}, S {"R GAIN", 650, 268, {"LOW", "HIGH"}},
              S {"R MODE", 800, 268, {"COMP 1", "COMP 2", "LIMIT", "THD"}}, S {"R SIDECHAIN", 975, 268, {"OFF", "ON"}} } };
    // Unverified front panel: generic controls are placeholders, NOT a faithful reconstruction.
    Layout l { id, "PANEL NOT YET DOCUMENTED", "UNVERIFIED PANEL • controls are placeholders, not the actual front panel", juce::Colour (0xff27333e), juce::Colours::orange, {}, {} };
    for (int n = 0; n < 8; ++n) l.knobs.push_back ({"POT " + juce::String(n+1), 90.0f + 130.0f*(n%8), 150.0f, 0, 100, 1, "%"});
    for (int n = 0; n < 6; ++n) l.selectors.push_back ({"SW " + juce::String(n+1), 110.0f + 175.0f*n, 265.0f, {"OFF", "ON"}});
    return l;
}
}
