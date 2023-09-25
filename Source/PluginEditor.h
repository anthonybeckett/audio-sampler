/*
  ==============================================================================

	This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "./UI/WaveThumbnail.h"
#include "./UI/AdsrComponent.h"

//==============================================================================
/**
*/
class SamplerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
	SamplerAudioProcessorEditor(SamplerAudioProcessor&);
	~SamplerAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

private:
	WaveThumbnail waveThumbnail;

	AdsrComponent adsrComponent;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	SamplerAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessorEditor)
};
