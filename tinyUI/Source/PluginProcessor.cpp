/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"




//==============================================================================
TinyUIAudioProcessor::TinyUIAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ),
                        apvts(*this, nullptr, "parameters", createParameters()),
                        myComp(apvts, getSampleRate())
#endif
{
    addParameter (gain = new juce::AudioParameterFloat ("gain", //parameterID
                                                        "Gain", //parameterName
                                                        0.0f,   //min
                                                        1.0f,   //max
                                                        0.5f)); //default
}

TinyUIAudioProcessor::~TinyUIAudioProcessor()
{
}

//==============================================================================
const juce::String TinyUIAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool TinyUIAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool TinyUIAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool TinyUIAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double TinyUIAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int TinyUIAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int TinyUIAudioProcessor::getCurrentProgram()
{
    return 0;
}

void TinyUIAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String TinyUIAudioProcessor::getProgramName (int index)
{
    return {};
}

void TinyUIAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void TinyUIAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    previousGain = *gain;
    
    
    
}

void TinyUIAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool TinyUIAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
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

void TinyUIAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    //juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();
    
    float d{0.0};
    
    float inGain = apvts.getRawParameterValue("INGAIN")->load();
    inGain = (inGain - 0.5) * 12.0;
    inGain = pow(10.0, (inGain / 20.0));
    buffer.applyGain(inGain);
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    
    float mix = apvts.getRawParameterValue("MIX")->load();
    
    myComp.updateParameters(apvts, getSampleRate());
    std::vector<float> chans{};
    
    for( int j = 0; j < buffer.getNumSamples(); ++j)
    {
        for(int l = 0; l < totalNumInputChannels; ++l)
        {
            chans.push_back(buffer.getSample(l, j));
        }
        auto samps = myComp.process(chans, totalNumInputChannels, getSampleRate());
        d += myComp.delta;
        
        for(int k = 0; k < totalNumInputChannels; ++k)
        {
            //auto* channelData = buffer.getWritePointer (k, j);
            //channelData = &samps[k];
            float dry = buffer.getSample(k, j) * (1.0 - mix);
            buffer.setSample(k, j, ((samps[k]) * mix) + dry);
        }
        if(j % 8 == 0)
        {
            d /= 8;
            redux.store(d);
            d = 0;
        }
        
        chans.clear();
    }
    float outGain = apvts.getRawParameterValue("OUTGAIN")->load();
    outGain = (outGain - 0.5) * 12.0;
    outGain = pow(10.0, (outGain / 20.0));
    buffer.applyGain(outGain);
    
    /*
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);
        
        // ..do something to the data...
    }
     */
}

//==============================================================================
bool TinyUIAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* TinyUIAudioProcessor::createEditor()
{
    return new TinyUIAudioProcessorEditor (*this);
}

//==============================================================================
void TinyUIAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    juce::MemoryOutputStream (destData, true).writeFloat (*gain);
    
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void TinyUIAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    *gain = juce::MemoryInputStream(data, static_cast<size_t> (sizeInBytes), false).readFloat();
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}


//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new TinyUIAudioProcessor();
}


juce::AudioProcessorValueTreeState::ParameterLayout TinyUIAudioProcessor::createParameters()
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
    
    auto dBAttributes = juce::AudioParameterFloatAttributes().
        withStringFromValueFunction(
            [](float x, int ssize){
                std::ostringstream ss;
                x = pow(x, 2);
                if(x <= 0.000001)
                    x = -120.0;
                else
                    x = 20.0 * log10(abs(x));
                ss << static_cast<int>(x) << " db";
                return juce::String(ss.str());
            }
        );
    
    params.push_back (std::make_unique<juce::AudioParameterFloat>(  "THRESHOLD" //ID
                                                                  , "threshold" //name
                                                                  , juce::NormalisableRange<float>(0.0, 1.0) //range
                                                                  , 1.0         //default
                                                                  , dBAttributes    //attributes
                                                                  ));
    
    
    auto ratioAttributes = juce::AudioParameterFloatAttributes().
        withStringFromValueFunction(
            [](float x, int ssize){
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(2) << (x * 19.0) + 1.0 << ":1";
                return juce::String(ss.str());
            }
        );
    
    
    
    params.push_back (
        std::make_unique<juce::AudioParameterFloat>(
            "RATIO", //parameterID
            "ratio", //name
            juce::NormalisableRange<float>(0.0, 1.0), //range
            0.5, //default
            ratioAttributes
        )
     );
    
    
    auto timingAttributes = juce::AudioParameterFloatAttributes().
        withStringFromValueFunction(
            [](auto x, auto){
                std::ostringstream ss;
                ss << static_cast<int>(x * 100.0) << " ms";
                return juce::String(ss.str());
            }
        );
    
    params.push_back (
        std::make_unique<juce::AudioParameterFloat>(
            "ATTACK",
            "Attack",
            juce::NormalisableRange<float>(0.0, 1.0),
            0.1,
            timingAttributes
        )
    );
    
    
    params.push_back (
        std::make_unique<juce::AudioParameterFloat>(
            "RELEASE",
            "Release",
            juce::NormalisableRange<float>(0.0, 1.0),
            0.1,
            timingAttributes
        )
    );
    
    
    auto dbBipolarAttributes = juce::AudioParameterFloatAttributes().
        withStringFromValueFunction(
            [](auto x, auto){
                std::ostringstream ss;
                x = x * 120.0;                  //replace w db conversion at some point
                x = x - 60;
                ss << static_cast<int>(x) << " db";
                return juce::String(ss.str());
            }
        );
    
    auto percentAttributes = juce::AudioParameterFloatAttributes().
        withStringFromValueFunction(
            [](auto x, auto){
                std::ostringstream ss;
                x = x * 100.0;
                ss << static_cast<int>(x) << " %";
                return juce::String(ss.str());
            }
        );
    
    
    params.push_back (
        std::make_unique<juce::AudioParameterFloat>(
            "INGAIN",
            "in gain",
            juce::NormalisableRange<float>(0.0, 1.0),
            0.5,
            dbBipolarAttributes
        )
    );
    
    params.push_back (
        std::make_unique<juce::AudioParameterFloat>(
            "OUTGAIN",
            "out gain",
            juce::NormalisableRange<float>(0.0, 1.0),
            0.5,
            dbBipolarAttributes
        )
    );
    
    params.push_back (
        std::make_unique<juce::AudioParameterFloat>(
            "MIX",
            "mix",
            juce::NormalisableRange<float>(0.0, 1.0),
            1.0,
            percentAttributes
        )
    );
            
    
    
    
    return {params.begin(), params.end()};
}
