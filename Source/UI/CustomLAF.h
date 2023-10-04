#pragma once

#include <JuceHeader.h>

class CustomLAF : public juce::LookAndFeel_V4
{
public:
	void drawLinearSlider(
		juce::Graphics&, 
		int x, 
		int y, 
		int width, 
		int height, 
		float sliderPos, 
		float minSliderPos, 
		float maxSliderPos, 
		const juce::Slider::SliderStyle, 
		juce::Slider&
	) override;
};