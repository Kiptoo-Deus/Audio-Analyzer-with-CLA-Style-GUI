#include "PluginEditor.h"

AudioAnalyzerCLAEditor::AudioAnalyzerCLAEditor(AudioAnalyzerCLA& p)
    : AudioProcessorEditor(&p), processor(p) {
    // Customize CLA look
    lnf.setColour(foleys::LevelMeter::lmMeterGradientLowColour, juce::Colours::green);
    lnf.setColour(foleys::LevelMeter::lmMeterGradientMaxColour, juce::Colours::red);
    meter.setLookAndFeel(&lnf);

    gainKnob.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setRange(0.0, 2.0, 0.01);
    gainKnob.setValue(1.0);
    gainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(gainKnob);

    rangeKnob.setSliderStyle(juce::Slider::Rotary);
    rangeKnob.setRange(-60.0f, 0.0f, 1.0f);
    rangeKnob.setValue(-24.0f);
    rangeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(rangeKnob);

    gainLabel.setText("GAIN", juce::dontSendNotification);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    rangeLabel.setText("RANGE", juce::dontSendNotification);
    rangeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    rangeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rangeLabel);

    meter.setMeterSource(&processor.getMeterSource());
    addAndMakeVisible(meter);

    setSize(800, 600);
    startTimerHz(30); // 30 FPS refresh
}

AudioAnalyzerCLAEditor::~AudioAnalyzerCLAEditor() {
    meter.setLookAndFeel(nullptr);
    stopTimer();
}

void AudioAnalyzerCLAEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(0xff2a2a2a)); // dark gray
    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 2.0f); // Border
}

void AudioAnalyzerCLAEditor::resized() {
    gainKnob.setBounds(50, 50, 80, 80);
    gainLabel.setBounds(50, 130, 80, 20);
    rangeKnob.setBounds(150, 50, 80, 80);
    rangeLabel.setBounds(150, 130, 80, 20);
    meter.setBounds(300, 50, 60, 200); // Stereo meters
}