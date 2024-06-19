// SSDMainFrame.cpp
//
// Implementation of the CSSDMainFrame class, a CFrameWnd derived class that
// handles the application frame and holds the menu, status bar, toolbar and
// the application threads.

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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307  USA
 */

#include "stdafx.h"
#include "SongSigDemo.h"
#include "SSDDocument.h"
#include "SSDSongDataView.h"
#include "SSDMainFrame.h"
#include "SSDDirChooserDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const MAX_PATH_LENGTH = 255;

/////////////////////////////////////////////////////////////////////////////
// CSSDMainFrame

IMPLEMENT_DYNCREATE(CSSDMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CSSDMainFrame, CFrameWnd)
	//{{AFX_MSG_MAP(CSSDMainFrame)
	ON_WM_CREATE()
	ON_UPDATE_COMMAND_UI(ID_ADDSONGS, OnUpdateAddsongs)
	ON_COMMAND(ID_ADDSONGS, OnAddsongs)
	ON_MESSAGE(WM_UPDATESTATUSBAR, OnUpdateStatusBar)
	ON_UPDATE_COMMAND_UI(ID_ADDDIRECTORY, OnUpdateAddDirectory)
	ON_COMMAND(ID_ADDDIRECTORY, OnAddDirectory)
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, OnFilePrintPreview)
	ON_MESSAGE(WM_UPDATESTATUSBAR, OnUpdateStatusBar)
	ON_MESSAGE(WM_UPDATESONGLIST, OnUpdateSongList)
	ON_COMMAND(ID_FILE_PRINT, OnFilePrint)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CSSDMainFrame construction/destruction

CSSDMainFrame::CSSDMainFrame()
{
}

CSSDMainFrame::~CSSDMainFrame()
{
}


/**
 * OnCreate
 *
 * Called by MFC on the creation of the main frame. This routine will
 * initialize all main frame controls and start all threads.
 * 
 * Return values:
 *  0 for sucess, -1 for fail (TRUE/FALSE)
 * Exceptions:
 *  None
 */
int
CSSDMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{

    // Call base class OnCreate

    if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
    // Create the toolbar

	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

    // Creat the status bar

	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

    // Setup toolbar docking

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);

    // Create the threads

	m_pAddSongsThread = (CSSDAddSongsThread*)AfxBeginThread(RUNTIME_CLASS(CSSDAddSongsThread), 
        THREAD_PRIORITY_NORMAL, 0, 0, NULL);

	m_pSongSigThread = (CSSDSongSigThread*)AfxBeginThread(RUNTIME_CLASS(CSSDSongSigThread), 
        THREAD_PRIORITY_BELOW_NORMAL, 0, 0, NULL);

	return 0;
}


/**
 * PreCreateWindow
 *
 * @cs: A MFC CREATESTRUCT reference containing all settings for the new
 *  window
 *
 * Called by MFC on the before creation of the window. Since we are
 * not modifying any styles the base class is called.
 * 
 * Return values:
 *  TRUE/FALSE based on call to base class PreCreateWindow
 * Exceptions:
 *  None
 */
BOOL
CSSDMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CSSDMainFrame diagnostics

#ifdef _DEBUG
void
CSSDMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void
CSSDMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSSDMainFrame message handlers

/**
 * OnCreateClient
 *
 * Called by MFC on the creation of the window clint. This routine will
 * create a splitter view and insert the CSDDSongListView and
 * CSSDSongDataView view classes into the splitter. The splitter will be
 * initially divided with 1/2 the size for each view.
 *
 * Return values:
 *  TRUE/FALSE based on creation of the splitter window
 * Exceptions:
 *  None
 */
BOOL
CSSDMainFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
    // Client Splitter Window
   
    BOOL bCreateSpltr = m_wndSplitter.CreateStatic(this, 2, 1);

    // Create Views, song list is 1/2 of available space, song data is the rest

    RECT rc;
    GetClientRect(&rc);
    m_wndSplitter.CreateView(0,0,RUNTIME_CLASS(CSSDSongListView), CSize(0,rc.bottom/2), pContext);
    m_wndSplitter.CreateView(1,0,RUNTIME_CLASS(CSSDSongDataView), CSize(0,0), pContext);

    return (bCreateSpltr);
}


/**
 * OnUpdateStatusBar
 *
 * Called to update the text of the status bar from the views and threads
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDMainFrame::OnUpdateStatusBar(WPARAM wParam, LPARAM lParam)
{
	// Check for NULL as precaution

	if (lParam!=NULL)
		m_wndStatusBar.SetPaneText(0, (char *)lParam, TRUE);
}


/**
 * OnUpdateSongList
 *
 * Called to redraw a song in the list view and update the data view if the 
 * item is selected
 *
 * @wParam: Windows Message WPARAM variable, not used
 * @lParam: Index of the song in the document/listview
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDMainFrame::OnUpdateSongList(WPARAM wParam, LPARAM lParam)
{

    // Get the document

    CSSDDocument* pDoc=(CSSDDocument*)GetActiveDocument();
    ASSERT_VALID(pDoc);
    
	// Check for NULL as precaution

    if (pDoc->m_pSongListCtrl!=NULL)
    {
        // Update item

        pDoc->m_pSongListCtrl->Update(lParam);

        // Update data view if this item is highlighted in the list view

        if (pDoc->m_pSongListCtrl->GetItemState(lParam, LVIS_SELECTED)!=0)
            SendMessageToDescendants(WM_UPDATESONGDATA, 0, lParam);
    }
}


/**
 * OnUpdateAddDirectory
 *
 * Called by MFC to check for enable/disable of the AddDirectory command ID.
 *
 * @pCmdUI: MFC UI command pointer
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDMainFrame::OnUpdateAddDirectory(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(TRUE);   // Command is always enabled
}


/**
 * OnUpdateAddSongs
 *
 * Called by MFC to check for enable/disable of the AddSongs command ID.
 *
 * @pCmdUI: MFC UI command pointer
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDMainFrame::OnUpdateAddsongs(CCmdUI* pCmdUI) 
{
    pCmdUI->Enable(TRUE);   // Command is always enabled
}


/**
 * OnAddSongs
 *
 * Called when user selects Add Songs. Routine display the dialog for picking
 * files and sends them to the add songs thread for processing
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDMainFrame::OnAddsongs() 
{

    AddSongsQueryStruct*    pAddQuery;

    // Allocate 1K string buffer for files

    TCHAR FileBuffer[1023];
    FileBuffer[0]=0;

    // Create/setup/launch add file dialog

    CString Filter="MPEG-3 Files (*.mp3)|*.mp3|Vorbis Files (*.ogg)|*.ogg|Wave Files (*.wav)|(*.wav)||";
    CFileDialog dlgFileDialog(TRUE, "mp3",NULL, OFN_ALLOWMULTISELECT | OFN_FILEMUSTEXIST |
        OFN_HIDEREADONLY | OFN_PATHMUSTEXIST, Filter, this);
    dlgFileDialog.m_ofn.lpstrTitle="Add Songs";
    dlgFileDialog.m_ofn.lpstrFile=(char *)&FileBuffer;
    dlgFileDialog.m_ofn.nMaxFile=1023;
    int nResult=dlgFileDialog.DoModal();

    if (nResult==IDOK)
    {
        // Create new add query

        pAddQuery = new AddSongsQueryStruct;

        // Assemble filenames into string list

        pAddQuery->pFiles=new CStringList;
        POSITION pos;
        pos=dlgFileDialog.GetStartPosition();
        while (pos)
            pAddQuery->pFiles->AddTail(dlgFileDialog.GetNextPathName(pos));

        // Set Add Query flags, get document pointer

        pAddQuery->bAddingDirectories=FALSE;
        pAddQuery->bRecurseDirs=FALSE;
        CSSDDocument* pDoc=(CSSDDocument*)GetActiveDocument();
        ASSERT_VALID(pDoc);
        pAddQuery->pDoc=pDoc;

        // Send data to add songs thread

        m_pAddSongsThread->PostThreadMessage(WM_ADDSONGSTHREAD_ADD, 0, LPARAM(pAddQuery));
    }
}


/**
 * OnAddDirectory
 *
 * Called when user selects Add Directory. Routine display the dialog for
 * picking a directory and sends the directory and recurse subdirectory 
 * setting to the add songs thread for processing
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDMainFrame::OnAddDirectory() 
{

    CSSDDirChooserDialog    DirDialog;
    int nResult;
    AddSongsQueryStruct*    pAddQuery;

    nResult=DirDialog.DoModal();

    if (nResult==IDOK)
    {
        // Create new query

        pAddQuery = new AddSongsQueryStruct;

        // Place directory into string list

        pAddQuery->pFiles=new CStringList;
        pAddQuery->pFiles->AddTail(DirDialog.m_strDirectoryChoosen);

        // Send data to thread/start thread

        pAddQuery->pDoc=(CSSDDocument*)GetActiveDocument();
        ASSERT_VALID(pAddQuery->pDoc);
        pAddQuery->bAddingDirectories=TRUE;
        pAddQuery->bRecurseDirs=DirDialog.m_bRecurseDirs;
        m_pAddSongsThread->PostThreadMessage(WM_ADDSONGSTHREAD_ADD, 0, LPARAM(pAddQuery));
    }
    	
}

	
/**
 * OnClose
 *
 * Called when user selects to close the application. This routine orders
 * threads to close and re-posts OnClose if they are still running to allow
 * safe normal message dispatch until the threads are done.
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if ::GetExitCodeThread fails
 */
void
CSSDMainFrame::OnClose() 
{
    // Set thread termination flags, order threads to die, set Signature thread's priority up so it will
    // finish faster
/*
	if (m_pAddSongsThread)
	{
		m_pAddSongsThread->m_bTerminateThread=TRUE;
		m_pAddSongsThread->PostThreadMessage(WM_THREAD_DIE, 0, 0);
	}
	
	if (m_pSongSigThread)
	{
		m_pSongSigThread->SetThreadPriority(THREAD_PRIORITY_HIGHEST);
		m_pSongSigThread->m_bTerminateThread=TRUE;
		m_pSongSigThread->PostThreadMessage(WM_THREAD_DIE, 0, 0);
	}

    // Safe thread termination check, verify exit codes for both threads
    // before allowing application exit

	int nCount = 0;
	DWORD dwStatus;
	if (m_pAddSongsThread!= NULL)
	{
		VERIFY(::GetExitCodeThread(m_pAddSongsThread->m_hThread, &dwStatus));
		if (dwStatus == STILL_ACTIVE)
			nCount++;
		else
		{
			delete m_pAddSongsThread;
			m_pAddSongsThread = NULL;
		}
	}
	if (m_pSongSigThread!= NULL)
	{
		VERIFY(::GetExitCodeThread(m_pSongSigThread->m_hThread, &dwStatus));
		if (dwStatus == STILL_ACTIVE)
			nCount++;
		else
		{
			delete m_pSongSigThread;
			m_pSongSigThread = NULL;
		}
	}

    // If thread still active, re-post close and leave. This allows all 
    // messages queued to continue normally and gives still running threads
    // time to deallocate and terminate.

	if (nCount == 0)
    {
        // Get Temporary Directory and remove temporary file if it exists

        CString strTempPath;
        GetTempPath(MAX_PATH_LENGTH, strTempPath.GetBuffer(MAX_PATH_LENGTH));
        strTempPath.ReleaseBuffer();
        if (strTempPath.Right(1)!='\\') strTempPath+='\\';

        CFileStatus fs;
        if (CFile::GetStatus(strTempPath+"URLTemp.url",fs))
            CFile::Remove(strTempPath+"URLTemp.url");
        
    	CFrameWnd::OnClose();
    }
	else
		PostMessage(WM_CLOSE, 0, 0);
*/
    	CFrameWnd::OnClose();
}


/**
 * OnFilePrintPreview
 *
 * This routine passes on the Print Preview command to the song data view
 * where it will be prcessed. This catches selections of this command when
 * the Song data view is not active
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void CSSDMainFrame::OnFilePrintPreview() 
{
    // Get the Data View Pane

    while (m_wndSplitter.GetActivePane()->IsKindOf(RUNTIME_CLASS(CSSDSongDataView))==FALSE)
        m_wndSplitter.ActivateNext();

    // Send Message

    ::SendMessage(m_wndSplitter.GetActivePane()->m_hWnd, WM_COMMAND, ID_FILE_PRINT_PREVIEW, 0);
}


/**
 * OnFilePrint
 *
 * This routine passes on the Print command to the song data view where it 
 * will be prcessed. This catches selections of this command when the Song 
 * data view is not active
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void CSSDMainFrame::OnFilePrint() 
{
    // Get the Data View Pane

    while (m_wndSplitter.GetActivePane()->IsKindOf(RUNTIME_CLASS(CSSDSongDataView))==FALSE)
        m_wndSplitter.ActivateNext();

    // Send Message

    ::SendMessage(m_wndSplitter.GetActivePane()->m_hWnd, WM_COMMAND, ID_FILE_PRINT, 0);
}
