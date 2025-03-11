
#pragma once

#include <juce_graphics/juce_graphics.h>
#include "PluginProcessor.h"
#include "WaveThumbnail.h"
#include "ADSRComponent.h"

//==============================================================================
/**
*/
class TapAudioSamplerAudioProcessorEditor final : public juce::AudioProcessorEditor,
                                                  public juce::Timer
{
public:
    explicit TapAudioSamplerAudioProcessorEditor (TapAudioSamplerAudioProcessor&);
    ~TapAudioSamplerAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;
    
    void timerCallback() override;

private:    
    WaveThumbnail mWaveThumbnail;
    AdsrComponent mADSR;
    juce::ImageComponent mImageComponent;
    
    TapAudioSamplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapAudioSamplerAudioProcessorEditor)
};
