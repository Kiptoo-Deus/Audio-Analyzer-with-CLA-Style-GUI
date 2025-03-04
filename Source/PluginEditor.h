#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ff_meters/ff_meters.h"

class AudioAnalyzerCLAEditor : public juce::AudioProcessorEditor, public juce::Timer {
public:
    AudioAnalyzerCLAEditor(AudioAnalyzerCLA&);
    ~AudioAnalyzerCLAEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override { meter.repaint(); } // Refresh meter

private:
    AudioAnalyzerCLA& processor;
    juce::Slider gainKnob;
    juce::Slider rangeKnob;
    juce::Label gainLabel;
    juce::Label rangeLabel;
    foleys::LevelMeter meter{ foleys::LevelMeter::Default }; // Stereo meters
    foleys::LevelMeterLookAndFeel lnf; 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioAnalyzerCLAEditor)
};