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

    addAndMakeVisible(loadButton);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (200, 200);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

//==============================================================================
void SamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (juce::Colours::black);
    g.setFont(15);

    if(audioProcessor.getNumSamplerSounds() > 0)
    {
        g.fillAll(juce::Colours::whitesmoke);
        g.setColour(juce::Colours::black);
        g.drawText("Sound Loaded", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }
    else
    {
        g.fillAll(juce::Colours::black);
        g.setColour(juce::Colours::white);
        g.drawText("Drg A File To Get Started", getWidth() / 2 - 50, getHeight() / 2 - 10, 100, 20, juce::Justification::centred);
    }
}

void SamplerAudioProcessorEditor::resized()
{
    //loadButton.setBounds(getWidth() / 2 - 50, getHeight() / 2 - 50, 100, 100);
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
            audioProcessor.loadFile(file);
	    }
    }

    repaint();
}
