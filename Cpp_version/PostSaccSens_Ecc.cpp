
// PostSaccSens_Ecc.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "PostSaccSens_Ecc.h"
#include "ExperimentBody.h"

#include "myAutoCalibration.h"
#include "myManualCalibrator2.h"

#include <emil2-console\CEMILConsole.hpp>
#include <emil2-console\CWinEMIL.hpp>

#include <ExpDDPICalibration.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CEyeChartEApp

BEGIN_MESSAGE_MAP(CPostSaccSens_EccApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CEyeChartEApp construction

CPostSaccSens_EccApp::CPostSaccSens_EccApp()
{
	// support Restart Manager
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

}


// The one and only CEyeChartEApp object

CPostSaccSens_EccApp theApp;


// CEyeChartEApp initialization

BOOL CPostSaccSens_EccApp::InitInstance()
{
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);
	CWinApp::InitInstance();
	AfxEnableControlContainer();
	CShellManager *pShellManager = new CShellManager;
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
	SetRegistryKey(_T("EyeRIS @ APLab"));

	// EyeRIS ------------------------------------------------------------------
	// Initialize the library
	// Insert this line if the library needs to load a specific configuration file
	CWinEMIL::Instance()->initialize("emil-library-monocular-singleROG.cfg");
	//CWinEMIL::Instance()->initialize("C:/EyeRIS/system/config/emil-library.cfg");
	// Put your code here ======================================================

	// Load the specified configuration file
	std::string DestinationDir = "./mydata/" + std::string(subjectName) + "/calibration";


	
	// to set the initial target window
	/*ExTarget* target = new ExTarget(HResolution, VResolution, RefreshRate);	
	target->setBackgroundColor(127, 127, 127);*/
	//CWinEMIL::Instance()->addExperiment(target);
	ExpDDPICalibration* ddpiCalibrator = new ExpDDPICalibration(screenW, screenH, screenR);
	ddpiCalibrator->setBackground(127, 127, 127);
	ddpiCalibrator->setGridColor(0, 0, 0);
	ddpiCalibrator->setGridDotSize(10);
	ddpiCalibrator->setGridSpacing(600, 600);
	//CWinEMIL::Instance()->addExperiment(new ExpDDPICalibration(HResolution, VResolution, RefreshRate));
	CWinEMIL::Instance()->addExperiment(ddpiCalibrator);

	
	// updated call for eyeris 2.4.1 version
	myAutoCalibration* autoCalibrator = new myAutoCalibration(screenW, screenH, screenR);
	autoCalibrator->setOutputDir(DestinationDir);
	autoCalibrator->setBackgroundColor(127, 127, 127);
	autoCalibrator->setPointOffset(250, 250); //Set calibration grid size here, the parameters define the spacing between the calibration points
	CWinEMIL::Instance()->addExperiment(autoCalibrator);


	//////// right eye calibration
	//myManualCalibrator2* manualCalibrator2 = new myManualCalibrator2(HResolution, VResolution, RefreshRate, EOS_EYE_1, false);
	myManualCalibrator2* manualCalibrator2 = new myManualCalibrator2(screenW, screenH, screenR, false);
	manualCalibrator2->setOutputDir(DestinationDir);
	manualCalibrator2->setBackgroundColor(127, 127, 127);
	manualCalibrator2->setPointOffset(250, 250); //Set calibration grid size here, the parameters define the spacing between the calibration points
	CWinEMIL::Instance()->addExperiment(manualCalibrator2);

	CWinEMIL::Instance()->addExperiment(new ExperimentBody(screenW, screenH, screenR));

	// =========================================================================
	//CEyeChartEDlg dlg;
	CEMILConsole dlg("emil-console-monocular-singleROG.cfg");
	//CEMILConsole dlg("C:/EyeRIS/system/config/emil-console.cfg");
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	 
	// Destroy the the library
	CWinEMIL::Destroy();
	// EyeRIS ------------------------------------------------------------------
	
	// Delete the shell manager created above.
	if (pShellManager != nullptr)
	{
		delete pShellManager;
	}

#if !defined(_AFXDLL) && !defined(_AFX_NO_MFC_CONTROLS_IN_DIALOGS)
	ControlBarCleanUp();
#endif

	// Since the dialog has been closed, return FALSE so that we exit the
	//  application, rather than start the application's message pump.
	return FALSE;
}

