#pragma once

#include "emil.hpp"
#include <fstream>
#include "ExFun.h"
#include "PostSaccSens_Ecc.h"
//#include "emil/CFloatTexture.hpp"

#define subjectName "Test"


//debug
#define DEBUG_FLAG 1

//variable options
#define frameRate 40
#define frameNum 21
#define BUFFERLEN 10
#define caseNum 2
#define SNR 4
#define stimOnDelay 25

//changeable setting
#define fixDur 1200
#define fixDur1 300
#define stimDimen 120
#define saccMaxDur 2000
#define afterMaskDur 500
#define respTimeLimit 5000
#define fixRange 80
#define targetRange 150
#define dropTime 12000
#define loadTime 1000


//screen setting
#define screenW 1920
#define screenH 1080
#define screenR 200
#define bckLumin 127

class ExperimentBody : public CExperiment
{
public:
	ExperimentBody(int pxWidth, int pxHeight,
		int RefreshRate);

	/// Standard event handlers
	void initialize();
	void finalize();
	void saveData();
	void eventRender(unsigned int FrameCount, CEOSData* Samples);
	void eventJoypad();
	void createTrial();

	void createObjects();

	float getGazePxPosX();
	float getGazePxPosY();
	float getGazePxDist(float pX, float pY);
	void updateGazePxPos();
	void createStimuli();

	int getCurrTrial();

private:


	enum STATE {
		STATE_LOADING,
		STATE_INIT_FIX,
		STATE_SUSTAIN_FIX,
		STATE_STIM_ON,
		STATE_SACCADE_ONSET,
		STATE_SACCADE_OFFSET,
		STATE_STIM_OFF,
		STATE_MASK_OFF,
		STATE_WAIT_RESPONSE,
		STATE_QUIT,
		STATE_CALIBRATION
	};

	struct stopWatch {
		float fixDotOn;
		float targetDotOn;
		float saccOn;
		float saccOff;
		float stimON;
		float stimOff;
		float quit;

	};


	struct trialParams {

		float respTime;

		int caseIdx;

		int key;
		int resp;
		int hit;

		
		int saccMode;	
		int stimPhase;
		int targetPos;
	
	};

	struct screenLoc {
		float targetX;
		float targetY;
		float fixationX;
		float fixationY;
		float landingX;
		float landingY;
	} scrPix;


	STATE m_state;

	int ExperimentBody::string2int(std::string x);


	bool anyButtonPressed;
	bool firstEnter;

	float stimPresDur;

	int frameIdx;
	CImagePlane* stimMovie[frameNum];
	CImagePlane* checkerMask;
	CImagePlane* stimDebug;
	CEOSData* m_samples;
	CImagePlane* fixationDot;
	CImagePlane* targetDot;
	CImagePlane* reCalib_target;
	CImagePlane* reCalib_gaze;

	
	CTimer loadingDelay;
	CTimer fixTimer;
	CTimer fixTimer1;
	CTimer stimOnDelayTimer;	
	CTimer saccTimer;
	CTimer stimTimer;
	CTimer maskTimer;
	CTimer trialTimer;
	CTimer respTimer;

	trialParams currTrial;
	
	int validIndex;
	int trialIndex;

	float hit_vec[BUFFERLEN*caseNum];
	float rate_vec[caseNum];

	//unsigned int m_frameCount;
	float CorrectResp;
	float saccSize;
	int trialNum;
	int imgGratW;
	int imgGratH;



	string sbj;
	string saveDirectory;
	string expStartTime;

	stopWatch  timingRecord;


	float fixRangePix;
	float targetRangePix;
	float corrX;
	float corrY;
	float stabX;
	float stabY;

	float Velocity;

	float gazePxX;
	float gazePxY;

	float *imgGrat_Float;   // Grating
	float *noise_Float; //noise

	CNoisyImagePlane* Mask;

	bool eBlink;
	bool emEvent;
	float Dist;
	float Dist1;
	float Dist2;
	float preTime;
	float prePxPosX;
	float prePxPosY;
	float vel1;
	float saccPrepTime;
};