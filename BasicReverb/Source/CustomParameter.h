#pragma once

#include <JuceHeader.h>

using namespace std;
using namespace juce;

class CustomParameter : public AudioProcessorParameter {
public:
	// Inherited via AudioProcessorParameter
	float getValue() const;
	void setValue(float newValue);
	void setValue(float newValue, ValueTree& state);
	void setValueNotifyingHost(float newValue, ValueTree& state);
	float getDefaultValue() const;
	String getName(int maximumStringLength) const;
	void setName(String name);
	String getLabel() const;
	float getValueForText(const String& text) const;
private:
	float value = 0;
	String name;
};
