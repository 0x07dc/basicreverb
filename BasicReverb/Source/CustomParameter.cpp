#include "CustomParameter.h"

float CustomParameter::getValue() const
{
    return value;
}

void CustomParameter::setValue(float newValue)
{
    value = newValue;
}

void CustomParameter::setValue(float newValue, ValueTree& state)
{
    state.setProperty(getName(999), newValue, nullptr);
    setValue(newValue);
}

void CustomParameter::setValueNotifyingHost(float newValue, ValueTree& state)
{
    state.setProperty(getName(999), newValue, nullptr);
    this->AudioProcessorParameter::setValueNotifyingHost(newValue);
}

float CustomParameter::getDefaultValue() const
{
    return 0.0f;
}

String CustomParameter::getName(int maximumStringLength) const
{
    return name.substring(0, maximumStringLength - 1);
}

void CustomParameter::setName(String name)
{
    this->name = name;
}

String CustomParameter::getLabel() const
{
    return String();
}

float CustomParameter::getValueForText(const String& text) const
{
    return 0.0f;
}
