#include <JuceHeader.h>
#include "WaveThumbnail.h"

//==============================================================================
WaveThumbnail::WaveThumbnail(SamplerAudioProcessor& p)
    : audioProcessor(p)
{
    //
}

WaveThumbnail::~WaveThumbnail()
{
}

void WaveThumbnail::paint (juce::Graphics& g)
{
    g.fillAll(juce::Colours::grey);

    juce::AudioBuffer<float> waveform = audioProcessor.getWaveform();

    if (waveform.getNumChannels() > 0)
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
            auto point = juce::jmap<float>(audioPoints[sample], -1.0f, 1.0f, getHeight(), 0.0f);

            waveformPath.lineTo(static_cast<float>(sample), point);
        }

        g.strokePath(waveformPath, juce::PathStrokeType(2));

        // Draw filename

        auto bounds = getLocalBounds().reduced(10, 10);

        g.setColour(juce::Colours::whitesmoke);
        g.setFont(15);
        g.drawFittedText(fileName, bounds, juce::Justification::topRight, 1);

        //Draw the playhead
        auto playheadPosition = juce::jmap<int>(audioProcessor.getSampleCount(), 0, audioProcessor.getWaveform().getNumSamples(), 0, getWidth());

        g.setColour(juce::Colours::white);
        g.drawLine(playheadPosition, 0, playheadPosition, getHeight(), 2.0f);

        g.setColour(juce::Colours::black.withAlpha(0.2f));
        g.fillRect(0, 0, playheadPosition, getHeight());
    }
	else
	{
        g.setColour(juce::Colours::white);
        g.setFont(30.0f);
        g.drawFittedText("Drop a file...", getLocalBounds(), juce::Justification::centred, 1);
	}
}

void WaveThumbnail::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}

bool WaveThumbnail::isInterestedInFileDrag(const juce::StringArray& files)
{
    for (auto& file : files)
    {
        if (file.contains(".wav") || file.contains(".mp3") || file.contains(".aif"))
        {
            return true;
        }
    }

    return false;
}

void WaveThumbnail::filesDropped(const juce::StringArray& files, int x, int y)
{
    for (auto& file : files)
    {
        if (isInterestedInFileDrag(file))
        {
            auto loadedFile = std::make_unique<juce::File>(file);
            fileName = loadedFile->getFileNameWithoutExtension();

            audioProcessor.loadFile(file);
        }
    }

    repaint();
}

