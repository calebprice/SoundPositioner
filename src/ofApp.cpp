#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){

	sender.setup(HOST, PORT);

	/* some standard setup stuff*/

	ofEnableAlphaBlending();
	ofSetupScreen();
	ofBackground(0, 0, 0);
	ofSetFrameRate(60);

	/* This is stuff you always need.*/

	sampleRate = 48000;
	bufferSize = 512;
	lAudioOut = new float[bufferSize];	/* outputs */
	rAudioOut = new float[bufferSize];
	lAudioIn = new float[bufferSize];	/* inputs */
	rAudioIn = new float[bufferSize];

	/* This is a nice safe piece of code */
	memset(lAudioOut, 0, bufferSize * sizeof(float));
	memset(rAudioOut, 0, bufferSize * sizeof(float));

	memset(lAudioIn, 0, bufferSize * sizeof(float));
	memset(rAudioIn, 0, bufferSize * sizeof(float));

	fftSize = 1024;
	left_fft.setup(fftSize, 512, 256);
	right_fft.setup(fftSize, 512, 256);
	float startFreq = 200; // Hz, below which audio signal will be ignored for analysis purposes
	startBin = (int)startFreq / (sampleRate / fftSize);

	ofxMaxiSettings::setup(sampleRate, 2, bufferSize);

	// the following line sets up the default audio input as the audio source...
	//ofSoundStreamSetup(2, 2, this, sampleRate, bufferSize, 4);	/* Call this last ! */

	// ...alternatively, the following lines set up a specific Microsoft DIrect Sound input as the audio source - 
	//		this is obviously platform specific, so commented out from here...
	ofSoundStream soundStream;
	ofSoundStreamSettings settings;
	settings.numOutputChannels = 2;
	settings.numInputChannels = 2;
	settings.setOutListener(this);
	settings.setInListener(this);
	settings.numBuffers = 4;
	settings.sampleRate = sampleRate;
	settings.bufferSize = bufferSize;

	settings.setApi(ofSoundDevice::Api::MS_DS);
	auto devices = soundStream.getDeviceList(ofSoundDevice::Api::MS_DS);
	settings.setInDevice(devices[0]);

	ofSoundStreamSetup(settings);
	// ...to here

	//GUI STUFF
	float textWidth = 600;
	gui.setup(); // most of the time you don't need a name
	gui.add(peakFrequencyToggle.setup("Peak frequency (Hz) (1 input)", true, textWidth));
	gui.add(powerToggle.setup("Left and right channel power of peak frequency (dB) (2 inputs)", true, textWidth));
	gui.add(powerRatioToggle.setup("L/R power ratio (dB) (1 input)", true, textWidth));
	gui.add(phaseDeltaToggle.setup("L-R phase difference (degrees) (1 input)", true, textWidth));
	gui.add(timeDeltaToggle.setup("L-R time difference (milliseconds) (1 input)", true, textWidth));

	bHide = true;

	myfont.loadFont("arial.ttf", 18); //requires this to be in bin/data/
	myFont2.loadFont("arial.ttf", 12); //requires this to be in bin/data/

	ofSetVerticalSync(true);

}

//--------------------------------------------------------------
void ofApp::update(){

	ofxOscMessage m;
	m.setAddress("/wek/inputs");

	if (peakFrequencyToggle) {
		m.addFloatArg(peakFreq);
	}

	if (powerToggle) {
		m.addFloatArg(leftPower);
		m.addFloatArg(rightPower);
	}

	if (powerRatioToggle) {
		m.addFloatArg(leftPower - rightPower);
	}

	if (phaseDeltaToggle) {
		m.addFloatArg(phaseDelta);
	}

	if (timeDeltaToggle) {
		m.addFloatArg(timeDelta);
	}

	sender.sendMessage(m);

}

//--------------------------------------------------------------
void ofApp::draw(){

	float horizWidth = 500.;
	float horizOffset = 400;
	float leftFftTop = 300;
	float rightFftTop = 450;
	float textTop = 450;


	float xinc = horizWidth / fftSize * 2.0;

	//Left channel FFT magnitudes:
	ofSetColor(255, 0, 0, 255);
	for (int i = 0; i < fftSize / 2; i++) {
		float height_l = left_fft.magnitudesDB[i] * 4;
		ofRect(horizOffset + (i*xinc), leftFftTop - height_l, 2, height_l);
	}
	myfont.drawString("Left Channel FFT:", 30, leftFftTop);

	//Right channel FFT magnitudes:
	ofSetColor(0, 255, 0, 255);
	for (int i = 0; i < fftSize / 2; i++) {
		float height_r = right_fft.magnitudesDB[i] * 4;
		ofRect(horizOffset + (i*xinc), rightFftTop - height_r, 2, height_r);
	}
	myfont.drawString("Right Channel FFT:", 30, rightFftTop);

	ofSetColor(255, 255, 255, 255);

	char peakString[255]; // an array of chars
	sprintf(peakString, "Peak Frequency: %.2fHz", peakFreq);
	myfont.drawString(peakString, 12, textTop + 50);

	char leftPowerString[255]; // an array of chars
	sprintf(leftPowerString, "Left channel power at peak frequency: %.2fdB", leftPower);
	myfont.drawString(leftPowerString, 12, textTop + 100);

	char rightPowerString[255]; // an array of chars
	sprintf(rightPowerString, "Right channel power at peak frequency: %.2fdB", rightPower);
	myfont.drawString(rightPowerString, 12, textTop + 150);

	char deltaPowerString[255]; // an array of chars
	sprintf(deltaPowerString, "L/R power ratio at peak frequency: %.2fdB", leftPower - rightPower);
	myfont.drawString(deltaPowerString, 12, textTop + 200);

	char phaseDeltaString[255]; // an array of chars
	sprintf(phaseDeltaString, "L-R phase difference at peak frequency: %.2fdegrees", phaseDelta);
	myfont.drawString(phaseDeltaString, 12, textTop + 250);

	char timeDeltaString[255]; // an array of chars
	sprintf(timeDeltaString, "L-R time difference at peak frequency: %.2fms", timeDelta);
	myfont.drawString(timeDeltaString, 12, textTop + 300);

	int numInputs = 0;

	if (peakFrequencyToggle) {
		numInputs += 1;
	}

	if (powerToggle) {
		numInputs += 2;
	}

	if (powerRatioToggle) {
		numInputs += 1;
	}

	if (phaseDeltaToggle) {
		numInputs += 1;
	}

	if (timeDeltaToggle) {
		numInputs += 1;
	}

	char numInputsString[255]; // an array of chars
	sprintf(numInputsString, "Sending %d inputs total", numInputs);
	myfont.drawString(numInputsString, 650, 100);

	if (bHide) {
		gui.draw();
	}

}

	//--------------------------------------------------------------
	void ofApp::audioRequested (float * output, int bufferSize, int nChannels) {
		
		int peakBinLeft = 0;
		int peakBinRight = 0;
		int peakBin = 0;

		int maxBin;

		for (int i = 0; i < bufferSize; i++) {

			left_wave = lAudioIn[i];

			float peakPwrLeft = 0;

			if (left_fft.process(left_wave)) {

				left_fft.magsToDB();
				
				maxBin = startBin; // skip first few bins in case there is a significant amount of DC offset or mains hum

				for (int j = startBin; j < fftSize / 2; j++) {
					if (left_fft.magnitudes[j] > peakPwrLeft) {
						peakPwrLeft = left_fft.magnitudes[j];
						maxBin = j;
					}
				}

				peakBinLeft = maxBin;
			}

			right_wave = rAudioIn[i];

			float peakPwrRight = 0;

			if (right_fft.process(right_wave)) {

				right_fft.magsToDB();

				maxBin = startBin; // skip first few bins in case there is a significant amount of DC offset or mains hum

				for (int j = startBin; j < fftSize / 2; j++) {
					if (right_fft.magnitudes[j] > peakPwrRight) {
						peakPwrRight = right_fft.magnitudes[j];
						maxBin = j;
					}
				}

				peakBinRight = maxBin;
			}

			if (peakPwrLeft > peakPwrRight) {
				peakBin = peakBinLeft;
			}
			else {
				peakBin = peakBinRight;
			}

			peakFreq = (float)peakBin / fftSize * sampleRate;
			leftPower = (float)left_fft.magnitudesDB[peakBin];
			leftPhase = (float)left_fft.phases[peakBin];
			rightPower = (float)right_fft.magnitudesDB[peakBin];
			rightPhase = (float)right_fft.phases[peakBin];

			phaseDelta = (leftPhase - rightPhase) * 180 / 3.141592654;

			// wrap phaseDelta to =/- 180 degrees interval
			if (phaseDelta > 180) {
				phaseDelta -= 360;
			}
			else if (phaseDelta < -180) {
				phaseDelta += 360;
			}

			// avoid divide by zero error
			if (peakFreq > 0) {
				timeDelta = 1000 * phaseDelta / 360 / peakFreq;
			}
			else {
				timeDelta = 0;
			}

			lAudioOut[i] = 0;
			rAudioOut[i] = 0;

		}
	}

	//--------------------------------------------------------------
	void ofApp::audioReceived(float * input, int bufferSize, int nChannels) {


		/* You can just grab this input and stick it in a double, then use it above to create output*/

		float sum = 0;
		for (int i = 0; i < bufferSize; i++) {

			/* you can also grab the data out of the arrays*/

			lAudioIn[i] = input[i * 2];
			rAudioIn[i] = input[i * 2 + 1];

			sum += input[i * 2] * input[i * 2];

		}

	}

	//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
