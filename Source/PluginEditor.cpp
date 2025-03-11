
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapAudioSamplerAudioProcessorEditor::TapAudioSamplerAudioProcessorEditor (TapAudioSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), thumbnail (p), adsr (p), processor (p)
{
    auto tapImage = juce::ImageCache::getFromMemory(BinaryData::TapLogo_png, BinaryData::TapLogo_pngSize);
    
    if (! tapImage.isNull())
        imageComponent.setImage (tapImage, juce::RectanglePlacement::fillDestination);
    else
        jassert (! tapImage.isNull());
    
    addAndMakeVisible (thumbnail);
    addAndMakeVisible (adsr);
    addAndMakeVisible (imageComponent);
    
    startTimerHz (30);
    
    setSize (600, 400);
}

TapAudioSamplerAudioProcessorEditor::~TapAudioSamplerAudioProcessorEditor()
{
    stopTimer();
}

//==============================================================================
void TapAudioSamplerAudioProcessorEditor::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void TapAudioSamplerAudioProcessorEditor::resized()
{
    thumbnail.setBoundsRelative (0.0f, 0.25f, 1.0f, 0.5);
    adsr.setBoundsRelative (0.0f, 0.75f, 1.0f, 0.25f);
    imageComponent.setBoundsRelative (0.0f, 0.0f, 0.5f, 0.25f);
}

void TapAudioSamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}



