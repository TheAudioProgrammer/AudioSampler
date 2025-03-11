/*
  ==============================================================================

    WaveThumbnail.h
    Created: 4 Apr 2020 5:47:38pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <juce_graphics/juce_graphics.h>
#include "PluginProcessor.h"

//==============================================================================
/*
*/
class WaveThumbnail : public juce::Component,
                      public juce::FileDragAndDropTarget
{
public:
    explicit WaveThumbnail (TapAudioSamplerAudioProcessor& p);
    ~WaveThumbnail() override;

    void paint (juce::Graphics&) override;
    void resized() override;
    
    bool isInterestedInFileDrag (const juce::StringArray& files) override;
    void filesDropped (const juce::StringArray& files, int x, int y) override;

private:
    std::vector<float> audioPoints;
    bool shouldBePainting { false };
    
    juce::String fileName { "" };
    
    TapAudioSamplerAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (WaveThumbnail)
};
