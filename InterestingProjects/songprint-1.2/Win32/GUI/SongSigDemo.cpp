// SongSigDemo.cpp : Defines the class behaviors for the application.
//
// The main application implementation. Creates the application and
// manages the about box.

/*  
 *
 *  Copyright (C) 2000  eTantrum, Inc.
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */


#include "stdafx.h"
#include "SongSigDemo.h"
#include "SSDMainFrame.h"
#include "SSDDocument.h"
#include "SSDSongListView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSDApp

BEGIN_MESSAGE_MAP(CSSDApp, CWinApp)
	//{{AFX_MSG_MAP(CSSDApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDApp construction

CSSDApp::CSSDApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CSSDApp object

CSSDApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CSSDApp initialization

/**
 * InitInstance
 *
 * MFC based InitInstance. Run enables, setup and load registry settings,
 * register the document and view templates, handle DDE and command line
 * open, and display the app
 *
 * Return values:
 *  TRUE/FALSE based on if the process was sucessfull
 * Exceptions:
 *  None
 */
BOOL 
CSSDApp::InitInstance()
{
	AfxEnableControlContainer();

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.

	SetRegistryKey(_T("eTantrum"));

	LoadStdProfileSettings();  // Load standard INI file options (including MRU)

	// Register the application's document templates.  Document templates
	//  serve as the connection between documents, frame windows and views.

	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CSSDDocument),
		RUNTIME_CLASS(CSSDMainFrame),       // main SDI frame window
		RUNTIME_CLASS(CSSDSongListView));
	AddDocTemplate(pDocTemplate);

    // Enable DDE Execute open

	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	// Parse command line for standard shell commands, DDE, file open

	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);

	// Dispatch commands specified on the command line

	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// The one and only window has been initialized, so show and update it.

	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	// Enable drag/drop open

	m_pMainWnd->DragAcceptFiles();

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
		// No message handlers
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}


/**
 * DoDataEnxchange
 *
 * @pDX: Pointer to CDataExchange class setup by MFC
 *
 * Call parent instance of DoDataExchangeMFC based InitInstance. 
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void 
CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/**
 * OnAppAbout
 *
 * Launch the About dialog box in modal mode
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

