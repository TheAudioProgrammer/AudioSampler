
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
                       ), mAPVTS (*this, nullptr, "Parameters", createParameters())
#endif
{
    mFormatManager.registerBasicFormats();
    mAPVTS.state.addListener (this);
    
    for (int i = 0; i < mNumVoices; i++)
    {
        mSampler.addVoice (new juce::SamplerVoice());
    }
}

TapAudioSamplerAudioProcessor::~TapAudioSamplerAudioProcessor()
{
    mAPVTS.state.removeListener (this);
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
    mSampler.setCurrentPlaybackSampleRate (sampleRate);
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
    
    if (mShouldUpdate)
    {
        updateADSR();
    }
    
    juce::MidiMessage m;
    juce::MidiBuffer::Iterator it { midiMessages };
    int sample;
    
    while (it.getNextEvent (m, sample))
    {
        if (m.isNoteOn())
            mIsNotePlayed = true;
        else if (m.isNoteOff())
            mIsNotePlayed = false;
    }
    
    mSampleCount = mIsNotePlayed ? mSampleCount += buffer.getNumSamples() : 0;

    mSampler.renderNextBlock (buffer, midiMessages, 0, buffer.getNumSamples());
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
    mSampler.clearSounds();
    
    juce::FileChooser chooser { "Please load a file" };

    chooser.launchAsync(juce::FileBrowserComponent::openMode | juce::FileBrowserComponent::canSelectFiles,
        [this](const juce::FileChooser& fc)
    {
        auto file = fc.getResult();

        // the reader can be a local variable here since it's not needed by the SamplerSound after this
        std::unique_ptr<juce::AudioFormatReader> reader{ mFormatManager.createReaderFor(file) };
        if (reader)
        {
            juce::BigInteger range;
            range.setRange(0, 128, true);
            mSampler.addSound(new juce::SamplerSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
        }
    });
    
    
}

void TapAudioSamplerAudioProcessor::loadFile (const juce::String& path)
{
    mSampler.clearSounds();
    
    auto file = juce::File (path);
    // the reader can be a local variable here since it's not needed by the other classes after this
    std::unique_ptr<juce::AudioFormatReader> reader{ mFormatManager.createReaderFor(file) };
    if (reader)
    {
        juce::BigInteger range;
        range.setRange(0, 128, true);
        mSampler.addSound(new juce::SamplerSound("Sample", *reader, range, 60, 0.1, 0.1, 10.0));
        updateADSR();
    }
    
}

juce::AudioBuffer<float>& TapAudioSamplerAudioProcessor::getWaveForm()
{
    // get the last added synth sound as a SamplerSound*
    auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(mSampler.getNumSounds() - 1).get());
    if (sound)
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
    mShouldUpdate = false;
    
    mADSRParams.attack = mAPVTS.getRawParameterValue ("ATTACK")->load();
    mADSRParams.decay = mAPVTS.getRawParameterValue ("DECAY")->load();
    mADSRParams.sustain = mAPVTS.getRawParameterValue ("SUSTAIN")->load();
    mADSRParams.release = mAPVTS.getRawParameterValue ("RELEASE")->load();
    
    for (int i = 0; i < mSampler.getNumSounds(); ++i)
    {
        if (auto sound = dynamic_cast<juce::SamplerSound*>(mSampler.getSound(i).get()))
        {
            sound->setEnvelopeParameters (mADSRParams);
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
    mShouldUpdate = true;
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TapAudioSamplerAudioProcessor();
}
