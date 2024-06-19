// SSDDocument.cpp : implementation of the CSSDDocument class
//
// Implementation of the CSSDDocument class, an MFC CDocument derived class
// that stores all songs data

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
#include "SSDMainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSDDocument

IMPLEMENT_DYNCREATE(CSSDDocument, CDocument)

BEGIN_MESSAGE_MAP(CSSDDocument, CDocument)
	//{{AFX_MSG_MAP(CSSDDocument)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDDocument construction/destruction

CSSDDocument::CSSDDocument() : m_SongListMutex(FALSE, NULL)
{
}

CSSDDocument::~CSSDDocument()
{
	// Destroy all song data on destruction

	SongDataStruct* pCurSong;
    for (int nCount=0; nCount<m_SongList.GetSize(); nCount++)
	{
		pCurSong=(SongDataStruct*)m_SongList.GetAt(nCount);
        delete pCurSong;
	}
    m_SongList.RemoveAll();
}


/**
 * OnNewDocument
 *
 * Called by MFC on the creation of a new document. If there is a document
 * already open, the data for it will be destroyed
 * 
 * Return values:
 *  TRUE/FALSE from base class OnNewDocument()
 * Exceptions:
 *  None
 */
BOOL
CSSDDocument::OnNewDocument()
{
	SongDataStruct* pCurSong;

    // Check for data

	CSingleLock SongMutexLock(&m_SongListMutex);
    SongMutexLock.Lock();
    if (m_SongList.GetSize()>0)
    {
        // Mark add song thread to stop until re-activated
    
        ((CSSDMainFrame*)AfxGetApp()->m_pMainWnd)->m_pAddSongsThread->m_bStopAndClear=TRUE;
        ((CSSDMainFrame*)AfxGetApp()->m_pMainWnd)->m_pAddSongsThread->m_bTerminateThread=TRUE;

    	// Destroy all song data on destruction

        for (int nCount=0; nCount<m_SongList.GetSize(); nCount++)
	    {
		    pCurSong=(SongDataStruct*)m_SongList.GetAt(nCount);
            delete pCurSong;
	    }
        m_SongList.RemoveAll();

        // Clear Status bar


        // Re-activate Add Songs Thread

        ((CSSDMainFrame*)AfxGetApp()->m_pMainWnd)->m_pAddSongsThread->PostThreadMessage(WM_ADDSONGSTHREAD_REACTIVATE, 0, 0);
    }
    SongMutexLock.Unlock();

	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: add reinitialization code here
	// (SDI documents will reuse this document)

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CSSDDocument serialization


/**
 * Serialize
 *
 * Called by MFC on for saving and loading of documents to media.
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void CSSDDocument::Serialize(CArchive& ar)
{
	SongDataStruct*	pSongData;
	int	nSize;

	if (ar.IsStoring())
	{
        // Get mutex, and save song data

	    CSingleLock SongMutexLock(&m_SongListMutex);
	    SongMutexLock.Lock();

        // Save the number of entries

		nSize=m_SongList.GetSize();
		ar << nSize;

        // Save all song data

		for (int nCount=0; nCount <nSize ; nCount++)
		{
			pSongData=(SongDataStruct* )m_SongList.GetAt(nCount);
			ar << pSongData->Path;
            ar << pSongData->Status;
			ar << pSongData->Title;
            ar << pSongData->URL;
			pSongData->Artists.Serialize(ar);
			pSongData->Albums.Serialize(ar);
			pSongData->Genres.Serialize(ar);
		}
	    SongMutexLock.Unlock();
	}
	else
	{

        // Get mutex, and load song data

		CSingleLock SongMutexLock(&m_SongListMutex);
	    SongMutexLock.Lock();

        // Temp variable to load enum

        int nStatus;

        // Get all song data

		ar >> nSize;
		for (int nCount=0; nCount < nSize; nCount++)
		{
			pSongData= new SongDataStruct;
			ar >> pSongData->Path;
            ar >> nStatus;
            pSongData->Status=(SongStatusType)nStatus;
            ar >> pSongData->Title;
            ar >> pSongData->URL;
			pSongData->Artists.Serialize(ar);
			pSongData->Albums.Serialize(ar);
			pSongData->Genres.Serialize(ar);
			m_SongList.Add(pSongData);
		}
	    SongMutexLock.Unlock();

        // Notify list view to update count

        if (AfxGetApp()->m_pMainWnd)
            AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_UPDATELISTCOUNTS, 0, 0);

    	// Post message to SongSigThread to start processing songs
	
        ((CSSDMainFrame*)AfxGetApp()->m_pMainWnd)->m_pSongSigThread->PostThreadMessage(WM_SONGSIGTHREAD_NEWSONGS, 0, (LPARAM)this);

        // Redraw everythign

		UpdateAllViews(NULL);
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSSDDocument diagnostics

#ifdef _DEBUG
void
CSSDDocument::AssertValid() const
{
	CDocument::AssertValid();
}

void
CSSDDocument::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

