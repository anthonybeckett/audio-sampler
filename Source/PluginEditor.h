/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class SamplerAudioProcessorEditor : public juce::AudioProcessorEditor,
                                    public juce::FileDragAndDropTarget
{
public:
	SamplerAudioProcessorEditor(SamplerAudioProcessor&);
	~SamplerAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	bool isInterestedInFileDrag(const juce::StringArray& files) override;
	void filesDropped(const juce::StringArray& files, int x, int y) override;

private:
	juce::TextButton loadButton{"Load"};

	std::vector<float> audioPoints;

	bool shouldRepaint{ false };

	juce::AudioBuffer<float> waveform;

	juce::Path waveformPath;

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	SamplerAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessorEditor)
};
