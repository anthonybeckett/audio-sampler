/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p),
	audioProcessor (p),
	waveThumbnail(p),
	adsrComponent(p),
    adsrEnvelope(p)
{
    juce::LookAndFeel::setDefaultLookAndFeel(&customLaf);

    addAndMakeVisible(waveThumbnail);
    addAndMakeVisible(adsrComponent);
    addAndMakeVisible(adsrEnvelope);

    startTimerHz(60);

    setSize (600, 400);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
    juce::LookAndFeel::setDefaultLookAndFeel(nullptr);

    stopTimer();
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour(juce::Colours::whitesmoke);
}

void SamplerAudioProcessorEditor::resized()
{
    waveThumbnail.setBoundsRelative(0.0f, 0.25f, 1.0f, 0.5f);
    adsrComponent.setBoundsRelative(0.0f, 0.75f, 0.5f, 0.25f);
    adsrEnvelope.setBoundsRelative(0.5f, 0.75f, 0.5f, 0.25f);
}

void SamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}


