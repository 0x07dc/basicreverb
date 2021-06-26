# Basic Reverb (or Delay)

This is an example of a simple reverb using a multi-tap comb filter, created using the JUCE C++ framework.

This plugin also functions as a type of multi-tap comb delay, depending on the settings used.

## Running

To run this plugin, one needs a copy of JUCE, of which a free license is available. Simply clone the repository and open the .projucer file with Projucer from the JUCE framework. The project is configured with XCode and Visual Studio exporters, although adding others of the available options is a simple task available in Projucer.

## Note of JUCE bug with Visual Studio

Because of a bug with the latest edition of JUCE, to compile in Visual Studio, one needs to make sure to set the "Program Database File Name" setting of the Basic_ReverbSharedCode target is set to "inherit from parent or project defaults." To do this in Visual Studio, simply right-click the Basic_ReverbSharedCode target, select "Properties" at the bottom, and under "Configuration Properties > C/C++ > Output Files," is the "Program Database File Name" setting. Simply click the down arrow and select "inherit from parent or project defaults." Previous editions of JUCE didn't require this manually setting Visual Studio configuration settings, and it'll probably be corrected in future editions. Therefore, this bug notice may not apply in the future.

## Repository Structure

The JUCE project is located in the folder, "BasicReverb." In the root folder of this repository is a demo .FLAC audio file which fades in and out the reverb mix. Also included in the root folder is a screenshot of the plugin.

## Features

- Uses comb-filter to create reverb/delay
- Adjustable number of echo taps
- Adjustable timing of echo taps
- Adjustable feedback of reverb/delay
- Persistent state for saving/loading music projects and saving/loading plugin presets
- No memory leaks; everything is RAII.