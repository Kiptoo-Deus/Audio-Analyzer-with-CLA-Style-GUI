#pragma once
#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "ff_meters/ff_meters.h"

class CLALookAndFeel : public juce::LookAndFeel_V4 {
public:
    void drawRotarySlider(juce::Graphics&, int x, int y, int width, int height,
        float sliderPos, float startAngle, float endAngle,
        juce::Slider&) override;
};

class AudioAnalyzerCLAEditor : public juce::AudioProcessorEditor, public juce::Timer {
public:
    AudioAnalyzerCLAEditor(AudioAnalyzerCLA&);
    ~AudioAnalyzerCLAEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;
    void timerCallback() override { meter.repaint(); repaint(); }

private:
    AudioAnalyzerCLA& processor;
    juce::Slider gainKnob;
    juce::Slider rangeKnob;
    juce::Label gainLabel;
    juce::Label rangeLabel;
    juce::Label metersLabel;  
    juce::Label spectrumLabel;
    juce::Label scopeLabel;
    foleys::LevelMeter meter{ foleys::LevelMeter::Default };
    foleys::LevelMeterLookAndFeel lnf;
    std::vector<juce::Rectangle<float>> spectrumBars;
    juce::Path scopePath;
    CLALookAndFeel claLnf; // Custom knob style

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioAnalyzerCLAEditor)
};