#include "PluginProcessor.h"
#include "PluginEditor.h"

AudioAnalyzerCLA::AudioAnalyzerCLA()
    : AudioProcessor(BusesProperties()
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    inputBuffer(2, 512), fftBuffer(fft.getSize() * 2), spectrumMagnitudes(numBars, 0.0f),
    scopeBuffer(1, 1024) { // Mono scope, 1024 samples
    scopeBuffer.clear();
}

AudioAnalyzerCLA::~AudioAnalyzerCLA() {}

void AudioAnalyzerCLA::prepareToPlay(double sampleRate, int samplesPerBlock) {
    meterSource.resize(2, sampleRate * 0.1 / samplesPerBlock);
    inputBuffer.setSize(2, samplesPerBlock);
    fftBuffer.resize(fft.getSize() * 2);
    spectrumMagnitudes.resize(numBars, 0.0f);
    scopeBuffer.setSize(1, 1024);
    scopeBuffer.clear();
    scopePos = 0;
}

void AudioAnalyzerCLA::releaseResources() {}

void AudioAnalyzerCLA::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer&) {
    juce::ScopedNoDenormals noDenormals;
    auto* inData = buffer.getArrayOfReadPointers();
    auto* outData = buffer.getArrayOfWritePointers();
    int numSamples = buffer.getNumSamples();

    // Fill input buffer and scope
    for (int i = 0; i < numSamples; ++i) {
        float sampleL = inData[0][i];
        float sampleR = inData[1][i];
        inputBuffer.setSample(0, i, sampleL);
        inputBuffer.setSample(1, i, sampleR);
        outData[0][i] = sampleL;
        outData[1][i] = sampleR;

        // Mono scope - average channels
        float scopeSample = (sampleL + sampleR) * 0.5f;
        scopeBuffer.setSample(0, scopePos, scopeSample);
        scopePos = (scopePos + 1) % scopeBuffer.getNumSamples();
    }

    meterSource.measureBlock(inputBuffer);

    // FFT
    int fftSize = fft.getSize();
    if (numSamples >= fftSize) {
        std::fill(fftBuffer.begin(), fftBuffer.end(), 0.0f);
        for (int i = 0; i < fftSize; ++i) {
            fftBuffer[i] = (inputBuffer.getSample(0, i) + inputBuffer.getSample(1, i)) * 0.5f;
        }
        fft.performRealOnlyForwardTransform(fftBuffer.data());
        for (int i = 0; i < numBars; ++i) {
            int binStart = i * (fftSize / 2) / numBars;
            int binEnd = (i + 1) * (fftSize / 2) / numBars;
            float mag = 0.0f;
            for (int j = binStart; j < binEnd; ++j) {
                float real = fftBuffer[2 * j];
                float imag = fftBuffer[2 * j + 1];
                mag += std::sqrt(real * real + imag * imag);
            }
            spectrumMagnitudes[i] = mag / (binEnd - binStart);
        }
    }
}

juce::AudioProcessorEditor* AudioAnalyzerCLA::createEditor() {
    return new AudioAnalyzerCLAEditor(*this);
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter() {
    return new AudioAnalyzerCLA();
}