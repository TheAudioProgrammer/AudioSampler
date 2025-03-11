#pragma once

#include <juce_audio_processors/juce_audio_processors.h>
#include <juce_data_structures/juce_data_structures.h>
#include <juce_audio_formats/juce_audio_formats.h>

//==============================================================================
/**
*/
class TapAudioSamplerAudioProcessor  : public juce::AudioProcessor,
                                    public juce::ValueTree::Listener
{
public:
    //==============================================================================
    TapAudioSamplerAudioProcessor();
    ~TapAudioSamplerAudioProcessor() override;

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    juce::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const juce::String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const juce::String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    void loadFile();
    void loadFile (const juce::String& path);
    
    int getNumSamplerSounds() { return sampler.getNumSounds(); }
    juce::AudioBuffer<float>& getWaveForm();
    
    void updateADSR();
    juce::ADSR::Parameters& getAdsrParams() { return adsrParams; }
    
    juce::AudioProcessorValueTreeState& getValueTree() { return apvts; }
    std::atomic<bool>& isNoteCurrentlyPlayed() { return isNotePlayed; }
    std::atomic<int>& getSampleCount() { return sampleCount; }

private:
    juce::Synthesiser sampler;
    const int numVoices { 3 };
    
    
    juce::ADSR::Parameters adsrParams;
    
    juce::AudioFormatManager formatManager;
    
    
    juce::AudioProcessorValueTreeState apvts;
    juce::AudioProcessorValueTreeState::ParameterLayout createParameters();
    void valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property) override;
    
    std::atomic<bool> shouldUpdate { false };
    std::atomic<bool> isNotePlayed { false };
    std::atomic<int> sampleCount { 0 };
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TapAudioSamplerAudioProcessor)
};
