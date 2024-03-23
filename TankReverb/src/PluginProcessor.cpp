#include "PluginProcessor.h"
#include "PluginEditor.h"

#include <algorithm>

PluginProcessor::PluginProcessor()
:
#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
        _parameters(*this,nullptr,juce::Identifier("SchroederReverbParams"),
                    {
                        std::make_unique<AudioParameterFloat>(
                            "bandwith",
                            "Bandwith",
                            0.01F,
                            1.0F,
                            .5F
                            ),
                        std::make_unique<AudioParameterFloat>(
                            "decay",
                            "Decay",
                            0.01F,
                            1.0F,
                            0.5F
                            ),
                        std::make_unique<AudioParameterFloat>(
                            "damping",
                            "Damping ",
                            .01F,
                            1.0F,
                            0.5F
                            )
                    })
{
    _paramBandwidth = _parameters.getRawParameterValue("bandwith");
    _paramDamping = _parameters.getRawParameterValue("damping");
    _paramDecay = _parameters.getRawParameterValue("decay");
}

//==============================================================================
const String PluginProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PluginProcessor::acceptsMidi() const
{
    return false;
}

bool PluginProcessor::producesMidi() const
{
    return false;
}

bool PluginProcessor::isMidiEffect() const
{
    return false;
}

double PluginProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PluginProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PluginProcessor::getCurrentProgram()
{
    return 0;
}

void PluginProcessor::setCurrentProgram (int index)
{
}

const String PluginProcessor::getProgramName (int index)
{
    return {};
}

void PluginProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PluginProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // _schroederReverb.prepare(sampleRate, _rt60_s.getTargetValue() * 1000);
    _dattoroReverb.prepare(sampleRate);
}

void PluginProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PluginProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
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

void PluginProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    updateParameters();
    _dattoroReverb.setBandwith(_bandwidth.getCurrentValue());
    _dattoroReverb.setDecay(_decay.getCurrentValue());
    _dattoroReverb.setDamping(_damping.getCurrentValue());

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
         buffer.clear (i, 0, buffer.getNumSamples());

     _dattoroReverb.process(buffer.getArrayOfWritePointers(),buffer.getNumChannels(),buffer.getNumSamples());
}

//==============================================================================
bool PluginProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* PluginProcessor::createEditor()
{
    return new GenericAudioProcessorEditor(*this);
}

//==============================================================================
void PluginProcessor::getStateInformation (MemoryBlock& destData)
{
    auto state = _parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml (state.createXml());
    copyXmlToBinary (*xml, destData);
}

void PluginProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xmlState (getXmlFromBinary (data, sizeInBytes));
    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName (_parameters.state.getType()))
            _parameters.replaceState (juce::ValueTree::fromXml (*xmlState));
}


void PluginProcessor::updateParameters() {
    if(auto decay= _paramDecay->load();!juce::approximatelyEqual(decay, _decay.getTargetValue()))
    {
         _decay.setTargetValue(decay);
    }
    
    if(auto damping = _paramDamping->load(); !juce::approximatelyEqual(damping, _damping.getTargetValue())){
         _damping.setTargetValue(damping);
    }
      
    if(auto bandwith = _paramBandwidth->load();!juce::approximatelyEqual(bandwith, _bandwidth.getTargetValue())){
        _bandwidth.setTargetValue(bandwith);    
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PluginProcessor();
}



