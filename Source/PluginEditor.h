
#pragma once

#include <juce_graphics/juce_graphics.h>
#include "BinaryData.h"
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
    WaveThumbnail thumbnail;
    AdsrComponent adsr;
    juce::ImageComponent imageComponent;
    
    TapAudioSamplerAudioProcessor& processor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapAudioSamplerAudioProcessorEditor)
};
