/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./UI/CustomLAF.h"
#include "./UI/WaveThumbnail.h"
#include "./UI/AdsrComponent.h"
#include "./UI/AdsrEnvelope.h"

//==============================================================================
/**
*/
class SamplerAudioProcessorEditor : public juce::AudioProcessorEditor, public juce::Timer
{
public:
	SamplerAudioProcessorEditor(SamplerAudioProcessor&);
	~SamplerAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	void timerCallback() override;

private:
	CustomLAF customLaf;

	WaveThumbnail waveThumbnail;

	AdsrEnvelope adsrEnvelope;

	AdsrComponent adsrComponent;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	SamplerAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessorEditor)
};
