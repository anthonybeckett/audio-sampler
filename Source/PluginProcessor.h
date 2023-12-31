/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/**
*/
class SamplerAudioProcessor  : public juce::AudioProcessor
                            #if JucePlugin_Enable_ARA
                             , public juce::AudioProcessorARAExtension
                            #endif
    , public juce::ValueTree::Listener
{
public:
    //==============================================================================
    SamplerAudioProcessor();
    ~SamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    void loadFile();
    void loadFile(const juce::String& path);

    int getNumSamplerSounds();

    juce::AudioBuffer<float>& getWaveform();

    //Temporary
    float attack{0.0f};
    float decay{ 0.0f };
    float sustain{ 0.0f };
    float release{ 0.0f };

    void updateAdsr();

    juce::ADSR::Parameters& getAdsrParams();

    juce::AudioProcessorValueTreeState& getApvts();

    std::atomic<bool>& getIsNotePlayed();

    std::atomic<int>& getSampleCount();

private:
    juce::Synthesiser sampler;
    const int numVoices{ 32 };

    juce::AudioFormatManager audioFormatManager;

    juce::BigInteger midiRange;

    const int midiNoteC3 = 60;
    double attackTime = 0.1;
    double releaseTime = 0.1;
    double maxTimeInSeconds = 10.0;

    juce::AudioBuffer<float> waveform;

    juce::ADSR::Parameters adsrParams;

    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();

    void valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property) override;

    std::atomic<bool> shouldUpdate{ false };

    std::atomic<bool> isNotePlayed{false};

    std::atomic<int> sampleCount{ 0 };

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SamplerAudioProcessor)
};
