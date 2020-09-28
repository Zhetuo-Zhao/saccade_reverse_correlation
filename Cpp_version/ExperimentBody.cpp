#include "stdafx.h"
#include "ExperimentBody.h"
#include <math.h>
#include <random>

#define PI 3.14159265



ExperimentBody::ExperimentBody(int pxWidth,
	int pxHeight, int RefreshRate) :
	CExperiment(pxWidth, pxHeight, RefreshRate)
{
	setExperimentName("saccFreq");

}


void ExperimentBody::initialize()
{
	srand(time(NULL));

	//get the experiment start time
	time_t rawTime;
	struct tm * timeInfo;
	time(&rawTime);
	timeInfo = localtime(&rawTime);
	expStartTime = asctime(timeInfo);


	imgGratW = stimDimen;
	imgGratH = stimDimen;

	createObjects();

	disable(CExperiment::EIS_PHOTOCELL);
	disable(CExperiment::EIS_NOTRACK_ICON);
	disable(CExperiment::EIS_STAT1);
	disable(CExperiment::EIS_STAT2);
	disable(CExperiment::EIS_JP_STRT);


	// Set the background color for the experiment
	COGLEngine::Instance()->setBackgroundColor(bckLumin, bckLumin, bckLumin);

	for (int i = 0; i < BUFFERLEN*caseNum; i++)
		hit_vec[i] = 0.75;
	for (int i = 0; i < caseNum; i++)
		rate_vec[i] = 0.75;

	trialNum = 100;
	CorrectResp = 0;
	validIndex = 0;
	trialIndex = 0;
	corrX = corrY = 0;

	currTrial.resp = 0;
	currTrial.respTime = 0;

	preTime = 0;
	prePxPosX = 0;
	prePxPosY = 0;

	fixRangePix = CConverter::Instance()->a2px(fixRange);
	targetRangePix = CConverter::Instance()->a2px(targetRange);
	saveDirectory = "./mydata/" + std::string(subjectName);

	m_state = STATE_LOADING;
	//	m_state = STATE_CALIBRATION;
}

void ExperimentBody::eventRender(unsigned int FrameCount, CEOSData* Samples)
{
	m_samples = Samples;
	// velocity signal from the dsp
	Velocity = Samples->vchannels[0][5];

	vel1 = sqrt((getGazePxPosX() - prePxPosX) *(getGazePxPosX() - prePxPosX) + (getGazePxPosY() - prePxPosY) * (getGazePxPosY() - prePxPosY));
	prePxPosX = getGazePxPosX();
	prePxPosY = getGazePxPosY();
	
	storeTrialStream(0, trialTimer.getTime());
	storeTrialStream(1, getGazePxPosX());
	storeTrialStream(2, vel1);
	storeTrialStream(3, frameIdx);
/*
	bool emEvent1 = CTriggers::all(Samples->triggers, Samples->samplesNumber, EOS_TRIG_1_EMEVENT);
	bool emEvent2 = CTriggers::any(Samples->triggers, Samples->samplesNumber, EOS_TRIG_1_EMEVENT);
	bool eBlink = CTriggers::any(Samples->triggers, Samples->samplesNumber, EOS_TRIG_1_BLINK);
	bool badData = CTriggers::any(m_samples->triggers, m_samples->samplesNumber, EOS_TRIG_1_BADDATA);
*/
	//CEnvironment::Instance()->outputMessage("emEvent1=%d, Velocity=%.2f, eBlink=%d, badData=%d", emEvent1, Velocity, eBlink, badData);
    //CEnvironment::Instance()->outputMessage("emEvent1=%d, emEvent2=%d, eBlink=%d, badData=%d", emEvent1, emEvent2, eBlink, badData); 
	//CEnvironment::Instance()->outputMessage("trialTimer=%f, m_state=%d", trialTimer.getTime(), float(m_state));

	//if ( !m_fixMode && trialTimer.isExpired()) {
	//	CEnvironment::Instance()->outputMessage("trialTimer=%f, m_state=%d, isExpired=%d, isRunning=%d", trialTimer.getTime(), m_state, trialTimer.isExpired(), trialTimer.isRunning());
	if (trialTimer.isExpired() && trialTimer.isRunning())
	{
		checkerMask->hide();
		fixationDot->hide();
		targetDot->hide();


		trialTimer.stop();
		m_state = STATE_QUIT;
		CEnvironment::Instance()->outputMessage("Quitting earlier");
	}
	//	CEnvironment::Instance()->outputMessage("m_state=%d", m_state);
	switch (m_state)
	{
	case STATE_CALIBRATION:

		if (firstEnter)
		{
			firstEnter = 0;
			CEnvironment::Instance()->outputMessage("------------------------------------------------------");
			startTrial();

			reCalib_target->pxSetPosition(0, 0);
			reCalib_target->show();
			reCalib_gaze->pxSetPosition(getGazePxPosX(), getGazePxPosY());
			reCalib_gaze->show();
			moveToFront(reCalib_target);
			moveToFront(reCalib_gaze);

			CEnvironment::Instance()->outputMessage("---Calibration Trial---");
		}
		else
		{
			if (anyButtonPressed)
			{
				reCalib_gaze->hide();
				reCalib_target->hide();
				endTrial();
				m_state = STATE_LOADING;
				firstEnter = 1;
			}
			else
				reCalib_gaze->pxSetPosition(getGazePxPosX(), getGazePxPosY());
		}
		break;

	case STATE_LOADING:

		if (firstEnter)
		{
			firstEnter = 0;

			timingRecord.fixDotOn = 0;
			timingRecord.targetDotOn = 0;
			timingRecord.saccOff = 0;
			timingRecord.saccOn = 0;
			timingRecord.stimOff = 0;
			timingRecord.stimON = 0;
			timingRecord.quit = 0;


			loadingDelay.start(loadTime);
			CEnvironment::Instance()->outputMessage("----------------------------------------------------------------");
			// calculate peripheral locations
			float fixXadeg = 0;
			float fixYadeg = 0;
			CConverter::Instance()->a2p(fixXadeg, fixYadeg, scrPix.fixationX, scrPix.fixationY);

			// reset saccade landing position
			scrPix.landingX = NULL;
			scrPix.landingY = NULL;

			createTrial();
			createStimuli();

		}

		else
		{
			if (loadingDelay.isExpired())
			{
				m_state = STATE_INIT_FIX;
				firstEnter = 1;
			}
		}
		break;

	case STATE_INIT_FIX:

		if (firstEnter)
		{
			firstEnter = 0;

			startTrial(); // start trial

			trialTimer.start(dropTime);
			timingRecord.fixDotOn = trialTimer.getTime();

			fixationDot->pxSetPosition(scrPix.fixationX, scrPix.fixationY);
			fixationDot->show();
			moveToFront(fixationDot);
		}

		else
		{

			Dist = getGazePxDist(scrPix.fixationX, scrPix.fixationY);
			if (DEBUG_FLAG) Dist = 0;
			if (Dist < fixRangePix)
			{
				fixTimer.start(fixDur);
				fixTimer1.start(fixDur1);
				m_state = STATE_SUSTAIN_FIX;
				firstEnter = 1;
			}
		}
		break;

	case STATE_SUSTAIN_FIX:
		Dist = getGazePxDist(scrPix.fixationX, scrPix.fixationY);
		if (DEBUG_FLAG) Dist = 0;

		if (Dist > fixRangePix)
		{
			m_state = STATE_INIT_FIX;
			firstEnter = 0;
		}

		if (vel1 >100)
		{
			fixTimer1.start(fixDur1);
		}

		if (fixTimer.isExpired() && fixTimer1.isExpired())
		{
			fixationDot->hide();
			targetDot->show();
			targetDot->pxSetPosition(scrPix.targetX, scrPix.targetY);
			moveToFront(targetDot);
			timingRecord.targetDotOn = trialTimer.getTime();


			stimOnDelayTimer.start(rand() % stimOnDelay);
			m_state = STATE_STIM_ON;
			firstEnter = 1;
		}
		break;

	case STATE_STIM_ON:

		if (stimOnDelayTimer.isExpired())
		{
			stimTimer.start(stimPresDur);
			frameIdx = floor(stimTimer.getTime() / (1000 / frameRate));
			//CEnvironment::Instance()->outputMessage("stimTimer=%.2f, frameIdx=%d", stimTimer.getTime(), frameIdx);
			stimMovie[frameIdx]->show();
			stimMovie[frameIdx]->pxSetPosition(scrPix.targetX, scrPix.targetY);
			moveToFront(stimMovie[frameIdx]);
			moveToFront(targetDot);
			timingRecord.stimON = trialTimer.getTime();

			m_state = STATE_SACCADE_ONSET;
			firstEnter = 1;
		}
		break;

	case STATE_SACCADE_ONSET:

		frameIdx = floor(stimTimer.getTime() / (1000 / frameRate));
		if (frameIdx > (frameNum - 1))
			frameIdx = (frameNum - 1);
		//CEnvironment::Instance()->outputMessage("stimTimer=%.2f, frameIdx=%d", stimTimer.getTime(), frameIdx);
		stimMovie[frameIdx]->show();
		stimMovie[frameIdx]->pxSetPosition(scrPix.targetX, scrPix.targetY);
		moveToFront(stimMovie[frameIdx]);
		moveToFront(targetDot);

		Dist = getGazePxDist(scrPix.fixationX, scrPix.fixationY);
		//CEnvironment::Instance()->outputMessage("emEvent1=%d, Dist=%.2f, badData=%d", emEvent1, Dist, badData);
		//CEnvironment::Instance()->outputMessage("saccOn: vel=%f, Dist=%.2f (%.2f) %.2f, badData=%d ", Velocity, Dist1, (1.2*fixRangePix), Dist2, badData);
		saccPrepTime = trialTimer.getTime() - timingRecord.targetDotOn;
		if ((vel1 > 10 && saccPrepTime>150) || DEBUG_FLAG || currTrial.saccMode==0)//(velocity>)
		{
			// SACCADE STARTS
			timingRecord.saccOn = trialTimer.getTime();
			
			saccTimer.start(saccMaxDur);
			m_state = STATE_SACCADE_OFFSET;
		}

		break;

	case STATE_SACCADE_OFFSET:

		frameIdx = floor(stimTimer.getTime() / (1000 / frameRate));
		if (frameIdx > (frameNum - 1))
			frameIdx = (frameNum - 1);
		//CEnvironment::Instance()->outputMessage("stimTimer=%.2f, frameIdx=%d", stimTimer.getTime(), frameIdx);
		stimMovie[frameIdx]->show();
		stimMovie[frameIdx]->pxSetPosition(scrPix.targetX, scrPix.targetY);
		moveToFront(stimMovie[frameIdx]);
		moveToFront(targetDot);

		if (saccTimer.isExpired())
		{
			targetDot->hide();
			timingRecord.quit = trialTimer.getTime();
			trialTimer.stop();


			m_state = STATE_QUIT;
		}
		else
		{
			if ((Velocity<20000) && saccTimer.getTime()>30 && Dist< targetRangePix || DEBUG_FLAG || currTrial.saccMode==0)
			{
				timingRecord.saccOff = trialTimer.getTime();
				targetDot->hide();
				
				m_state = STATE_STIM_OFF;
			}
		}

		break;

	case STATE_STIM_OFF:

		if (stimTimer.isExpired())
		{
			//// mask
			//stimDebug->hide();
			checkerMask->pxSetPosition(scrPix.targetX, scrPix.targetY);
			checkerMask->show();
			moveToFront(checkerMask);
			maskTimer.start(afterMaskDur);

			timingRecord.stimOff = trialTimer.getTime();
			Beep(5000, 150);
			respTimer.start(respTimeLimit);
			currTrial.resp = -1;

			m_state = STATE_WAIT_RESPONSE;
		}
		else
		{

			frameIdx = floor(stimTimer.getTime() / (1000 / frameRate));
			if (frameIdx > (frameNum - 1))
				frameIdx = (frameNum - 1);
			stimMovie[frameIdx]->show();
			stimMovie[frameIdx]->pxSetPosition(scrPix.targetX, scrPix.targetY);
			moveToFront(stimMovie[frameIdx]);
			//CEnvironment::Instance()->outputMessage("stimTimer=%.2f, frameIdx=%d", stimTimer.getTime(), frameIdx);
		}
		break;

	case STATE_WAIT_RESPONSE:
		//CEnvironment::Instance()->outputMessage("1");
		if (respTimer.isExpired() || anyButtonPressed)
		{
			Beep(2000, 150);
			timingRecord.quit = trialTimer.getTime();
			m_state = STATE_QUIT;
			//CEnvironment::Instance()->outputMessage("STATE_QUIT");
		}

		break;

	case STATE_QUIT:

		checkerMask->hide();
		for (int i = 0; i < frameNum; i++)
		{
			stimMovie[i]->hide();
		}

		currTrial.hit = currTrial.key == currTrial.resp;

		endTrial();

		if (!trialTimer.isRunning())
		{
			CEnvironment::Instance()->outputMessage("Data not saved");
			trialTimer.stop();
		}
		else 
		{
			saveData();
			trialTimer.stop();
		}


		
		
		CEnvironment::Instance()->outputMessage("stimulus presentation duration: %f", timingRecord.stimOff- timingRecord.stimON);
		if (currTrial.resp == 1)
			CEnvironment::Instance()->outputMessage("Response: postive");
		if (currTrial.resp == 0)
			CEnvironment::Instance()->outputMessage("Response: negative");
		if (currTrial.resp == -1)
			CEnvironment::Instance()->outputMessage("No Response");

		if (currTrial.hit) CorrectResp++;
		if (currTrial.resp > 0) validIndex++;


		CEnvironment::Instance()->outputMessage("Percent correct response: %f", CorrectResp / validIndex);
		CEnvironment::Instance()->outputMessage("ResponseTrial N: %d", validIndex);
		CEnvironment::Instance()->outputMessage("Trial N: %d", trialIndex + 1);

		trialIndex++;


		if (trialIndex < trialNum)
		{
			if (trialIndex % 10 == 0)
				m_state = STATE_CALIBRATION;
			else
				m_state = STATE_LOADING;
		}
		else
			finalize();

		break;
	}

	
	anyButtonPressed = false;
}
int ExperimentBody::getCurrTrial()
{
	return trialIndex;
}

void ExperimentBody::updateGazePxPos()
{
	CConverter::Instance()->
		a2p(m_samples->x1, m_samples->y1, gazePxX, gazePxY);
	gazePxX += corrX;  gazePxY += corrY;
}

float ExperimentBody::getGazePxPosX()
{
	updateGazePxPos();
	return(gazePxX);
}

float ExperimentBody::getGazePxPosY()
{
	updateGazePxPos();
	return(gazePxY);
}

float ExperimentBody::getGazePxDist(float pX, float pY)
{
	float d = sqrt(pow(pX - getGazePxPosX(), 2) +
		pow(pY - getGazePxPosY(), 2));
	return(d);
}
void ExperimentBody::finalize() {


	string warning("\n-----------------\n\n");
	warning.append("Move the patch to the Right Eye \n");
	warning.append("\n-----------------\n");
	CEnvironment::Instance()->outputMessage(warning.c_str());


	//check this line...
	deleteObject(reCalib_target);
	deleteObject(reCalib_gaze);
	deleteObject(fixationDot);
	deleteObject(targetDot);

	delete[] imgGrat_Float;
	delete[] stimMovie;
}

void ExperimentBody::eventJoypad()
{
	// this additional variable prevents button release
	// events to affect "m_trial.key" value
	int resp = -1;
	if (CDriver_Joypad::Instance()->getButtonStatus(CDriver_Joypad::JPAD_BUTTON_R1))
		resp = 1;
	if (CDriver_Joypad::Instance()->getButtonStatus(CDriver_Joypad::JPAD_BUTTON_L1))
		resp = 0;

	if ((respTimer.isRunning() || m_state == STATE_CALIBRATION) && resp != 0)
	{
		anyButtonPressed = true;
		respTimer.stop();
		currTrial.resp = resp;
		currTrial.respTime = respTimer.getTime();
	}

	if (m_state == STATE_CALIBRATION)
	{
		corrY +=
			CDriver_Joypad::Instance()->
			getButtonStatus(CDriver_Joypad::JPAD_BUTTON_UP) -
			CDriver_Joypad::Instance()->
			getButtonPressed(CDriver_Joypad::JPAD_BUTTON_DOWN);
		corrX +=
			CDriver_Joypad::Instance()->
			getButtonPressed(CDriver_Joypad::JPAD_BUTTON_RGHT) -
			CDriver_Joypad::Instance()->
			getButtonPressed(CDriver_Joypad::JPAD_BUTTON_LEFT);
	}

	bool start = CDriver_Joypad::Instance()->getButtonStatus(CDriver_Joypad::JPAD_BUTTON_STRT);
}


void ExperimentBody::createTrial()
{
	stimPresDur = 1000 / stimPresDur * frameNum;
	currTrial.saccMode = 1;
	saccSize = 6.75;
	currTrial.targetPos = rand() % 2;

	if (currTrial.saccMode)
		CEnvironment::Instance()->outputMessage("saccade trial");
	else
		CEnvironment::Instance()->outputMessage("flash trial");
	currTrial.caseIdx = currTrial.saccMode;


	float sum = 0;
	for (int j = 0; j < BUFFERLEN; j++)
		sum += hit_vec[j + BUFFERLEN * currTrial.caseIdx];
	rate_vec[currTrial.caseIdx] = sum / BUFFERLEN;
		CEnvironment::Instance()->outputMessage("Performance  in flash: %.2f, in saccade: %.2f", rate_vec[0], rate_vec[1]);

	float saccSizePix = CConverter::Instance()->a2px(saccSize * 60);
	
	if (currTrial.targetPos)
	{
		scrPix.targetX = saccSizePix;
		scrPix.targetY = 0;
	}
	else
	{
		scrPix.targetX = -saccSizePix;
		scrPix.targetY = 0;
	}
		

	if (currTrial.saccMode)
	{
		scrPix.fixationX = 0;
		scrPix.fixationY = 0;
	}
	else
	{
		scrPix.fixationX = scrPix.targetX;
		scrPix.fixationY = scrPix.targetY;
	}


}

void ExperimentBody::createObjects()
{
	reCalib_target = addObject(
		new CImagePlane("./img/whitecross.tga"));
	reCalib_target->enableTrasparency(true);
	reCalib_target->hide();
	reCalib_target->pxSetSize(20, 20);

	reCalib_gaze = addObject(
		new CImagePlane("./img/redcross.tga"));
	reCalib_gaze->enableTrasparency(true);
	reCalib_gaze->hide();
	reCalib_gaze->pxSetSize(20, 20);

	fixationDot = addObject(
		new CImagePlane("./img/reddot.tga"));
	fixationDot->enableTrasparency(true);
	fixationDot->pxSetSize(15, 15);
	fixationDot->hide();

	targetDot = addObject(
		new CImagePlane("./img/reddot.tga"));
	targetDot->enableTrasparency(true);
	//	targetDot->pxSetSize(30, 30);
	targetDot->pxSetSize(15, 15);
	targetDot->hide();

	Mask = addObject(new CNoisyImagePlane("./img/mask.tga", 1.0f));
	Mask->pxSetSize(imgGratW, imgGratH);
	Mask->hide();


	
}


void ExperimentBody::createStimuli() {

	currTrial.stimPhase = rand() % 8 + 1;
	currTrial.key = rand()%2;
	for (int i = 0; i < frameNum-1; i++)
	{
		int randIdx = rand() % 200 + 1;
		char stimFileName[100];
		if (currTrial.key)
			sprintf(stimFileName, "./stim/c%d/rand%d/stim_p%d_c%d_%d.bmp", SNR, randIdx, currTrial.stimPhase, SNR, randIdx);
		else
			sprintf(stimFileName, "./stim/c%d/rand%d/noise_c%d_%d.bmp", SNR, randIdx, SNR, randIdx);

		CEnvironment::Instance()->outputMessage(stimFileName);
		stimMovie[i] = addObject(new CImagePlane(stimFileName));
		stimMovie[i]->hide();
	}


	char stimFileName[100];
	sprintf(stimFileName, "./stim/checkerMask.bmp");
	stimMovie[frameNum] = addObject(new CImagePlane(stimFileName));
	stimMovie[frameNum]->hide();
	checkerMask = addObject(new CImagePlane(stimFileName));
	checkerMask->hide();

	/*
	char stimFileName[100];
	sprintf(stimFileName, "./stim/stimDebug.bmp");
	stimDebug = addObject(new CImagePlane(stimFileName));
	stimDebug->hide();
	*/
}



void ExperimentBody::saveData() //check if all variables are stored
{
	
	// General Tags
	storeTrialVariable("experiment", getExperimentName());
	storeTrialVariable("sbj", subjectName);
	storeTrialVariable("startTime", expStartTime);

	// exp setting

	storeTrialVariable("respTimeLimit", respTimeLimit);
	storeTrialVariable("bckLumin", bckLumin);
	storeTrialVariable("screenW", screenW);
	storeTrialVariable("screenH", screenH);
	storeTrialVariable("screenR", screenR);
	storeTrialVariable("imgGratW", imgGratW);
	storeTrialVariable("imgGratH", imgGratH);
	storeTrialVariable("fixRange", fixRange);
	storeTrialVariable("targetRange", targetRange);

	//other individual trial information
	storeTrialVariable("trialIndex", trialIndex);
	storeTrialVariable("corrX", corrX);
	storeTrialVariable("corrY", corrY);
	//storeTrialVariable("stimGrat", currTrial.imgGrat);
	storeTrialVariable("respTime", currTrial.respTime);
	storeTrialVariable("key", currTrial.key);
	storeTrialVariable("resp", currTrial.resp);
	storeTrialVariable("hit", currTrial.hit);
	storeTrialVariable("stimPhase", currTrial.stimPhase);
	storeTrialVariable("idx", currTrial.caseIdx);
	storeTrialVariable("saccMode", currTrial.saccMode);
	storeTrialVariable("targetX", scrPix.targetX);
	storeTrialVariable("targetY", scrPix.targetY);
	storeTrialVariable("fixationX", scrPix.fixationX);
	storeTrialVariable("fixationY", scrPix.fixationY);

	// Timing
	storeTrialVariable("fixDotOnTime", timingRecord.fixDotOn);
	storeTrialVariable("targetDotOnTime", timingRecord.targetDotOn);
	storeTrialVariable("saccOnTime", timingRecord.saccOn);
	storeTrialVariable("saccOffTime", timingRecord.saccOff);
	storeTrialVariable("stimOnTime", timingRecord.stimON);
	storeTrialVariable("stimOffTime", timingRecord.stimOff);
	storeTrialVariable("quitTime", timingRecord.quit);

	saveTrial(saveDirectory);
}