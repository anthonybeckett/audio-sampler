/*
  ==============================================================================

    WaveThumbnail.h
    Created: 25 Sep 2023 8:13:23pm
    Author:  antne

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail  : public juce::Component, public juce::FileDragAndDropTarget
{
public:
    WaveThumbnail(SamplerAudioProcessor&);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;

    bool isInterestedInFileDrag(const juce::StringArray& files) override;
    void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
    std::vector<float> audioPoints;

    juce::AudioBuffer<float> waveform;

    juce::Path waveformPath;

    juce::String fileName {""};

    SamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
