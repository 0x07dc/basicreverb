/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

using namespace juce;

//==============================================================================
BasicReverbAudioProcessorEditor::BasicReverbAudioProcessorEditor (BasicReverbAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{

    feedbackLabel = make_unique<Label>("feedbackLabel");
    feedbackLabel->setText("Feedback", NotificationType::dontSendNotification);
    addAndMakeVisible(*feedbackLabel);

    numberOfEchoesLabel = make_unique<Label>("numberOfEchoesLabel");
    numberOfEchoesLabel->setText("Number of Echoes", NotificationType::dontSendNotification);
    addAndMakeVisible(*numberOfEchoesLabel);
    
    echoTimeDelayLabel = make_unique<Label>("echoTimeDelayLabel");
    echoTimeDelayLabel->setText("Echo Time Delay", NotificationType::dontSendNotification);
    addAndMakeVisible(*echoTimeDelayLabel);

    manufacturerLabel = make_unique<Label>("manufacturerLabel");
    manufacturerLabel->setText("Paradigm E", NotificationType::dontSendNotification);
    manufacturerLabel->setColour(Label::ColourIds::textColourId, Colour::fromRGB(0, 62, 63));
    manufacturerLabel->setFont(getLookAndFeel().getLabelFont(*manufacturerLabel).withPointHeight(16.0f));
    addAndMakeVisible(*manufacturerLabel);



    feedbackSlider = make_unique<Slider>("feedbackSlider");
    feedbackSlider->setRange(0.0, 1.0, 0.000001);
    feedbackSlider->getValueObject().referTo(audioProcessor.pluginSettingsState.getPropertyAsValue("feedback", nullptr));
    feedbackSlider->addListener(this);
    addAndMakeVisible(*feedbackSlider);

    numberOfEchoesSlider = make_unique<Slider>("numberOfEchoesSlider");
    numberOfEchoesSlider->setRange(1.0, 50.0, 1);
    numberOfEchoesSlider->getValueObject().referTo(audioProcessor.pluginSettingsState.getPropertyAsValue("numberOfEchoes", nullptr));
    numberOfEchoesSlider->addListener(this);
    addAndMakeVisible(*numberOfEchoesSlider);

    echoTimeDelaySlider = make_unique<Slider>("echoTimeDelaySlider");
    echoTimeDelaySlider->setRange(0, 0.25, 0.0001);
    echoTimeDelaySlider->getValueObject().referTo(audioProcessor.pluginSettingsState.getPropertyAsValue("echoTimeDelay", nullptr));
    echoTimeDelaySlider->addListener(this);
    addAndMakeVisible(*echoTimeDelaySlider);

    getLookAndFeel().setColour(ResizableWindow::backgroundColourId, Colour::fromRGB(0, 21, 21));

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (260, 190 + 20);
}

BasicReverbAudioProcessorEditor::~BasicReverbAudioProcessorEditor()
{
}

//==============================================================================
void BasicReverbAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

}

void BasicReverbAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    int fromTop = 14;
    feedbackLabel->setBounds(30, fromTop, 200, 15);
    feedbackSlider->setBounds(30, fromTop, 200, 60);

    numberOfEchoesLabel->setBounds(30, fromTop + 60 , 200, 15);
    numberOfEchoesSlider->setBounds(30, fromTop + 60, 200, 60);

    echoTimeDelayLabel->setBounds(30, fromTop + 60 * 2, 200, 15);
    echoTimeDelaySlider->setBounds(30, fromTop + 60 * 2, 200, 60);

    manufacturerLabel->setBounds(5, fromTop + 60 * 3 - 5, 200, 20);
}

void BasicReverbAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    String sliderName = slider->getName();
    if (sliderName == "feedbackSlider") {
        float value = slider->getValue();
        audioProcessor.feedback->beginChangeGesture();
        audioProcessor.feedback->setValueNotifyingHost(value, audioProcessor.pluginSettingsState);
        audioProcessor.feedback->endChangeGesture();
    }
    if (sliderName == "numberOfEchoesSlider") {
        float value = slider->getValue();
        audioProcessor.numberOfEchoes->beginChangeGesture();
        audioProcessor.numberOfEchoes->setValueNotifyingHost(value, audioProcessor.pluginSettingsState);
        audioProcessor.numberOfEchoes->endChangeGesture();
    }
    if (sliderName == "echoTimeDelaySlider") {
        float value = slider->getValue();
        audioProcessor.echoTimeDelay->beginChangeGesture();
        audioProcessor.echoTimeDelay->setValueNotifyingHost(value, audioProcessor.pluginSettingsState);
        audioProcessor.echoTimeDelay->endChangeGesture();
    }
}
