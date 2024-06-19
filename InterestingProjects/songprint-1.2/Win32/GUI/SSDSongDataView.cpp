// SSD Song Data View.cpp
//
// Implementation of the CSSDSongDataView class, a CScrollView derived class
// that handles displaying song information

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
#include "SSDSongDataView.h"
#include "SSDDocument.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

const PRINTSONGSWARNING  = 3000;
const PRINTPAGESWARNING = 50;
const MAX_PATH_LENGTH = 255;

/////////////////////////////////////////////////////////////////////////////
// CSSDSongDataView

IMPLEMENT_DYNCREATE(CSSDSongDataView, CScrollView)

CSSDSongDataView::CSSDSongDataView()
{
}

CSSDSongDataView::~CSSDSongDataView()
{
}


BEGIN_MESSAGE_MAP(CSSDSongDataView, CScrollView)
	//{{AFX_MSG_MAP(CSSDSongDataView)
	ON_MESSAGE(WM_UPDATESONGDATA, OnUpdateSongData)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
	ON_COMMAND(ID_FILE_PRINT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CScrollView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDSongDataView drawing

/**
 * OnInitialUpdate
 *
 * Called by MFC on first update of view when it is created. This routine
 * sets up a default help message if the document is empty, then it will
 * initialize the CScrollView scroll bars and position
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongDataView::OnInitialUpdate()
{
    SIZE Size;

	// No URL, yet

	m_strURL="";

    // Call base class

	CScrollView::OnInitialUpdate();

	// Get document

	CSSDDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // If no items then setup default message

    CSingleLock SongMutexLock(&pDoc->m_SongListMutex);
    SongMutexLock.Lock();
    if (pDoc->m_SongList.GetSize()==0)
    {

        // Set default/empty message

        m_SongDisplayData.RemoveAll();
        CString Temp="Song information received from a song signature appears in this window";
        m_SongDisplayData.Add(Temp);

        // Caclulate/reset scroll sizes
    
        CClientDC ClientDC(this);
        Size=ClientDC.GetTextExtent(Temp, Temp.GetLength());
        Size.cx+=MARGIN*2;
        Size.cy+=MARGIN;
    }
    else 
        Size.cx=Size.cy=0;
    SongMutexLock.Unlock();

    // Setup scroll bar sizes and scroll position

	SetScrollSizes(MM_TEXT, Size);
    ScrollToPosition(CPoint(0,0));
}

/**
 * OnDraw
 *
 * Draw the contents of the client window that are visible
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongDataView::OnDraw(CDC* pDC)
{
	// Get document

	CSSDDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    // Get text metrics and line height

    TEXTMETRIC TM;
    pDC->GetTextMetrics(&TM);
    int nLineHeight = TM.tmHeight+TM.tmExternalLeading;

    // Get display region

    RECT ClipRect;
    pDC->GetClipBox(&ClipRect);

    // Draw all visible lines in the region

    int Y = MARGIN;
    for (int Line=0; Line < m_SongDisplayData.GetSize(); Line++)
    {
        if (Y+nLineHeight>=ClipRect.top && Y<=ClipRect.bottom)
            pDC->TextOut(MARGIN, Y, m_SongDisplayData.GetAt(Line));
        Y+=nLineHeight;
    }
}

/////////////////////////////////////////////////////////////////////////////
// CSSDSongDataView diagnostics

#ifdef _DEBUG
void
CSSDSongDataView::AssertValid() const
{
	CScrollView::AssertValid();
}

void
CSSDSongDataView::Dump(CDumpContext& dc) const
{
	CScrollView::Dump(dc);
}

#endif //_DEBUG

/**
 * GetDocument
 *
 * Get the current document pointer. Since this view was not created as part
 * of the document template this function must always be present, the inline
 * debug version cannot be used.
 *
 * Return values:
 *  Pointer to the active document
 * Exceptions:
 *  ASSERT if document is not corrent class
 */
CSSDDocument*
CSSDSongDataView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CSSDDocument)));
	return (CSSDDocument*)m_pDocument;
}


/////////////////////////////////////////////////////////////////////////////
// CSSDSongDataView printing

/**
 * OnPreparePrinting
 *
 * @pInfo: Pointer to CPrintInfo class that contains information about the
 *  current print job.
 *
 * First phase of the MFC printing process. If items are highlights then
 * enable the "print selection" flag in the print dialog window. Initialize
 * minimum pages and print preview page display. After calling the required
 * DoPreparePrinting routine, the user is prompted if they wish to proceed 
 * for a large number of songs
 *
 * Return values:
 *  TRUE/FALSE on if printing is to proceed
 * Exceptions:
 *  ASSERT if document is not valid
 */
BOOL
CSSDSongDataView::OnPreparePrinting(CPrintInfo* pInfo) 
{
	// Get document

    CSSDDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    CSingleLock SongMutexLock(&pDoc->m_SongListMutex);

    // Open the selected option if items highlighted in list

    if (pDoc->m_pSongListCtrl->GetSelectedCount()>0)
    {
        pInfo->m_pPD->m_pd.Flags&=~PD_NOSELECTION;
        pInfo->m_pPD->m_pd.Flags|=PD_SELECTION;
    }

    // Only 1 page display in print preview, start at page one

    pInfo->m_nNumPreviewPages=1;    
    pInfo->SetMinPage(1);

    // Do required DoPreparePrinting
    
    int nPrepResult=DoPreparePrinting(pInfo);
    if (nPrepResult)
    {
        // Warn user if they have more than PRINTSONGSWARNING songs

        SongMutexLock.Lock();
        int nResult=IDYES;
        if (pDoc->m_SongList.GetSize()>PRINTSONGSWARNING)
        {
            nResult=MessageBox("This could take several minutes. Do you wish to continue?", 
                "Warning", MB_YESNO | MB_ICONWARNING);
        }
        SongMutexLock.Unlock();

        if (nResult==IDNO)
            return FALSE;
    }
    
    return nPrepResult;

}


/**
 * OnBeginPrinting
 *
 * @pDC: Pointer to the CDC of the printer(print) or screen(print preview)
 *
 * @pInfo: Pointer to CPrintInfo class that contains information about the
 *  current print job.
 *
 * Second phase of the MFC printing process. This routine assembles the print
 * data by assembling all the lines to print, then calling the FormatForPrint
 * and Paginate routines to adjust the lines.
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongDataView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
    // Get document

    CSSDDocument* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

    CString Temp;

    // Get line height

    TEXTMETRIC TM;
    pDC->GetTextMetrics(&TM);
    m_nLineHeight=TM.tmHeight+TM.tmExternalLeading;

    // Get visible region

	RECT ClipRect;
	pDC->GetClipBox(&ClipRect);

    // Init vars, get song list mutex, clear any old print data

    SongDataStruct* pSongData = NULL;
    m_SongPrintData.RemoveAll();
    CSingleLock SongMutexLock(&pDoc->m_SongListMutex);

    // Display wait message

    m_PrintWaitDialog.Create(IDD_WAITDIALOG);
    m_PrintWaitDialog.CenterWindow();
    m_PrintWaitDialog.SetDlgItemText(IDC_WAITMSG, "Creating Job...0%");
    m_PrintWaitDialog.SetRedraw(TRUE);
    m_PrintWaitDialog.RedrawWindow();

    int nTotalItems;
    int nLastPercentage=0;
    int nItemCount=0;
    float nNewPercentage;

    SongMutexLock.Lock();

    // Must be songs in the document to add them, then add either all songs
    // or the selection songs to the song print data array. Update the wait
    // dialog as the process proceeds.

    if (pDoc->m_SongList.GetSize()>0)
    {
        if (pInfo->m_pPD->PrintAll())
        {
            // Add all Songs

            nTotalItems=pDoc->m_SongList.GetSize();
            for (; nItemCount< nTotalItems; nItemCount++)
            {
                pSongData=(SongDataStruct*)pDoc->m_SongList.GetAt(nItemCount);
                FormatSongData(&m_SongPrintData, pSongData, TRUE);
                m_SongPrintData.Add("");

                // Update wait dialog

                nNewPercentage=((float)(nItemCount+1)*100/nTotalItems);
                if ((int)nNewPercentage!=nLastPercentage)
                {
                    nLastPercentage=(int)nNewPercentage;
                    Temp.Format("Creating Job...%d%%", nLastPercentage);
                    m_PrintWaitDialog.SetDlgItemText(IDC_WAITMSG, Temp);
                    m_PrintWaitDialog.SetRedraw(TRUE);
                    m_PrintWaitDialog.BringWindowToTop();
                    m_PrintWaitDialog.RedrawWindow();
                }
            }
        }
        else if (pInfo->m_pPD->PrintSelection())
        {

            // Add selected songs

            POSITION Pos = pDoc->m_pSongListCtrl->GetFirstSelectedItemPosition();
            int nTotalItems=pDoc->m_pSongListCtrl->GetSelectedCount();
            while (Pos)
            {
                nItemCount++;
                pSongData=(SongDataStruct*)pDoc->m_SongList.GetAt(pDoc->m_pSongListCtrl->GetNextSelectedItem(Pos));
                FormatSongData(&m_SongPrintData, pSongData, TRUE);
                m_SongPrintData.Add("");

                // Update wait dialog/percentage

                nNewPercentage=((float)nItemCount*100/nTotalItems);
                if ((int)nNewPercentage!=nLastPercentage)
                {
                    nLastPercentage=(int)nNewPercentage;
                    Temp.Format("Creating Job...%d%%", nLastPercentage);
                    m_PrintWaitDialog.SetDlgItemText(IDC_WAITMSG, Temp);
                    m_PrintWaitDialog.SetRedraw(TRUE);
                    m_PrintWaitDialog.BringWindowToTop();
                    m_PrintWaitDialog.RedrawWindow();
                }
            }
        }

        // Call routine for formatting and paginating

        if (m_SongPrintData.GetSize()>0)
        {
            FormatForPrint(pDC, pInfo);
            Paginate(pDC, pInfo);
        }
    }
    SongMutexLock.Unlock();

    // Kill the wait dialog

    m_PrintWaitDialog.DestroyWindow();

    // Warn user if PRINTPAGESWARNING exceeded that it take a large number
    // of pages to print the job

    if ((pInfo->GetMaxPage()>PRINTPAGESWARNING) && (!pInfo->m_bPreview))
    {
        Temp.Format("This document is %d pages long. Do you wish to continue?", pInfo->GetMaxPage());
        int nResult=MessageBox(Temp, "Warning", MB_YESNO | MB_ICONWARNING);

        if (nResult==IDNO)
        {
            pDC->AbortDoc();
            pInfo->SetMaxPage(0);
        }
    }

    
}


/**
 * FormatForPrint
 *
 * @pDC: Pointer to the CDC of the printer(print) or screen(print preview)
 *
 * @pInfo: Pointer to CPrintInfo class that contains information about the
 *  current print job.
 *
 * Check all lines of the print data to ensure that each line will fit on the
 * printed page. If the line is too long it is shorted on the word and the
 * remainder moved to a new line below the current line. If the line cannot
 * be broken between words, then a word itself is broken into two lines. 
 * This routine assumes m_PrintWaitDialog is initialized.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongDataView::FormatForPrint(CDC* pDC, CPrintInfo* pInfo) 
{

    // Used for wait dialog and percentage counting

    CString Temp;
    int nLastPercentage=0;
    float nNewPercentage;

    // Used for line formatting

    CString Line;
    CString RemovedText;
    int nBreakPosition;
    int nPageWidth = pDC->GetDeviceCaps(HORZRES);

    // Update wait dialog for new task

    m_PrintWaitDialog.SetDlgItemText(IDC_WAITMSG, "Print Formatting...0%");
    m_PrintWaitDialog.SetRedraw(TRUE);
    m_PrintWaitDialog.RedrawWindow();

    // Process all lines of the print document

    int nTotalItems=m_SongPrintData.GetSize();
    for (int nItemCount=0; nItemCount<nTotalItems; nItemCount++)
    {
        Line=m_SongPrintData.GetAt(nItemCount);
        RemovedText="";

        // Break apart string if too long

        if (pDC->GetTextExtent(Line).cx + MARGIN*2 > nPageWidth)
        {
            // Repeat until text will fit on paper size

            while (pDC->GetTextExtent(Line).cx + MARGIN*2 > nPageWidth)
            {
                // Look for spaces to break apart words and do word wrapping.
                // If no spaces can be found then the text of the word 
                // itself must be broken apart.

                nBreakPosition=Line.ReverseFind(' ');
                if (nBreakPosition!=-1)
                {
                    RemovedText=Line.Mid(nBreakPosition)+RemovedText;
                    Line.Delete(nBreakPosition, Line.GetLength()-nBreakPosition);
                }
                else
                {
                    RemovedText=Line.Mid(Line.GetLength()-1)+RemovedText;
                    Line.Delete(Line.GetLength()-1);
                }
            }

            // Place Line back into array and insert removed text on new line

            m_SongPrintData.SetAt(nItemCount, Line);
            m_SongPrintData.InsertAt(nItemCount+1, RemovedText);

        }

        // Update wait dialog/percentage

        nNewPercentage=((float)nItemCount*100/nTotalItems);
        if ((int)nNewPercentage!=nLastPercentage)
        {
            nLastPercentage=(int)nNewPercentage;
            Temp.Format("Print Formatting...%d%%", nLastPercentage);
            m_PrintWaitDialog.SetDlgItemText(IDC_WAITMSG, Temp);
            m_PrintWaitDialog.SetRedraw(TRUE);
            m_PrintWaitDialog.BringWindowToTop();
            m_PrintWaitDialog.RedrawWindow();
        }
   }
}


/**
 * Paginate
 *
 * @pDC: Pointer to the CDC of the printer(print) or screen(print preview)
 *
 * @pInfo: Pointer to CPrintInfo class that contains information about the
 *  current print job.
 *
 * Creates an array of line indexes that define when line ends a printed
 * page. This data is used by OnPrint to determine which data to display for
 * a given page number. pInfo is then updated with the now calculated total
 * number of pages to print. Page breaks are set on blank lines.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongDataView::Paginate(CDC* pDC, CPrintInfo* pInfo)
{
    // Used for wait dialog and percentage counting

    int nLastPercentage=0;
    float nNewPercentage;
    CString Temp;

    // Used for paginating

    int nPageCount=1;
    int nNextBreakIndex=1;
    int nCurrentPageHeight=0;
    int nPaperHeight = pDC->GetDeviceCaps(VERTRES);

    // Clear the pagination array, index 0 starts page 1

    m_PaginationData.RemoveAll();
    m_PaginationData.Add(0);

    // Update wait dialog for new task

    m_PrintWaitDialog.SetDlgItemText(IDC_WAITMSG, "Paginating...0%");
    m_PrintWaitDialog.SetRedraw(TRUE);
    m_PrintWaitDialog.RedrawWindow();

    // Increment through print document

    int nItemCount=0;
    int nTotalItems=m_SongPrintData.GetSize();
    while (nItemCount < nTotalItems)
    {
        
        // Find the next section break

        while ((nNextBreakIndex<m_SongPrintData.GetSize()-1) &&
			(m_SongPrintData.GetAt(nNextBreakIndex)!="") )
            nNextBreakIndex++;

        // if the height of the section is too large then go next page

        nCurrentPageHeight+=m_nLineHeight*(nNextBreakIndex-nItemCount+1);
        if (nCurrentPageHeight>(nPaperHeight+MARGIN*2))
        {
            nPageCount++;
            m_PaginationData.Add(nItemCount);
            nCurrentPageHeight=m_nLineHeight*(nNextBreakIndex-nItemCount+1);
        }

        // Go on...

        nNextBreakIndex++;
        nItemCount=nNextBreakIndex;

        // Update wait dialog/percentage

        nNewPercentage=((float)nItemCount*100/nTotalItems);
        if ((int)nNewPercentage!=nLastPercentage)
        {
            nLastPercentage=(int)nNewPercentage;
            Temp.Format("Paginating...%d%%", nLastPercentage);
            m_PrintWaitDialog.SetDlgItemText(IDC_WAITMSG, Temp);
            m_PrintWaitDialog.SetRedraw(TRUE);
            m_PrintWaitDialog.BringWindowToTop();
            m_PrintWaitDialog.RedrawWindow();
        }
    }

    // Add last page, and Set max page number

    m_PaginationData.Add(nItemCount);
    pInfo->SetMaxPage(nPageCount);
}


/**
 * OnPrint
 *
 * @pDC: Pointer to the CDC of the printer(print) or screen(print preview)
 *
 * @pInfo: Pointer to CPrintInfo class that contains information about the
 *  current print job.
 *
 * 4th phase of the MFC print process. Called by MFC to display a specific
 * page set in pInfo's m_nCurPage variable, this routine Displays the lines
 * for the page using the pagination array.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongDataView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
    // Make sure page number is valid, aborted jobs can have page number 0

    if ((m_PaginationData.GetSize()>1) && (pInfo->m_nCurPage>0))
    {
        int StartIndex=m_PaginationData.GetAt(pInfo->m_nCurPage-1);
        int EndIndex=m_PaginationData.GetAt(pInfo->m_nCurPage);
        int YPos=MARGIN;

        // Display lines

        for (int nCount=StartIndex; nCount < EndIndex ; nCount++)
        {
            pDC->TextOut(MARGIN, YPos, m_SongPrintData.GetAt(nCount));
            YPos+=m_nLineHeight;
        }
    }
}


/**
 * OnEndPrinting
 *
 * @pDC: Pointer to the CDC of the printer(print) or screen(print preview)
 *
 * @pInfo: Pointer to CPrintInfo class that contains information about the
 *  current print job.
 *
 * 5th and final phase of the MFC print process. Called by MFC when all pages
 * have been printed, or the print preview display is closed. Print job
 * cleanup is done here.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongDataView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo) 
{
	
    // Call base class for standard cleanup

	CScrollView::OnEndPrinting(pDC, pInfo);

    // Clear print data and pagination arrays

    m_PaginationData.RemoveAll();
    m_SongPrintData.RemoveAll();
}


////////////////////////////////////////////////////////////////////////////
// CSSDSongDataView message handlers

/**
 * OnUpdateSongData
 *
 * @wParam: Windows Message WPARAM variable, not used
 * @lParam: Index of item to display song data
 *
 * Called to update the view with a the data of a particular song. if -1 is
 * used in lParam the view is cleared.
 *
 * Return values:
 *  None
 * Exceptions:
 *  ASSERT if document is not valid
 */
void
CSSDSongDataView::OnUpdateSongData(WPARAM wParam, LPARAM lParam)
{
    // Clear display data
        
    m_SongDisplayData.RemoveAll();

    SIZE Size;

    // if index -1 then view size is 0

    if (lParam==-1)
        Size.cx=Size.cy=0;
    else
    {

        // Get document

        CSSDDocument* pDoc = GetDocument();
	    ASSERT_VALID(pDoc);

        // Get song data

        SongDataStruct* pSongData = NULL;
        CSingleLock SongMutexLock(&pDoc->m_SongListMutex);
        SongMutexLock.Lock();
        pSongData=(SongDataStruct*)pDoc->m_SongList.GetAt(lParam);
        SongMutexLock.Unlock();

        // Assembly Data for Display

        FormatSongData(&m_SongDisplayData, pSongData, FALSE);

		// Check for URL

		m_strURL=pSongData->URL;

        // Vars to Calculate scroll sizes

        CClientDC ClientDC(this);
        int LineWidth=0;
        TEXTMETRIC TM;
        CString Temp;
        ClientDC.GetTextMetrics(&TM);

        // Widest line determines scroll width

        for (int Line=0; Line<m_SongDisplayData.GetSize(); ++Line)
        {
            Temp=m_SongDisplayData.GetAt(Line);
            Size=ClientDC.GetTextExtent(Temp, Temp.GetLength());
            if (Size.cx > LineWidth)
                LineWidth= Size.cx;
        }
    
        Size.cx = LineWidth+MARGIN;

        // Scroll height set by number of lines

        Size.cy = (TM.tmHeight + TM.tmExternalLeading) * (m_SongDisplayData.GetSize()+1)+MARGIN;
    }

    // Mark for redraw

    Invalidate();

    // Set scroll sizes and reset scroll position

    SetScrollSizes(MM_TEXT, Size);
    ScrollToPosition(CPoint(0,0));
    
    return;
}


/**
 * OnLButtonDblClk
 *
 * @nFlags: Flag of various virtual keys
 * @CPoint: Mouse position
 *
 * Called to when user double clicks on the Song Data View. If there is a
 * unknown song display, then create a temporary URL shortcut,launch it,
 * and delete the shortcut.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void CSSDSongDataView::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if (m_strURL.GetLength())
	{

        
        // Get Temporary Directory

        CString strTempPath;
        GetTempPath(MAX_PATH_LENGTH, strTempPath.GetBuffer(MAX_PATH_LENGTH));
        strTempPath.ReleaseBuffer();
        if (strTempPath.Right(1)!='\\') strTempPath+='\\';

        // Create Temporary Shortcut file

        CFile URLFile;
        if (URLFile.Open(strTempPath+"URLTemp.url", CFile::modeCreate | CFile::modeWrite , NULL))
        {
            CString strTemp="[InternetShortcut]\r\n";
            URLFile.Write(strTemp, strTemp.GetLength());
            strTemp="URL=";
            URLFile.Write(strTemp, strTemp.GetLength());
            URLFile.Write(m_strURL, m_strURL.GetLength());
            strTemp="\r\n";
            URLFile.Write(strTemp, strTemp.GetLength());
            URLFile.Close();

		    // Launch URL

    		ShellExecute(m_hWnd, "open", strTempPath+"URLTemp.url", NULL, NULL, SW_SHOWNORMAL);
    
        }
	}
}

/////////////////////////////////////////////////////////////////////////////
// CSSDSongDataView routines

/**
 * FormatSongData
 *
 * @pDestArray: Desination array to fill with formatted data
 * @pSongData: Song data to assemble formatted data from
 * @bPrinting: Set to TRUE if this is being formatted for printing. This
 *	effects display for the URL message.
 *
 * Take songs data and formats it into lines suitable for display or print.
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongDataView::FormatSongData(CStringArray* pDestArray, SongDataStruct* pSongData, bool bPrinting)
{
    CString Temp;
    int nCount;

    // Song info line

    Temp.Format("Song Information for %s", pSongData->Path);
    pDestArray->Add(Temp);
    pDestArray->Add(" ");   // A space prevents a page break here, see 
                            // CSSDSongDataView::Paginate

    switch (pSongData->Status)
    {
        // if Song not IDed, then display notice

        case SONG_NEW:

            pDestArray->Add("This song is pending for indentification");
            break;

        // if Song IDed, then show data

        case SONG_IDED:

            // Title line

            pDestArray->Add("Title:");
            if (pSongData->Title.GetLength()==0)
                pDestArray->Add(" Unknown");
            else pDestArray->Add(" "+pSongData->Title);
            pDestArray->Add("");

            // Artist lines

            if (pSongData->Artists.GetSize()>1)
                pDestArray->Add("Artists:");
            else    
                pDestArray->Add("Artist:");
            if (pSongData->Artists.GetSize()==0)
                pDestArray->Add(" Unknown");
            else for (nCount=0; nCount<pSongData->Artists.GetSize(); nCount++)
                pDestArray->Add(" "+pSongData->Artists.GetAt(nCount));
            pDestArray->Add("");

            // Album lines

            if (pSongData->Albums.GetSize()>1)
                pDestArray->Add("Albums:");
            else
                pDestArray->Add("Album:");
            if (pSongData->Albums.GetSize()==0)
                pDestArray->Add(" Unknown");
            else for (nCount=0; nCount<pSongData->Albums.GetSize(); nCount++)
                pDestArray->Add(" "+pSongData->Albums.GetAt(nCount));
            pDestArray->Add("");

            // Genre lines

            if (pSongData->Genres.GetSize()>1)
                pDestArray->Add("Genres:");
            else
                pDestArray->Add("Genre:");
            if (pSongData->Genres.GetSize()==0)
                pDestArray->Add(" Unknown");
            else for (nCount=0; nCount<pSongData->Genres.GetSize(); nCount++)
                pDestArray->Add(" "+pSongData->Genres.GetAt(nCount));
            pDestArray->Add("");

            // URL Line

            if (bPrinting)
				pDestArray->Add("For more information, use the following URL:");
			else
				pDestArray->Add("For more information, use the following URL (Double click this window to launch URL now):");
            pDestArray->Add(" "+pSongData->URL);
            break;

        // If song is unknown then display URL

        case SONG_UNKNOWN:
            if (bPrinting)
				pDestArray->Add("This song is not in the database, to add it use the following URL:");
			else
				pDestArray->Add("This song is not in the database, to add it use the following URL (Double click this window to add now):");
            pDestArray->Add(" "+pSongData->URL);
            break;

        // Error Messages 

		case SONG_SIG_ERROR:
            pDestArray->Add("An error has occurred during the signature creation process");
			break;

		case SONG_SERVER_ERROR:
            pDestArray->Add("A server error occurred while attempting get information for this song");
			break;

		case SONG_SERVER_EXTENDED_ERROR:
            pDestArray->Add("A server error occurred while attempting get information for this song");
			pDestArray->Add(pSongData->URL);
			break;
default:

            pDestArray->Add("An unknown error has occurred during the ID process for this song");

    }
}


