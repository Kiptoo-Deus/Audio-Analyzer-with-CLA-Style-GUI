#include "MainComponent.h"

MainComponent::MainComponent() {
    // Gain knob
    gainKnob.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setRange(0.0, 2.0, 0.01);
    gainKnob.setValue(1.0);
    gainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(gainKnob);

    // Range knob
    rangeKnob.setSliderStyle(juce::Slider::Rotary);
    rangeKnob.setRange(-60.0, 0.0, 1.0);
    rangeKnob.setValue(-24.0);
    rangeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    addAndMakeVisible(rangeKnob);

    // Labels
    gainLabel.setText("GAIN", juce::dontSendNotification);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    rangeLabel.setText("RANGE", juce::dontSendNotification);
    rangeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    rangeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rangeLabel);

    setSize(800, 600);
}

MainComponent::~MainComponent() {}

void MainComponent::paint(juce::Graphics& g) {

    g.fillAll(juce::Colour(0xff2a2a2a)); // Dark gray

    // Meters (placeholders for now)
    g.setColour(juce::Colours::green);
    g.fillRect(meterL); // Left meter
    g.fillRect(meterR); // Right meter

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 2.0f); // Border
}

void MainComponent::resized() {
    gainKnob.setBounds(50, 50, 100, 100);
    gainLabel.setBounds(50, 150, 100, 20);
    rangeKnob.setBounds(170, 50, 100, 100);
    rangeLabel.setBounds(170, 150, 100, 20);
    meterL.setBounds(300, 50, 30, 200); // Left meter
    meterR.setBounds(340, 50, 30, 200); // Right meter
}