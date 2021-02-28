#pragma once

#include "ofMain.h"
#include "ofxMaxim.h"
#include "ofxGui.h"
//#include "ofRtAudioSoundStream.h"
#include "ofxOsc.h"

#define HOST "localhost"
#define PORT 6448

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		void audioRequested(float * input, int bufferSize, int nChannels); /* output method */
		void audioReceived(float * input, int bufferSize, int nChannels); /* input method */

		float 	* lAudioOut; /* outputs */
		float   * rAudioOut;

		float * lAudioIn; /* inputs */
		float * rAudioIn;

		int		bufferSize; /* buffer size */
		int		sampleRate;

		//MAXIMILIAN STUFF:
		double left_wave, right_wave, sample, outputs[2];

		int startBin = 0;
		float peakFreq = 0;

		float leftPower = 0;
		float rightPower = 0;
		float powerRatio = 0;

		float leftPhase = 0;
		float rightPhase = 0;
		float phaseDelta = 0;

		float timeDelta = 0;

		ofxMaxiFFT left_fft;
		ofxMaxiFFT right_fft;
		int fftSize;
		int bins, dataSize;

		maxiSample samp;

		//ofSoundStreamSettings settings;
		//ofSoundStream soundStream;

		//GUI STUFF
		bool bHide;

		ofxToggle peakFrequencyToggle;
		ofxToggle powerToggle;
		ofxToggle powerRatioToggle;
		ofxToggle phaseDeltaToggle;
		ofxToggle timeDeltaToggle;

		ofxPanel gui;
		ofTrueTypeFont myfont, myFont2;

		ofxOscSender sender;


};
