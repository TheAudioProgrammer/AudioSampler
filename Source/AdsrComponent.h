/*
  ==============================================================================

    AdsrComponent.h
    Created: 4 Apr 2020 7:35:53pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#pragma once

#include <juce_graphics/juce_graphics.h>
#include "PluginProcessor.h"


//==============================================================================
/*
*/
class AdsrComponent : public juce::Component
{
public:
    explicit AdsrComponent (TapAudioSamplerAudioProcessor& p);

    void paint (juce::Graphics&) override;
    void resized() override;

private:
    juce::Slider mAttackSlider, mDecaySlider, mSustainSlider, mReleaseSlider;
    juce::Label mAttackLabel, mDecayLabel, mSustainLabel, mReleaseLabel;
    
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mAttackAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mDecayAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mSustainAttachment;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> mReleaseAttachment;
    
    TapAudioSamplerAudioProcessor& processor;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AdsrComponent)
};
