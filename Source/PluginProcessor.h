#pragma once
#include <JuceHeader.h>
#include "ff_meters/ff_meters.h"

class AudioAnalyzerCLA : public juce::AudioProcessor {
public:
    AudioAnalyzerCLA();
    ~AudioAnalyzerCLA() override;

    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override { return true; }

    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return false; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override {}
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override {}
    void getStateInformation(juce::MemoryBlock&) override {}
    void setStateInformation(const void*, int) override {}

    foleys::LevelMeterSource& getMeterSource() { return meterSource; }
    const std::vector<float>& getSpectrum() const { return spectrumMagnitudes; }
    const juce::AudioBuffer<float>& getScopeBuffer() const { return scopeBuffer; } // Expose scope
    static const int numBars = 20;
private:
    foleys::LevelMeterSource meterSource;
    juce::AudioBuffer<float> inputBuffer;
    juce::dsp::FFT fft{ 10 }; // 1024 points
    std::vector<float> fftBuffer;
    std::vector<float> spectrumMagnitudes;
    juce::AudioBuffer<float> scopeBuffer; // Scope data
    size_t scopePos = 0;
  

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(AudioAnalyzerCLA)
};