/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

using namespace juce;

//==============================================================================
/**
*/
class BasicReverbAudioProcessorEditor  : public juce::AudioProcessorEditor, public Slider::Listener
{
public:
    BasicReverbAudioProcessorEditor (BasicReverbAudioProcessor&);
    ~BasicReverbAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    unique_ptr<Label> feedbackLabel;
    unique_ptr<Label> numberOfEchoesLabel;
    unique_ptr<Label> echoTimeDelayLabel;
    unique_ptr<Label> manufacturerLabel;

    unique_ptr<Slider> feedbackSlider;
    unique_ptr<Slider> numberOfEchoesSlider;
    unique_ptr<Slider> echoTimeDelaySlider;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    BasicReverbAudioProcessor& audioProcessor;

    // Inherited via Listener
    virtual void sliderValueChanged(Slider* slider) override;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BasicReverbAudioProcessorEditor)

};
