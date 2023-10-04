/*
  ==============================================================================

    AdsrEnvelope.cpp
    Created: 4 Oct 2023 7:34:47pm
    Author:  antne

  ==============================================================================
*/

#include <JuceHeader.h>
#include "AdsrEnvelope.h"

//==============================================================================
AdsrEnvelope::AdsrEnvelope(SamplerAudioProcessor& p)
    : audioProcessor(p)
{
    attackTime = 0.1f;
    decayTime = 0.2f;
    sustainLevel = 0.5f;
    releaseTime = 0.3f;

    attackColor = juce::Colour(0xffffff00);

}

AdsrEnvelope::~AdsrEnvelope()
{
}

void AdsrEnvelope::paint (juce::Graphics& g)
{
    auto& apvts = audioProcessor.getApvts();

    auto attack = apvts.getRawParameterValue("ATTACK")->load();
    auto decay = apvts.getRawParameterValue("DECAY")->load();
    auto sustain = apvts.getRawParameterValue("SUSTAIN")->load();
    auto release = apvts.getRawParameterValue("RELEASE")->load();

    auto attackLine = juce::jmap<float>(attack, 0.0f, 5.0f, 0.0f, getWidth()/4);
    auto decayLine = juce::jmap<float>(decay, 0.0f, 5.0f, 0.0f, getWidth()/3);
    auto sustainLine = juce::jmap<float>(sustain, 1.0f, 0.0f, 0.0f, getHeight()/2);
    auto releaseLine = juce::jmap<float>(release, 0.0f, 5.0f, 0.0f, getWidth()/4);

    path = juce::Path();
    path.startNewSubPath(0, getHeight());
    path.lineTo(attackLine, 0);
    path.lineTo(attackLine + decayLine, sustainLine + getHeight() / 2);
    path.lineTo(getWidth() - releaseLine, sustainLine + getHeight() / 2);
    path.lineTo(getWidth(), getHeight());

    g.setColour(attackColor);
    g.strokePath(path, juce::PathStrokeType(2.0f));
}

void AdsrEnvelope::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
