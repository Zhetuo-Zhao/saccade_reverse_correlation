
// PostSaccSens_Ecc.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
	#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include <emil/emil.hpp>

const std::string CFG_FIX_MODE = "fixationMode";
const std::string CFG_STABILIZED = "stabilized";
const std::string CFG_FLASHED = "flashed";
const std::string CFG_FEEDBACK = "feedback";
const std::string CFG_PEST_STEP = "pestStep";
const std::string CFG_PEST_TARGET = "pestTarget";

const std::string CFG_DATA_DESTINATION = "DataDestination";

const std::string CFG_SUBJECT_NAME = "Subject";
const std::string CFG_BACKGROUND_GRAY = "backgroundGray";

const std::string CFG_SCREEN_W = "screenW";
const std::string CFG_SCREEN_H = "screenH";
const std::string CFG_SCREEN_R = "screenR";

// Timing
const std::string CFG_RAMP_TIME = "rampTime";

const std::string CFG_PRES_TIME_0 = "presTime0";
const std::string CFG_PRES_TIME_1 = "presTime1";
const std::string CFG_PRES_TIME_2 = "presTime2";
const std::string CFG_PRES_TIME_3 = "presTime3";
const std::string CFG_PRES_TIME_4 = "presTime4";
const std::string CFG_PRES_TIME_5 = "presTime5";
const std::string CFG_PRES_TIME_6 = "presTime6";

const std::string CFG_ECC_LEV = "eccLev";
const std::string CFG_ECC_0 = "ecc0";
const std::string CFG_ECC_1 = "ecc1";
const std::string CFG_ECC_2 = "ecc2";
const std::string CFG_ECC_3 = "ecc3";
const std::string CFG_ECC_4 = "ecc4";
const std::string CFG_ECC_5 = "ecc5";
const std::string CFG_ECC_6 = "ecc6";

const std::string CFG_RESP_TIME = "respTime";
const std::string CFG_LOAD_TIME = "loadTime";
const std::string CFG_FIX_TIME = "fixTime";
const std::string CFG_FIX_TIME_RNG = "fixTimeRange";

//Limiters
const std::string CFG_CONTRAST_LEV = "contrastLev";
const std::string CFG_PRES_TIME_LEV = "presTimeLev";
const std::string CFG_FIX_LOC_NO = "fixLocNo";
const std::string CFG_NUMB_TRIAL = "numbTrial";
const std::string CFG_NUMB_IMG = "numbImage"; // maybe just for noise image?


// Exp Params
const std::string CFG_SPAT_FREQ_LEV = "spFreqLev";
//const std::string CFG_SPAT_FREQ = "spFreq";
const std::string CFG_SPAT_FREQ_0 = "spFreq0";
const std::string CFG_SPAT_FREQ_1 = "spFreq1";
const std::string CFG_SPAT_FREQ_2 = "spFreq2";
//const std::string CFG_ECC = "eccentricity";
const std::string CFG_CONTRAST_BACK = "contrastback";
const std::string CFG_MIN_PEST_STEP = "minPestStep";

const std::string CFG_FIX_ECC = "fixEcc";
const std::string CFG_FIX_AREA_C = "fixAreaC";
const std::string CFG_FIX_AREA_P = "fixAreaP";
const std::string CFG_FIX_ANGLE_OFFSET = "fixAngleOffset";


const std::string CFG_CONTRAST_0 = "contrast0";
const std::string CFG_CONTRAST_1 = "contrast1";
const std::string CFG_CONTRAST_2 = "contrast2";
const std::string CFG_CONTRAST_3 = "contrast3";
const std::string CFG_CONTRAST_4 = "contrast4";
const std::string CFG_CONTRAST_5 = "contrast5";
const std::string CFG_CONTRAST_6 = "contrast6";

const std::string CFG_DEBUG = "debug";
const std::string CFG_NRECAL = "NRecal";

const std::string CFG_AWID = "annulusWidth";
const std::string CFG_GSD = "gaussianSD";
const std::string CFG_GSD2 = "gaussianSD_2";
const std::string CFG_GT = "GratingType";

// PostSaccSens_EccApp:
// See DriftControl_spf.cpp for the implementation of this class
//

class CPostSaccSens_EccApp : public CWinApp
{
public:
	CPostSaccSens_EccApp();

// Overrides
public:
	virtual BOOL InitInstance();

// Implementation

	DECLARE_MESSAGE_MAP()
	
	// Configuration file
	CCfgFile m_paramsFile;
};

extern CPostSaccSens_EccApp theApp;
