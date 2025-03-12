
#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
TapAudioSamplerAudioProcessor::TapAudioSamplerAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts (*this, nullptr, "Parameters", createParameters())
#endif
{
    formatManager.registerBasicFormats();
    apvts.state.addListener (this);
    
    for (int i = 0; i < numVoices; i++)
    {
        sampler.addVoice (new juce::SamplerVoice());
    }
}

TapAudioSamplerAudioProcessor::~TapAudioSamplerAudioProcessor()
{
    apvts.state.removeListener (this);
}

//==============================================================================
const juce::String TapAudioSamplerAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TapAudioSamplerAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TapAudioSamplerAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TapAudioSamplerAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TapAudioSamplerAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TapAudioSamplerAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TapAudioSamplerAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TapAudioSamplerAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TapAudioSamplerAudioProcessor::getProgramName (int index)
{
    return {};
}

void TapAudioSamplerAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TapAudioSamplerAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    sampler.setCurrentPlaybackSampleRate (sampleRate);
    updateADSR();
}

void TapAudioSamplerAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TapAudioSamplerAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void TapAudioSamplerAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    if (shouldUpdate)
    {
        updateADSR();
    }

    for (const auto metadata : midiMessages)
    {
        const auto message = metadata.getMessage();

        if (message.isNoteOn())
            isNotePlayed = true;
        else if (message.isNoteOff())
            isNotePlayed = false;
    }
    
    sampleCount = isNotePlayed ? sampleCount += buffer.getNumSamples() : 0;

    sampler.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
}

//==============================================================================
bool TapAudioSamplerAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TapAudioSamplerAudioProcessor::createEditor()
{
    return new TapAudioSamplerAudioProcessorEditor (*this);
}

//==============================================================================
void TapAudioSamplerAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TapAudioSamplerAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void TapAudioSamplerAudioProcessor::loadFile()
{
    sampler.clearSounds();
    
    juce::FileChooser chooser { "Please load a file" };

    chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
    {
        const auto file = fc.getResult();

        // the reader can be a local variable here since it's not needed by the SamplerSound after this
        if (const std::unique_ptr<juce::AudioFormatReader> reader{ formatManager.createReaderFor(file) })
        {
            juce::BigInteger range;
            range.setRange(0, 128, true);
            sampler.addSound(new juce::SamplerSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
        }
    });
}

void TapAudioSamplerAudioProcessor::loadFile (const juce::String& path)
{
    sampler.clearSounds();

    const auto file = juce::File (path);

    // the reader can be a local variable here since it's not needed by the other classes after this
    if (const std::unique_ptr<juce::AudioFormatReader> reader{ formatManager.createReaderFor(file) })
    {
        juce::BigInteger range;
        range.setRange(0, 128, true);
        sampler.addSound(new juce::SamplerSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
        updateADSR();
    }
}

juce::AudioBuffer<float>& TapAudioSamplerAudioProcessor::getWaveForm() const
{
    // get the last added synth sound as a SamplerSound*
    if (const auto sound = dynamic_cast<juce::SamplerSound*>(sampler.getSound(sampler.getNumSounds() - 1).get()))
    {
        return *sound->getAudioData();
    }

    // just in case it somehow happens that the sound doesn't exist or isn't a SamplerSound,
    // return a static instance of an empty AudioBuffer here...
    static juce::AudioBuffer<float> dummybuffer;

    return dummybuffer;
}

void TapAudioSamplerAudioProcessor::updateADSR()
{
    shouldUpdate = false;
    
    adsrParams.attack = apvts.getRawParameterValue ("ATTACK")->load();
    adsrParams.decay = apvts.getRawParameterValue ("DECAY")->load();
    adsrParams.sustain = apvts.getRawParameterValue ("SUSTAIN")->load();
    adsrParams.release = apvts.getRawParameterValue ("RELEASE")->load();
    
    for (int i = 0; i < sampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<juce::SamplerSound*>(sampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters (adsrParams);
        }
    }
}

juce::AudioProcessorValueTreeState::ParameterLayout TapAudioSamplerAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", 0.0f, 5.0f, 0.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", 0.0f, 5.0f, 2.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", 0.0f, 1.0f, 1.0f));
    params.push_back (std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", 0.0f, 5.0f, 0.0f));
    
    return { params.begin(), params.end() };
}

void TapAudioSamplerAudioProcessor::valueTreePropertyChanged (juce::ValueTree &treeWhosePropertyHasChanged, const juce::Identifier &property)
{
    shouldUpdate = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapAudioSamplerAudioProcessor();
}
