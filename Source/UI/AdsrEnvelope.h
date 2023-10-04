/*
  ==============================================================================

    AdsrEnvelope.h
    Created: 4 Oct 2023 7:34:47pm
    Author:  antne

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class AdsrEnvelope  : public juce::Component
{
public:
    AdsrEnvelope(SamplerAudioProcessor&);
    ~AdsrEnvelope() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Path path;
    float attackTime;
    float decayTime;
    float sustainLevel;
    float releaseTime;
    juce::Colour attackColor;

    SamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrEnvelope)
};
