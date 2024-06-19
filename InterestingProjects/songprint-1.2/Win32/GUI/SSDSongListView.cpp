// SSDSongListView.cpp : implementation of the CSSDSongListView class
//
// Implementation of the CSSDSongListView class, a CListView derived class that
// handles displaying the list of songs in the document

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
#include "SSDSongListView.h"
#include "SSDMainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSDSongListView

IMPLEMENT_DYNCREATE(CSSDSongListView, CListView)

BEGIN_MESSAGE_MAP(CSSDSongListView, CListView)
	//{{AFX_MSG_MAP(CSSDSongListView)
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, OnGetdispinfo)
	ON_MESSAGE(WM_UPDATELISTCOUNTS, OnUpdateListCount)
	ON_NOTIFY_REFLECT(LVN_ITEMCHANGED, OnItemchanged)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(LVN_KEYDOWN, OnKeydown)
	ON_WM_LBUTTONDOWN()
	//}}AFX_MSG_MAP
	// Standard printing commands
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDSongListView construction/destruction

CSSDSongListView::CSSDSongListView()
{
}

CSSDSongListView::~CSSDSongListView()
{
}

/**
 * PreCreateWindow
 *
 * @cs: Reference to CREATESTRUCT for list view data
 *
 * Set the list view styles.
 *
 * Return values:
 *  TRUE/FALSE from base class PreCreateWindow
 * Exceptions:
 *  None
 */
BOOL
CSSDSongListView::PreCreateWindow(CREATESTRUCT& cs)
{
    cs.style|= LVS_REPORT | LVS_SHOWSELALWAYS | LVS_OWNERDATA;
	return CListView::PreCreateWindow(cs);
}


/////////////////////////////////////////////////////////////////////////////
// CSSDSongListView drawing

/**
 * OnInitialUpdate
 *
 * Called by MFC on first update of view when it is created. This routine
 * create the list view columns, sets up a default help message if the 
 * document is empty, and gives the document a shortcut pointer to itself.
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongListView::OnInitialUpdate()
{

    // Call base class

	CListView::OnInitialUpdate();

    // Add Columns if not present (header control count is 0 or -1)

    if (GetListCtrl().GetHeaderCtrl()->GetItemCount()<=0)
    {
        RECT rc;
        GetClientRect(&rc);

        CString Temp="Song Path/Filename";
        GetListCtrl().InsertColumn(0, (LPCTSTR)Temp, LVCFMT_LEFT, rc.right-200, -1);
        Temp="Signature Status";
        GetListCtrl().InsertColumn(1, (LPCTSTR)Temp, LVCFMT_LEFT, 200, -1);
    }

    // Get document

    CSSDDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Initialize list view item count, if the document is empty create the
    // help message, otherwise the list view item count is the document item
    // count

    if (pDoc->m_SongList.GetSize()==0)
    {
        // Setup Initial Message

        m_InitialMessage.RemoveAll();
        m_InitialMessage.Add("Added songs appear here. To add songs:");
        m_InitialMessage.Add(" * Double click this window");
        m_InitialMessage.Add(" * Click the Add songs or Add directory buttons on the toolbar");
        m_InitialMessage.Add(" * Choose Add songs or Add directory in the File drop menu");

	    GetListCtrl().SetItemCountEx(m_InitialMessage.GetSize());
        m_bDisplayInitMessage=TRUE;
    }
	else
    {
        m_bDisplayInitMessage=FALSE;
        GetListCtrl().SetItemCountEx(pDoc->m_SongList.GetSize());
    }
	
    // Give Document Pointer to ListCtrl other views/threads

    pDoc->m_pSongListCtrl=&(this->GetListCtrl());

}


/**
 * OnGetdispinfo
 *
 * pNMHDR: Pointer to the WIN32 NMHDR structure which is a list view
 *  LV_DISPINFO structure in this case, which contains information on the
 *  item that data is being requested for.
 * pResult: Pointer to event result flag. Set to -1 to abort event.
 *
 * Virtual list views call this event to get data to display for a particular
 * item.
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void CSSDSongListView::OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // Get index from pNMHDR

	LV_DISPINFO* pDispInfo = (LV_DISPINFO*)pNMHDR;
    LV_ITEM* pItem= &(pDispInfo)->item;
    int iItemIndx= pItem->iItem;

    SongDataStruct* pSongData;

    // Get the document

    CSSDDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Process only text

    if (pItem->mask & LVIF_TEXT)
    {
        // If we are display help message the display required line

        if ((pDoc->m_SongList.GetSize()==0) && (m_bDisplayInitMessage))
        {
            if (pItem->iSubItem==0)
            {
                CString Temp=m_InitialMessage.GetAt(iItemIndx);
                lstrcpy(pItem->pszText, Temp);
            }
        }
        else
        {
            // Get song information and fill request

            CSingleLock SongMutexLock(&pDoc->m_SongListMutex);
            SongMutexLock.Lock();
            if (iItemIndx < pDoc->m_SongList.GetSize())
            {
                pSongData=(SongDataStruct*)pDoc->m_SongList.GetAt(iItemIndx);
                switch(pItem->iSubItem)
                {
                    case 0: // File name
                        lstrcpy(pItem->pszText, pSongData->Path);
                        break;

                    case 1: // Signature Status
                        if (pSongData->Status==SONG_NEW)
                            pItem->pszText="Song not processed yet";
                        else if (pSongData->Status==SONG_SIG_ERROR)
                            pItem->pszText="Error generating signature";
                        else if ((pSongData->Status==SONG_SERVER_ERROR) ||
								(pSongData->Status==SONG_SERVER_EXTENDED_ERROR))
                            pItem->pszText="Server error occurred";
                        else if (pSongData->Status==SONG_IDED)
                            pItem->pszText="Song IDed";
                        else if (pSongData->Status==SONG_UNKNOWN)
                            pItem->pszText="Unknown song";
                        else pItem->pszText="Unknown status";
                        break;
                }
                SongMutexLock.Unlock();
            }
        }
    }
	
	*pResult = 0;
}

/////////////////////////////////////////////////////////////////////////////
// CSSDSongListView diagnostics

#ifdef _DEBUG
void
CSSDSongListView::AssertValid() const
{
	CListView::AssertValid();
}

void
CSSDSongListView::Dump(CDumpContext& dc) const
{
	CListView::Dump(dc);
}

CSSDDocument* CSSDSongListView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSSDDocument)));
	return (CSSDDocument*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CSSDSongListView message handlers

/**
 * OnUpdateListCount
 *
 * Called to notify view that the document item count has changed so the
 * list view item count can be updated. If the help message is being
 * displayed then stop it.
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongListView::OnUpdateListCount(WPARAM wParam, LPARAM lParam)
{
    // Get document

    CSSDDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Get mutex

    CSingleLock SongMutexLock(&pDoc->m_SongListMutex);
    SongMutexLock.Lock();

    // Stop displaying initial message

    m_bDisplayInitMessage=FALSE;

    // Update without scrolling and only redraw what is needed to prevent flickering

    GetListCtrl().SetItemCountEx(pDoc->m_SongList.GetSize(), LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL);

    SongMutexLock.Unlock();
   
}


/**
 * OnItemchanged
 *
 * pNMHDR: Pointer to the WIN32 NMHDR structure which is a list view
 *  LV_DISPINFO structure in this case which contains list view item data
 * pResult: Pointer to event result flag. Set to -1 to abort event.
 *
 * Called when an item's status has been changed. If the item has been
 * highlighted and the document is not empty, the data view is notified to
 * display the song data for this item. If the document is empty than 
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongListView::OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    
    // Get document

    CSSDDocument* pDoc = GetDocument();
    ASSERT_VALID(pDoc);

    // Get mutex

    CSingleLock SongMutexLock(&pDoc->m_SongListMutex);
    SongMutexLock.Lock();
    int nItemCount=pDoc->m_SongList.GetSize();
    SongMutexLock.Unlock();

    // If item is selected and songs are listed, then send song index to data view

    if ((pNMListView->uChanged==LVIF_STATE) && 
        ((pNMListView->uNewState & LVIS_SELECTED)>0) &&
        (nItemCount>0))
        AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_UPDATESONGDATA, 0, (LPARAM)pNMListView->iItem);

	*pResult = 0;
}


/**
 * OnDlbclck
 *
 * pNMHDR: Pointer to the WIN32 NMHDR structure which is a list view
 *  LV_DISPINFO structure in this case which contains list view item data
 * pResult: Pointer to event result flag. Set to -1 to abort event.
 *
 * Called when a list view has been double clicked. This is a shortcut for
 * adding songs. Pass a message to the main frame to add songs.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongListView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// Do an Add songs command

    AfxGetApp()->m_pMainWnd->SendMessage(WM_COMMAND, ID_ADDSONGS, 0);
	
	*pResult = 0;
}


/**
 * OnKeydown
 *
 * pNMHDR: Pointer to the WIN32 NMHDR structure which contains keystroke
 *  data in this case.
 * pResult: Pointer to event result flag. Set to -1 to abort event.
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongListView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
    // Get key

	LV_KEYDOWN* pLVKeyDow = (LV_KEYDOWN*)pNMHDR;

    // If delete, then remove selected items

    if ((pLVKeyDow->wVKey==VK_DELETE) && (!m_bDisplayInitMessage))
    {
        // Get document

        CSSDDocument* pDoc = GetDocument();
    	ASSERT_VALID(pDoc);

        SongDataStruct* pSongData;
        int nCurrentIndex;

        CSingleLock SongMutexLock(&pDoc->m_SongListMutex);
        SongMutexLock.Lock();

        // loop through all selected items and delete them

        POSITION Pos = pDoc->m_pSongListCtrl->GetFirstSelectedItemPosition();
        while (Pos)
        {
            nCurrentIndex=pDoc->m_pSongListCtrl->GetNextSelectedItem(Pos);
            pSongData=(SongDataStruct*)pDoc->m_SongList.GetAt(nCurrentIndex);
            delete pSongData;
            pDoc->m_SongList.RemoveAt(nCurrentIndex);
        }

        // Update list view item count

        GetListCtrl().SetItemCountEx(pDoc->m_SongList.GetSize(), LVSICF_NOSCROLL);
        
        // Get caret item and update data view to it, otherwise display blank

        Pos = pDoc->m_pSongListCtrl->GetFirstSelectedItemPosition();
        if (Pos)
            nCurrentIndex=pDoc->m_pSongListCtrl->GetNextSelectedItem(Pos);
        else
            nCurrentIndex=-1;

        // Update data view

        AfxGetApp()->m_pMainWnd->SendMessageToDescendants(WM_UPDATESONGDATA, 0, (LPARAM)nCurrentIndex);

        SongMutexLock.Unlock();
    }
	
	*pResult = 0;
}


/**
 * OnLButtonDown
 *
 * @nFlags: Button flags
 * @point: CPoint class containing the mouse position
 *
 * Called on a left mouse button click. This routine traps selection of items
 * when the help message is being displayed.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongListView::OnLButtonDown(UINT nFlags, CPoint point) 
{
    if (!m_bDisplayInitMessage)
 	    CListView::OnLButtonDown(nFlags, point);
}
