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
	public juce::FileDragAndDropTarget,
	public juce::Slider::Listener
{
public:
	SamplerAudioProcessorEditor(SamplerAudioProcessor&);
	~SamplerAudioProcessorEditor() override;

	//==============================================================================
	void paint(juce::Graphics&) override;
	void resized() override;

	bool isInterestedInFileDrag(const juce::StringArray& files) override;
	void filesDropped(const juce::StringArray& files, int x, int y) override;

	void sliderValueChanged(juce::Slider* slider) override;

private:
	juce::TextButton loadButton{ "Load" };

	std::vector<float> audioPoints;

	bool shouldRepaint{ false };

	juce::AudioBuffer<float> waveform;

	juce::Path waveformPath;

	// Sliders
	juce::Slider attackSlider;
	juce::Label attackLabel;

	juce::Slider decaySlider;
	juce::Label decayLabel;

	juce::Slider sustainSlider;
	juce::Label sustainLabel;

	juce::Slider releaseSlider;
	juce::Label releaseLabel;

	void initialiseSlider(juce::Slider& slider, double minimumRange, double maximumRange, juce::Label& label, const std::string& labelText);

	// This reference is provided as a quick way for your editor to
	// access the processor object that created it.
	SamplerAudioProcessor& audioProcessor;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessorEditor)
};
