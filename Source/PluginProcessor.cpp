/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessor::SamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
    apvts(*this, nullptr, "parameters", createParameters())
#endif
{
    apvts.state.addListener(this);

    audioFormatManager.registerBasicFormats();

    midiRange.setRange(0, 127, true);

    for (int i = 0; i < numVoices; i++)
    {
        sampler.addVoice(new juce::SamplerVoice);
    }
}

SamplerAudioProcessor::~SamplerAudioProcessor()
{
    //
}

//==============================================================================
const juce::String SamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String SamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void SamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void SamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sampler.setCurrentPlaybackSampleRate(sampleRate);

    updateAdsr();
}

void SamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void SamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    if(shouldUpdate)
    {
        updateAdsr();

        shouldUpdate = false;
    }

    juce::MidiMessage m;
    juce::MidiBuffer::Iterator it {midiMessages};
    int sample;

    while(it.getNextEvent(m, sample))
    {
	    if(m.isNoteOn())
	    {
            isNotePlayed = true;
	    }
        else if(m.isNoteOff())
        {
            isNotePlayed = false;
        }
    }

    sampleCount = isNotePlayed ? sampleCount += buffer.getNumSamples() : 0;

    sampler.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool SamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor (*this);
}

//==============================================================================
void SamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void SamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void SamplerAudioProcessor::loadFile()
{
    sampler.clearSounds();

    juce::FileChooser chooser{"Please load a file"};

    if(chooser.browseForFileToOpen())
    {
        auto file = chooser.getResult();

        auto audioFormatReader = audioFormatManager.createReaderFor(file);
        if (audioFormatReader)
        {
            sampler.addSound(new juce::SamplerSound("Sample", *audioFormatReader, midiRange, midiNoteC3, attackTime, releaseTime, maxTimeInSeconds));
            delete audioFormatReader; // reader is no longer needed, the SamplerSound already used it and won't need it further
        }
    }
}

void SamplerAudioProcessor::loadFile(const juce::String& path)
{
    sampler.clearSounds();

    auto file = juce::File(path);

    if(auto audioFormatReader = audioFormatManager.createReaderFor(file))
    {
        const int audioFileLengthInSamples = static_cast<int>(audioFormatReader->lengthInSamples);

        waveform.setSize(1, audioFileLengthInSamples);

        audioFormatReader->read(
            &waveform,
            0,
            audioFileLengthInSamples,
            0,
            true,
            false
        );

        sampler.addSound(new juce::SamplerSound("Sample", *audioFormatReader, midiRange, midiNoteC3, attackTime, releaseTime, maxTimeInSeconds));

        delete audioFormatReader;

        updateAdsr();
    }
}

int SamplerAudioProcessor::getNumSamplerSounds()
{
    return sampler.getNumSounds();
}

juce::AudioBuffer<float>& SamplerAudioProcessor::getWaveform()
{
    return waveform;
}

void SamplerAudioProcessor::updateAdsr()
{
    adsrParams.attack = apvts.getRawParameterValue("ATTACK")->load();
    adsrParams.decay = apvts.getRawParameterValue("DECAY")->load();
    adsrParams.sustain = apvts.getRawParameterValue("SUSTAIN")->load();
    adsrParams.release = apvts.getRawParameterValue("RELEASE")->load();

    for (int i = 0; i < sampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<juce::SamplerSound*>(sampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters(adsrParams);
        }
    }
}

juce::ADSR::Parameters& SamplerAudioProcessor::getAdsrParams()
{
    return adsrParams;
}

juce::AudioProcessorValueTreeState& SamplerAudioProcessor::getApvts()
{
    return apvts;
}

std::atomic<bool>& SamplerAudioProcessor::getIsNotePlayed()
{
    return isNotePlayed;
}

std::atomic<int>& SamplerAudioProcessor::getSampleCount()
{
    return sampleCount;
}

juce::AudioProcessorValueTreeState::ParameterLayout SamplerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> parameters;

    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 5.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    parameters.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 1.0f));

    return { parameters.begin(), parameters.end() };
}

void SamplerAudioProcessor::valueTreePropertyChanged(juce::ValueTree& treeWhosePropertyHasChanged, const juce::Identifier& property)
{
    shouldUpdate = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}
