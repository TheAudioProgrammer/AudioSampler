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
    attackSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    attackSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    attackSlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    addAndMakeVisible (attackSlider);
    
    attackLabel.setFont (10.0f);
    attackLabel.setText ("Attack", juce::NotificationType::dontSendNotification);
    attackLabel.setJustificationType (juce::Justification::centredTop);
    attackLabel.attachToComponent (&attackSlider, false);
    
    attackAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "ATTACK", attackSlider);
    
    //Decay Slider
    decaySlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    decaySlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    decaySlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    addAndMakeVisible (decaySlider);
    
    decayLabel.setFont (10.0f);
    decayLabel.setText ("Decay", juce::NotificationType::dontSendNotification);
    decayLabel.setJustificationType (juce::Justification::centredTop);
    decayLabel.attachToComponent (&decaySlider, false);
    
    decayAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "DECAY", decaySlider);
    
    //Sustain Slider
    sustainSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    sustainSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    sustainSlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    addAndMakeVisible (sustainSlider);
    
    sustainLabel.setFont (10.0f);
    sustainLabel.setText ("Sustain", juce::NotificationType::dontSendNotification);
    sustainLabel.setJustificationType (juce::Justification::centredTop);
    sustainLabel.attachToComponent (&sustainSlider, false);
    
    sustainAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "SUSTAIN", sustainSlider);
    
    //Release Slider
    releaseSlider.setSliderStyle (juce::Slider::SliderStyle::RotaryVerticalDrag);
    releaseSlider.setTextBoxStyle (juce::Slider::TextBoxBelow, true, 40, 20);
    releaseSlider.setColour (juce::Slider::ColourIds::thumbColourId, juce::Colours::red);
    addAndMakeVisible (releaseSlider);
    
    releaseLabel.setFont (10.0f);
    releaseLabel.setText ("Release", juce::NotificationType::dontSendNotification);
    releaseLabel.setJustificationType (juce::Justification::centredTop);
    releaseLabel.attachToComponent (&releaseSlider, false);
    
    releaseAttachment = std::make_unique<juce::AudioProcessorValueTreeState::SliderAttachment>(processor.getValueTree(), "RELEASE", releaseSlider);
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
    
    attackSlider.setBoundsRelative (startX, startY, dialWidth, dialHeight);
    decaySlider.setBoundsRelative (startX + dialWidth, startY, dialWidth, dialHeight);
    sustainSlider.setBoundsRelative (startX + (dialWidth * 2), startY, dialWidth, dialHeight);
    releaseSlider.setBoundsRelative (startX + (dialWidth * 3), startY, dialWidth, dialHeight);
}
