/*
  ==============================================================================

    AdsrComponent.cpp
    Created: 4 Apr 2020 7:35:53pm
    Author:  Joshua Hodge

  ==============================================================================
*/

#include "AdsrComponent.h"

//==============================================================================
AdsrComponent::AdsrComponent (TapAudioSamplerAudioProcessor& p) : processor (p)
{
    //Attack Slider
    mAttackSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mAttackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    mAttackSlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    //mAttackSlider.setRange (0.0f, 5.0f, 0.01f);
    addAndMakeVisible (mAttackSlider);
    
    mAttackLabel.setFont (10.0f);
    mAttackLabel.setText ("Attack", juce::NotificationType::dontSendNotification);
    mAttackLabel.setJustificationType (juce::Justification::centredTop);
    mAttackLabel.attachToComponent (&mAttackSlider, false);
    
    mAttackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "ATTACK", mAttackSlider);
    
    //Decay Slider
    mDecaySlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mDecaySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    mDecaySlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    //mDecaySlider.setRange (0.0f, 5.0f, 0.01f);
    addAndMakeVisible (mDecaySlider);
    
    mDecayLabel.setFont (10.0f);
    mDecayLabel.setText ("Decay", juce::NotificationType::dontSendNotification);
    mDecayLabel.setJustificationType (juce::Justification::centredTop);
    mDecayLabel.attachToComponent (&mDecaySlider, false);
    
    mDecayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "DECAY", mDecaySlider);
    
    //Sustain Slider
    mSustainSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mSustainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    mSustainSlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    //mSustainSlider.setRange (0.0f, 1.0f, 0.01f);
    addAndMakeVisible (mSustainSlider);
    
    mSustainLabel.setFont (10.0f);
    mSustainLabel.setText ("Sustain", juce::NotificationType::dontSendNotification);
    mSustainLabel.setJustificationType (juce::Justification::centredTop);
    mSustainLabel.attachToComponent (&mSustainSlider, false);
    
    mSustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "SUSTAIN", mSustainSlider);
    
    //Release Slider
    mReleaseSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    mReleaseSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    mReleaseSlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    //mReleaseSlider.setRange (0.0f, 5.0f, 0.01f);
    addAndMakeVisible (mReleaseSlider);
    
    mReleaseLabel.setFont (10.0f);
    mReleaseLabel.setText ("Release", juce::NotificationType::dontSendNotification);
    mReleaseLabel.setJustificationType (juce::Justification::centredTop);
    mReleaseLabel.attachToComponent (&mReleaseSlider, false);
    
    mReleaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "RELEASE", mReleaseSlider);
}

void AdsrComponent::paint (juce::Graphics& g)
{
    g.fillAll (juce::Colours::black);
}

void AdsrComponent::resized()
{
    const auto startX = 0.6f;
    const auto startY = 0.2f;
    const auto dialWidth = 0.1f;
    const auto dialHeight = 0.75f;
    
    mAttackSlider.setBoundsRelative (startX, startY, dialWidth, dialHeight);
    mDecaySlider.setBoundsRelative (startX + dialWidth, startY, dialWidth, dialHeight);
    mSustainSlider.setBoundsRelative (startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    mReleaseSlider.setBoundsRelative (startX + (dialWidth * 3), startY, dialWidth, dialHeight);
}
