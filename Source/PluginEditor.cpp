/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SamplerAudioProcessorEditor::SamplerAudioProcessorEditor (SamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    loadButton.onClick = [&]()
    {
        audioProcessor.loadFile();
    };

    initialiseSlider(attackSlider, attackSliderAttachment, "ATTACK", attackLabel, "Attack");
    initialiseSlider(decaySlider, decaySliderAttachment, "DECAY", decayLabel, "Decay");
    initialiseSlider(sustainSlider, sustainSliderAttachment, "SUSTAIN", sustainLabel, "Sustain");
    initialiseSlider(releaseSlider, releaseSliderAttachment, "RELEASE", releaseLabel, "Release");

    addAndMakeVisible(loadButton);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 200);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);

    g.setColour(juce::Colours::whitesmoke);

    juce::AudioBuffer<float> waveform = audioProcessor.getWaveform();

    if(waveform.getNumChannels() > 0)
    {
        audioPoints.clear();
        waveformPath.clear();

        int ratio = waveform.getNumSamples() / getWidth();
        auto buffer = waveform.getReadPointer(0);

        for (int sample = 0; sample < waveform.getNumSamples(); sample += ratio)
        {
            audioPoints.push_back(buffer[sample]);
        }

        waveformPath.startNewSubPath(0, static_cast<float>(getHeight()) / 2);

        for (int sample = 0; sample < audioPoints.size(); ++sample)
        {
            auto point = juce::jmap<float>(audioPoints[sample], -1.0f, 1.0f, 200.0f, 0.0f);

            waveformPath.lineTo(static_cast<float>(sample), point);
        }

        g.strokePath(waveformPath, juce::PathStrokeType(2));

        shouldRepaint = false;
    }
}

void SamplerAudioProcessorEditor::resized()
{
    const float startX = 0.6f;
    const float startY = 0.6f;
    const float sliderWidth = 0.1f;
    const float sliderHeight = 0.4f;

    attackSlider.setBoundsRelative(startX, startY, sliderWidth, sliderHeight);
    decaySlider.setBoundsRelative(startX + sliderWidth, startY, sliderWidth, sliderHeight);
    sustainSlider.setBoundsRelative(startX + sliderWidth * 2, startY, sliderWidth, sliderHeight);
    releaseSlider.setBoundsRelative(startX + sliderWidth * 3, startY, sliderWidth, sliderHeight);
}

bool SamplerAudioProcessorEditor::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto& file : files)
    {
	    if(file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
	    {
            return true;
	    }
    }

    return false;
}

void SamplerAudioProcessorEditor::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto& file : files)
    {
	    if(isInterestedInFileDrag(file))
	    {
            shouldRepaint = true;
            audioProcessor.loadFile(file);
	    }
    }

    repaint();
}

void SamplerAudioProcessorEditor::initialiseSlider(juce::Slider& slider, std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment>& sliderAttachment, const std::string& attachmentId, juce::Label& label, const std::string& labelText)
{
    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, 40, 20);
    sliderAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(audioProcessor.getApvts(), attachmentId, slider);
    addAndMakeVisible(slider);

    label.setFont(10.0f);
    label.setText(labelText, juce::dontSendNotification);
    label.setJustificationType(juce::Justification::centredTop);
    label.attachToComponent(&slider, false);
}
