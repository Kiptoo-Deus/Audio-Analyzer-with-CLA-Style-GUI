#include "PluginEditor.h"

void CLALookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height,
    float sliderPos, float startAngle, float endAngle,
    juce::Slider&) {
    float radius = juce::jmin(width, height) * 0.4f;
    juce::Point<float> centre(x + width * 0.5f, y + height * 0.5f);
    float angle = startAngle + sliderPos * (endAngle - startAngle);

    // CLA metallic knob
    g.setGradientFill(juce::ColourGradient(juce::Colours::silver, centre.x, centre.y - radius,
        juce::Colours::darkgrey, centre.x, centre.y + radius, true));
    g.fillEllipse(centre.x - radius, centre.y - radius, radius * 2, radius * 2);

    // Pointer
    juce::Path pointer;
    float pointerLength = radius * 0.8f;
    pointer.startNewSubPath(centre.x, centre.y - pointerLength);
    pointer.lineTo(centre.x, centre.y - radius * 0.2f);
    g.setColour(juce::Colours::black);
    g.strokePath(pointer, juce::PathStrokeType(2.0f), juce::AffineTransform::rotation(angle, centre.x, centre.y));
}

AudioAnalyzerCLAEditor::AudioAnalyzerCLAEditor(AudioAnalyzerCLA& p)
    : AudioProcessorEditor(&p), processor(p), spectrumBars(processor.numBars) {
    lnf.setColour(foleys::LevelMeter::lmMeterGradientLowColour, juce::Colours::green);
    lnf.setColour(foleys::LevelMeter::lmMeterGradientMidColour, juce::Colours::yellow);
    lnf.setColour(foleys::LevelMeter::lmMeterGradientMaxColour, juce::Colours::red);
    lnf.setColour(foleys::LevelMeter::lmBackgroundColour, juce::Colour(0xff2a2a2a));
    lnf.setColour(foleys::LevelMeter::lmOutlineColour, juce::Colours::grey);
    meter.setLookAndFeel(&lnf);

    gainKnob.setSliderStyle(juce::Slider::Rotary);
    gainKnob.setRange(0.0, 2.0, 0.01);
    gainKnob.setValue(1.0);
    gainKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    gainKnob.setLookAndFeel(&claLnf);
    addAndMakeVisible(gainKnob);

    rangeKnob.setSliderStyle(juce::Slider::Rotary);
    rangeKnob.setRange(-60.0f, 0.0f, 1.0f);
    rangeKnob.setValue(-24.0f);
    rangeKnob.setTextBoxStyle(juce::Slider::NoTextBox, false, 0, 0);
    rangeKnob.setLookAndFeel(&claLnf);
    addAndMakeVisible(rangeKnob);

    gainLabel.setText("GAIN", juce::dontSendNotification);
    gainLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    gainLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(gainLabel);

    rangeLabel.setText("RANGE", juce::dontSendNotification);
    rangeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    rangeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(rangeLabel);

    metersLabel.setText("METERS", juce::dontSendNotification);
    metersLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    metersLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(metersLabel);

    spectrumLabel.setText("SPECTRUM", juce::dontSendNotification);
    spectrumLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    spectrumLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(spectrumLabel);

    scopeLabel.setText("SCOPE", juce::dontSendNotification);
    scopeLabel.setColour(juce::Label::textColourId, juce::Colours::white);
    scopeLabel.setJustificationType(juce::Justification::centred);
    addAndMakeVisible(scopeLabel);

    meter.setMeterSource(&processor.getMeterSource());
    addAndMakeVisible(meter);

    setSize(800, 600);
    startTimerHz(30);
}

AudioAnalyzerCLAEditor::~AudioAnalyzerCLAEditor() {
    meter.setLookAndFeel(nullptr);
    gainKnob.setLookAndFeel(nullptr);
    rangeKnob.setLookAndFeel(nullptr);
    stopTimer();
}

void AudioAnalyzerCLAEditor::paint(juce::Graphics& g) {
    g.fillAll(juce::Colour(0xff2a2a2a)); // CLA dark gray

    // Spectrum bars
    const auto& magnitudes = processor.getSpectrum();
    float spectrumHeight = 100.0f;
    for (int i = 0; i < spectrumBars.size(); ++i) {
        float height = juce::jmap(magnitudes[i], 0.0f, 0.1f, 0.0f, spectrumHeight);
        spectrumBars[i].setSize(15, height);
        g.setGradientFill(juce::ColourGradient(juce::Colours::blue, spectrumBars[i].getX(), spectrumBars[i].getBottom(),
            juce::Colours::cyan, spectrumBars[i].getX(), spectrumBars[i].getY(), false));
        g.fillRect(spectrumBars[i]);
    }

    // Scope
    const auto& scopeData = processor.getScopeBuffer();
    int scopeSamples = scopeData.getNumSamples();
    float scopeHeight = 100.0f;
    float scopeY = 450.0f;
    scopePath.clear();
    for (int i = 0; i < scopeSamples - 1; ++i) {
        float x1 = juce::jmap(float(i), 0.0f, float(scopeSamples), 300.0f, 700.0f);
        float y1 = juce::jmap(scopeData.getSample(0, i), -1.0f, 1.0f, scopeY + scopeHeight, scopeY);
        float x2 = juce::jmap(float(i + 1), 0.0f, float(scopeSamples), 300.0f, 700.0f);
        float y2 = juce::jmap(scopeData.getSample(0, (i + 1) % scopeSamples), -1.0f, 1.0f, scopeY + scopeHeight, scopeY);
        if (i == 0) scopePath.startNewSubPath(x1, y1);
        else scopePath.lineTo(x2, y2);
    }
    g.setColour(juce::Colours::yellow);
    g.strokePath(scopePath, juce::PathStrokeType(1.0f));

    g.setColour(juce::Colours::grey);
    g.drawRect(getLocalBounds(), 2.0f);
}

void AudioAnalyzerCLAEditor::resized() {
    gainKnob.setBounds(50, 50, 80, 80);
    gainLabel.setBounds(50, 130, 80, 20);
    rangeKnob.setBounds(150, 50, 80, 80);
    rangeLabel.setBounds(150, 130, 80, 20);
    metersLabel.setBounds(300, 20, 60, 20);
    meter.setBounds(300, 50, 60, 200);
    spectrumLabel.setBounds(300, 270, 100, 20);
    for (int i = 0; i < spectrumBars.size(); ++i) {
        spectrumBars[i].setPosition(300 + i * 20, 300);
    }
    scopeLabel.setBounds(300, 420, 60, 20);
}