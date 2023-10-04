
#include <JuceHeader.h>
#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent(SamplerAudioProcessor& p)
    : audioProcessor(p)
{
    initialiseSlider(attackSlider, attackSliderAttachment, "ATTACK", attackLabel, "Attack", 1.5);
    initialiseSlider(decaySlider, decaySliderAttachment, "DECAY", decayLabel, "Decay", 1.5);
    initialiseSlider(sustainSlider, sustainSliderAttachment, "SUSTAIN", sustainLabel, "Sustain");
    initialiseSlider(releaseSlider, releaseSliderAttachment, "RELEASE", releaseLabel, "Release", 1.5);
}

AdsrComponent::~AdsrComponent()
{
}

void AdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::black);
}

void AdsrComponent::resized()
{
    const float startX = 0.6f;
    const float startY = 0.2f;
    const float sliderWidth = 0.1f;
    const float sliderHeight = 0.75f;

    attackSlider.setBoundsRelative(startX, startY, sliderWidth, sliderHeight);
    decaySlider.setBoundsRelative(startX + sliderWidth, startY, sliderWidth, sliderHeight);
    sustainSlider.setBoundsRelative(startX + sliderWidth * 2, startY, sliderWidth, sliderHeight);
    releaseSlider.setBoundsRelative(startX + sliderWidth * 3, startY, sliderWidth, sliderHeight);
}

void AdsrComponent::initialiseSlider(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, const std::string& attachmentId, juce::Label& label, const std::string& labelText, double skewFactorFromMidPoint)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::LinearVertical);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getApvts(), attachmentId, slider);
    slider.setColour(juce::Slider::trackColourId, juce::Colours::aqua);
    slider.setColour(juce::Slider::thumbColourId, juce::Colours::whitesmoke);

    if(skewFactorFromMidPoint > 0)
    {
        slider.setSkewFactorFromMidPoint(skewFactorFromMidPoint);
    }

    addAndMakeVisible(slider);

    label.setFont(14.0f);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredTop);
    label.attachToComponent(&slider, false);
}
