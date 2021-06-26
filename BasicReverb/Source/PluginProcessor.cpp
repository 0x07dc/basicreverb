/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
BasicReverbAudioProcessor::BasicReverbAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    pluginSettingsState.setProperty("name", "pluginSettingsState", nullptr);

    addParameter(feedback = new CustomParameter());
    feedback->setName("feedback");
    feedback->setValue(defaultFeedback, pluginSettingsState);
    
    addParameter(numberOfEchoes = new CustomParameter());
    numberOfEchoes->setName("numberOfEchoes");
    numberOfEchoes->setValue(defaultNumberOfEchoes, pluginSettingsState);

    addParameter(echoTimeDelay = new CustomParameter());
    echoTimeDelay->setName("echoTimeDelay");
    echoTimeDelay->setValue(defaultEchoTimeDelay, pluginSettingsState);

}

BasicReverbAudioProcessor::~BasicReverbAudioProcessor()
{

}

//==============================================================================
const juce::String BasicReverbAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool BasicReverbAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool BasicReverbAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool BasicReverbAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double BasicReverbAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int BasicReverbAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int BasicReverbAudioProcessor::getCurrentProgram()
{
    return 0;
}

void BasicReverbAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String BasicReverbAudioProcessor::getProgramName (int index)
{
    return {};
}

void BasicReverbAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void BasicReverbAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void BasicReverbAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool BasicReverbAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void BasicReverbAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    int numberOfEchoesLocal = numberOfEchoes->getValue();
    echoTimesInSeconds.resize(numberOfEchoesLocal, 0);
    for (int i = 1; i <= numberOfEchoesLocal; i++) {
        echoTimesInSeconds[i-1] = ((float)i * echoTimeDelay->getValue());
    }

    int sampleRate = getSampleRate();
    reverbTrackLengthInSamples = (int)(sampleRate * reverbTrackLengthInSeconds);
    reverbTrack.resize(totalNumInputChannels, vector<float>(reverbTrackLengthInSamples, 0));
    echoTimesInSamples.resize(numberOfEchoesLocal);
    for (int i = 0; i < echoTimesInSamples.size(); i++) {
        echoTimesInSamples[i] = (int)(sampleRate * echoTimesInSeconds[i]);
    }


    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    if (reverbWritePointer.size() < totalNumInputChannels) reverbWritePointer.resize(totalNumInputChannels, 0);
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        for (int i = 0; i < buffer.getNumSamples(); i++) {
            
            //channelData[i] = 0;
            for (int j = 0; j < echoTimesInSamples.size(); j++) {
                int readPointer = reverbWritePointer[channel] - echoTimesInSamples[j];
                while (readPointer < 0) {
                    readPointer = reverbTrackLengthInSamples + readPointer;
                }
                float delayedSample = reverbTrack[channel][readPointer];
                channelData[i] += (feedback->getValue() / 2) / ((j+1) + (j*2)) * delayedSample;

            }
            if (channelData[i] > 1) channelData[i] = 1;
            if (channelData[i] < -1) channelData[i] = -1;
            reverbTrack[channel][reverbWritePointer[channel]] = channelData[i];
            
            reverbWritePointer[channel] = (reverbWritePointer[channel] + 1) % reverbTrackLengthInSamples;
        }
        
    }
}

//==============================================================================
bool BasicReverbAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* BasicReverbAudioProcessor::createEditor()
{
    return new BasicReverbAudioProcessorEditor (*this);
}

//==============================================================================
void BasicReverbAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    unique_ptr<juce::XmlElement> xml(pluginSettingsState.createXml());
    String testXML = xml->toString();
    copyXmlToBinary(*xml, destData);
}

void BasicReverbAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName("pluginSettingsState")) {
            ValueTree valueTree = ValueTree::fromXml(*xmlState);

            feedback->setValue(valueTree.getProperty("feedback"), pluginSettingsState);
            numberOfEchoes->setValue(valueTree.getProperty("numberOfEchoes"), pluginSettingsState);
            echoTimeDelay->setValue(valueTree.getProperty("echoTimeDelay"), pluginSettingsState);
        }
        
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new BasicReverbAudioProcessor();
}
