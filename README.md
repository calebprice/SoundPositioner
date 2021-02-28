# SoundPositioner

url: https://github.com/calebprice/SoundPositioner/

This app serves as an input to Wekinator and, as such provides a number of extracted features. The features are extracted from a stereo audio input with the intention that they could, in conjunction with appropriate Wekinator configuration and adequate training, form a machine learning system which is able to resolve the spatial location of a sound source. The envisaged system requires the use of a stereo microphone, or pair of mono microphones spaced apart appropriately.

SoundPositioner is an adaptation of an existing audio input example app for Wekinator 'VariousAudioInputs'.

The extracted features are:

1. Peak Frequency (Hz) - the frequency of the most prominent spectral component
2. Left channel power at peak frequency (dB) - relative to some arbitrary reference
3. Right channel powe at peak frequency (dB) - relative to some arbitrary reference
4. L/R power ratio at peak frequency (dB)
5. L-R phase difference at peak frequency (degrees)
6. Time difference at peak frequency (ms) - obtained by dividing phase difference by peak frequency

Build details:

The app was created using OpenFrameworks release 0.10.0 (2017) on a PC under MS Visual Studio version 15.9.30 (2017)

To set up the project, please use OpenFrameworks projectGenerator, specifying the following add-ons:

ofxGui, ofxOsc & ofxMaxim

ofxMaxim is an external add-on which can be downloaded from:

https://github.com/micknoise/Maximilian

It should then be copied into the add-ons folder within the OpenFrameworks directory hierarchy.

How to run:

Just double-click on the executable and the SoundPositioner GUI should pop-up.   The features are selected by the tick boxes in the top left corner.    All features are sent to port 6448, Wekinator's default listening port.

The app should receive audio data from your machine's default audio input device.   To change this to a specific device may require modification to the code specific to your machine and available devices.   Please see the comments within the setup procedure for some (limited) guidance here.

The GUI should display graphical representations of the FFT outputs of the left and right audio channels, below which should be displayed instantaneous values of the extracted features.
