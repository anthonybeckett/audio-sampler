/*
  ==============================================================================

    AdsrComponent.h
    Created: 25 Sep 2023 9:24:01pm
    Author:  antne

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "../PluginProcessor.h"

//==============================================================================
/*
*/
class AdsrComponent  : public juce::Component
{
public:
    AdsrComponent(SamplerAudioProcessor&);
    ~AdsrComponent() override;

    void paint (juce::Graphics&) override;
    void resized() override;

private:
	// Sliders
	juce::Slider attackSlider;
	juce::Label attackLabel;

	juce::Slider decaySlider;
	juce::Label decayLabel;

	juce::Slider sustainSlider;
	juce::Label sustainLabel;

	juce::Slider releaseSlider;
	juce::Label releaseLabel;

	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> attackSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> decaySliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> sustainSliderAttachment;
	std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> releaseSliderAttachment;

	void initialiseSlider(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, const std::string& attachmentId, juce::Label& label, const std::string& labelText, double skewFactorFromMidPoint = 0.0);

	SamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
