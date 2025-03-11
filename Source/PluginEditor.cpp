
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapAudioSamplerAudioProcessorEditor::TapAudioSamplerAudioProcessorEditor (TapAudioSamplerAudioProcessor& p)
    : AudioProcessorEditor (&p), mWaveThumbnail (p), mADSR (p), processor (p)
{
    // auto tapImage = juce::ImageCache::getFromMemory (juce::BinaryData::TAP_png, juce::BinaryData::TAP_pngSize);
    
    // if (! tapImage.isNull())
    //     mImageComponent.setImage (tapImage, juce::RectanglePlacement::stretchToFit);
    // else
    //     jassert (! tapImage.isNull());
    
    addAndMakeVisible (mWaveThumbnail);
    addAndMakeVisible (mADSR);
    addAndMakeVisible (mImageComponent);
    
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
    mWaveThumbnail.setBoundsRelative (0.0f, 0.25f, 1.0f, 0.5);
    mADSR.setBoundsRelative (0.0f, 0.75f, 1.0f, 0.25f);
    mImageComponent.setBoundsRelative (0.0f, 0.0f, 0.5f, 0.25f);
}

void TapAudioSamplerAudioProcessorEditor::timerCallback()
{
    repaint();
}



