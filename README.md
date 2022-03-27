# VAD-Assignment

A simple frequency based Voice Activity Detection (VAD) algorithm written in C++.


## WHAT IS THIS? ##

This class is designed to replace with silence noise section of a recorded speech.
The recorded speech must follow specific guidelines: 
- It must be PCM encoded with 8-bit signed values, sampled at 8000Hz with 160 samples every 20ms.
- It must be saved in ./inputData/inputaudio```<num>```.data where ```<num>``` is a placeholder for the actual int value

The output will be saved with the same encoding in ./outputData/outputaudio```<num>```.data where ```<num>``` is a placeholder for the value
previously submitted.

## OK, BUT HOW? ##
The actual functioning is rather simple:
1. An input and output streams are opened in order to simulate a real time connection
2. Every packet composed of 160 samples (minus the last one which is allowed to have less samples) is processed as follow:
  2.1 If in the last two packets there was a voice-match it will be transimitted regardeless of its content
  2.2 Using FFT (Fast Fourier Transformation) a packet is categorized as either voice-match or not thanks to its main frequency
  2.3 If it is a voice-match then:
    2.3.1 The most recent packet was also a voice-match then just the current packet is sent
    2.3.2 The most recent packet was not a voice-match then both packets will be sent
  2.4 If it is not a voice-match then:
    2.4.1 If the most recent packet was a voice-match then a silenced packet (composed of zeros) is sent
3. Both input and ouput streams are closed to prevent undefined behaviours

## BUT DOES IT WORK? ##
In fact, quite well. It is suggested to download a music player such as [Audacity](https://www.audacityteam.org/) and use
the "import raw data" option in order to specify sampling rate (8000Hz) and encoding (PCM - 8 bit signed).
All audio files are already available in the "outputData" folder.

## HOW DO I RUN IT? ##
Compile the source code with ```g++ *.cpp *.hpp -o vad``` in order to get an executable.
The next step is just to use it with ```./vad``` and follow the instructions.
