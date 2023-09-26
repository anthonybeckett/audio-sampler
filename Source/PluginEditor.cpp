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
	adsrComponent(p)
{
    addAndMakeVisible(waveThumbnail);
    addAndMakeVisible(adsrComponent);

    startTimerHz(60);

    setSize (600, 400);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
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
    adsrComponent.setBoundsRelative(0.0f, 0.75f, 1.0f, 0.25f);
}

void SamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}


