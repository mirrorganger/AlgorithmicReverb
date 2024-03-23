#pragma once

#include "JuceHeader.h"
#include "DattoroReverb.h"

class PluginProcessor  : public juce::AudioProcessor
{
public:
    PluginProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const juce::BusesLayout& layouts) const override;
   #endif

    void processBlock (juce::AudioBuffer<float>&, juce::MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const juce::String& newName) override;

    //==============================================================================
    void getStateInformation (juce::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    void updateParameters();
    DattoroReverb _dattoroReverb;
    juce::AudioProcessorValueTreeState _parameters;
    std::atomic<float>* _paramBandwidth = nullptr;
    std::atomic<float>* _paramDamping = nullptr;
    std::atomic<float>* _paramDecay = nullptr;
    juce::SmoothedValue<float> _bandwidth = .5F;
    juce::SmoothedValue<float> _damping = .5F;
    juce::SmoothedValue<float> _decay = .5F;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginProcessor)
};
