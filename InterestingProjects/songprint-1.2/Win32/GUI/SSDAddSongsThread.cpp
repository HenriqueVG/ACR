// SSDAddSongsThread.cpp : implementation of the CSSDAddSongsThread class
//
// Implementation of the CSSDAddSongsThread class, a CWinThread derived class
// that handles the adding of songs in the background

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
#include "SSDAddSongsThread.h"
#include "SSDMainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CAddSongsThread

IMPLEMENT_DYNCREATE(CSSDAddSongsThread, CWinThread)

CSSDAddSongsThread::CSSDAddSongsThread()
{
    // Init member variables on creation

	m_bTerminateThread=FALSE;
    m_bStopAndClear=FALSE;

    // Prevent CWinThread from auto deleting itself

	m_bAutoDelete=FALSE;
}

CSSDAddSongsThread::~CSSDAddSongsThread()
{
}

/**
 * InitInstance
 *
 * Always returns TRUE for CWinThread derived class. 
 * 
 * Return values:
 *  TRUE always
 * Exceptions:
 *  None
 */
BOOL
CSSDAddSongsThread::InitInstance()
{
	return TRUE;
}

/**
 * ExitInstance
 *
 * Call base class for exit code
 * 
 * Return values:
 *  Exit code
 * Exceptions:
 *  None
 */
int
CSSDAddSongsThread::ExitInstance()
{
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSSDAddSongsThread, CWinThread)
	//{{AFX_MSG_MAP(CSSDAddSongsThread)
	ON_THREAD_MESSAGE(WM_ADDSONGSTHREAD_ADD, OnThreadAddSongs)
	ON_THREAD_MESSAGE(WM_ADDSONGSTHREAD_REACTIVATE, OnThreadReactivate)
	ON_THREAD_MESSAGE(WM_THREAD_DIE, OnThreadDie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDAddSongsThread message handlers


/**
 * OnThreadDie
 *
 * Event call to order this thread to Quit normally with code 0
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDAddSongsThread::OnThreadDie()
{
	::PostQuitMessage(0);
}


/**
 * OnThreadReactivate
 *
 * Restart the thread after Stop/Clear and Terminate flags set. This will
 * allow AddSong messages to resume
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDAddSongsThread::OnThreadReactivate()
{
    m_bStopAndClear=FALSE;
    m_bTerminateThread=FALSE;
}


/**
 * OnThreadAddSongs
 *
 * @wParam: Windows Message WPARAM variable, not used
 * @lParam: Pointer to an AddSongsQuerytStruct
 *
 * Add songs based on information from the AddSongsQuery structure. For a
 * list of files immediately add them. For a directory path call 
 * RecurseSubDirs to search and add files in directories.
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDAddSongsThread::OnThreadAddSongs(WPARAM wParam, LPARAM lParam)
{

    // If stop and clear flag set, then exit

    if (m_bStopAndClear) return;

	// Get AddSongsQueryStructure

    m_pAddQuery=(AddSongsQueryStruct*)lParam;

    if (m_pAddQuery->bAddingDirectories==TRUE)
    {

		// List is a directory, call directory search/add routine

		int nTotalFound=RecurseSubDirs(m_pAddQuery->pFiles->RemoveHead());
		
		// Update Status Bar for done

        if (!m_bTerminateThread)
        {
		    CString strTemp;
		    strTemp.Format("Done scanning - %d files found", nTotalFound);
		    ::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_UPDATESTATUSBAR, 0, (LPARAM)((LPCTSTR)strTemp));
        }
    }
    else
    {
        // List is all Filenames, Start adding songs

        while (m_pAddQuery->pFiles->IsEmpty()==FALSE)
        {
            AddSong(&m_pAddQuery->pFiles->RemoveHead());
            AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_UPDATELISTCOUNTS, 0, 0);
			if (m_bTerminateThread)
                break;
        }
    }

	// Post message to SongSigThread to start processing songs
	
    if (!m_bTerminateThread)
        ((CSSDMainFrame*)AfxGetApp()->m_pMainWnd)->m_pSongSigThread->PostThreadMessage(WM_SONGSIGTHREAD_NEWSONGS, 0, (LPARAM)m_pAddQuery->pDoc);

	// This routine is responsible for cleanup. Empty the file list, 
	// de-allocate it, and deallocate the AddSongQuery structure

	m_pAddQuery->pFiles->RemoveAll();
	delete m_pAddQuery->pFiles;
    delete m_pAddQuery;


}


/////////////////////////////////////////////////////////////////////////////
// CSSDAddSongsThread routines

/**
 * RecurseSubDirs
 *
 * @CurrentDir: CString containing the path to search for files to add
 *
 * Recursive routine to search for directories to add files. If the
 * the AddSongQuery structure has the bRecurseDirs flag set this routine
 * will search and process all subdirectories recursively. This routine makes
 * checks at key points for the m_bTerminateThread flag to expedite immediate
 * exit.
 * 
 * Return values:
 *  Total Number of Songs added from AddSongsInDir calls
 * Exceptions:
 *  None
 */
int
CSSDAddSongsThread::RecurseSubDirs(CString CurrentDir)
{
	CString strFileName;
	int nTotalFilesFound=0;

    if (m_bTerminateThread) return 0;

	// Update Status Bar

	strFileName="Scanning "+CurrentDir;
	::SendMessage(AfxGetApp()->m_pMainWnd->m_hWnd,WM_UPDATESTATUSBAR, 0, (LPARAM)((LPCTSTR)strFileName));

	// Add trailing backslash

	if (CurrentDir.Right(1)!="\\")
		CurrentDir=CurrentDir+"\\";

    // Add files with supported extensions

    nTotalFilesFound+=AddSongsInDir(&CurrentDir,"*.MP3");
    nTotalFilesFound+=AddSongsInDir(&CurrentDir,"*.OGG");
    nTotalFilesFound+=AddSongsInDir(&CurrentDir,"*.WAV");

    if (m_bTerminateThread) return 0;

	// Search/recurse subdirectories if set

    if (m_pAddQuery->bRecurseDirs)
    {
		WIN32_FIND_DATA fd;
	    HANDLE hFind = ::FindFirstFile (CurrentDir+"*.*", &fd);
	    if (hFind!=INVALID_HANDLE_VALUE)
	    {
    		bool bSearchOK=TRUE;
		    while (bSearchOK)
		    {
    			strFileName = (LPCTSTR) &fd.cFileName;
			    if ((fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) && (strFileName!=".") && (strFileName!=".."))
    				nTotalFilesFound+=RecurseSubDirs(CurrentDir+fd.cFileName);
			    if ((::FindNextFile(hFind, &fd)==0) || (m_bTerminateThread==TRUE))
    				bSearchOK=FALSE;
		    }
		    ::FindClose(hFind);
        }
	}

	return nTotalFilesFound;
}


/**
 * AddSongsInDir
 *
 * @pCurrentDir: Pointer to CString containing the path to search for files 
 *  to add
 * @SearchSpec: CString containing the search specification (*.MP3, etc)
 *
 * Adds all files in pCurrentDir that match SearchSpec to the document by
 * calling AddSong
 * 
 * Return values:
 *  Total Number of Songs added
 * Exceptions:
 *  None
 */
int
CSSDAddSongsThread::AddSongsInDir(CString* pCurrentDir, CString SearchSpec)
{
	CString strFileName;
	int nTotalFilesFound=0;

    // Do not run if terminating

    if (m_bTerminateThread) return 0;

	// Find/add all files

    WIN32_FIND_DATA fd;
	HANDLE hFind = ::FindFirstFile (*pCurrentDir+SearchSpec, &fd);
	if (hFind!=INVALID_HANDLE_VALUE)
	{
		bool bSearchOK=TRUE;
		while (bSearchOK)
		{
            // Add filename to document song list

            strFileName=*pCurrentDir+fd.cFileName;
            AddSong(&strFileName);
			nTotalFilesFound++;

            // Notify list view that the number of items has changed

            AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_UPDATELISTCOUNTS, 0, 0);

            // Continue until done or thread termination flag

			if ((::FindNextFile(hFind, &fd)==0) || (m_bTerminateThread==TRUE))
				bSearchOK=FALSE;
		}
		::FindClose(hFind);
	}

    return nTotalFilesFound;

}


/**
 * AddSong
 *
 * @pstrPath: Pointer to CString containg the filename and path
 *
 * Creates a new song record in the document, status is set to SONG_NEW.
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDAddSongsThread::AddSong(CString *pstrPath)
{
    SongDataStruct*         pNewSong;


    // Initialize new SongData structure

    pNewSong = new SongDataStruct;
    pNewSong->Path=*pstrPath;
    pNewSong->Status=SONG_NEW;

    // Insert it into the document song list, also check for terminate flag
    // as this is a mutex waiting point

    CSingleLock SongMutexLock(&m_pAddQuery->pDoc->m_SongListMutex);
    SongMutexLock.Lock();
    if (!m_bTerminateThread) m_pAddQuery->pDoc->m_SongList.Add(pNewSong);
    SongMutexLock.Unlock();

}


