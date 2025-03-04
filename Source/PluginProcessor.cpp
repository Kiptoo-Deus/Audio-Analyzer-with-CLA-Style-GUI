#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioAnalyzerCLA::AudioAnalyzerCLA()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    inputBuffer(2, 512) {}

AudioAnalyzerCLA::~AudioAnalyzerCLA() {}

void AudioAnalyzerCLA::prepareToPlay(double sampleRate, int samplesPerBlock) {
    meterSource.resize(2, sampleRate * 0.1 / samplesPerBlock); // 100ms smoothing
    inputBuffer.setSize(2, samplesPerBlock);
}

void AudioAnalyzerCLA::releaseResources() {}

void AudioAnalyzerCLA::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;
    auto* inData = buffer.getArrayOfReadPointers();
    auto* outData = buffer.getArrayOfWritePointers();
    int numSamples = buffer.getNumSamples();

    for (int i = 0; i < numSamples; ++i) {
        float sampleL = inData[0][i];
        float sampleR = inData[1][i];
        inputBuffer.setSample(0, i, sampleL);
        inputBuffer.setSample(1, i, sampleR);
        outData[0][i] = sampleL; // Pass-through
        outData[1][i] = sampleR;
    }

    meterSource.measureBlock(inputBuffer);
}

juce::AudioProcessorEditor* AudioAnalyzerCLA::createEditor() {
    return new AudioAnalyzerCLAEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioAnalyzerCLA();
}