#pragma once
#include <JuceHeader.h>

class MainComponent : public juce::Component {
public:
    MainComponent();
    ~MainComponent() override;
    void paint(juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider gainKnob;        
    juce::Slider rangeKnob;       // Meter range knob
    juce::Label gainLabel;        // Knob labels
    juce::Label rangeLabel;
    juce::Rectangle<float> meterL; // Placeholder for meter bars
    juce::Rectangle<float> meterR;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(MainComponent)
};