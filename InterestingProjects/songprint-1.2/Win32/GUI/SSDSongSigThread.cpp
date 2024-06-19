// SSDSongSigThread.cpp
//
// Implementation of the CSSDSongSigThread class, a CWinThread derived class
// that handles the creating of signatures for songs and obtaining song 
// information from the eTantrum database server

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
#include "SSDSongSigThread.h"
#include "SSDMainFrame.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/* These macros remove some redundancy for assigning function pointers */

#define _CODECS_ADD_FUNC(vtable, fn, codec) ( vtable->fn = codec ## _ ## fn )
#define _CODECS_ADD_FUNCTIONS(vtable, codec) \
{ \
    _CODECS_ADD_FUNC(vtable, open, codec); \
    _CODECS_ADD_FUNC(vtable, get_format, codec); \
    _CODECS_ADD_FUNC(vtable, get_song_info, codec); \
    _CODECS_ADD_FUNC(vtable, get_pcm_data, codec); \
    _CODECS_ADD_FUNC(vtable, close, codec); \
}

/////////////////////////////////////////////////////////////////////////////
// CSSDSongSigThread

IMPLEMENT_DYNCREATE(CSSDSongSigThread, CWinThread)

CSSDSongSigThread::CSSDSongSigThread()
{
	m_bTerminateThread=FALSE;
	m_bAutoDelete=FALSE;
}

CSSDSongSigThread::~CSSDSongSigThread()
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
CSSDSongSigThread::InitInstance()
{
	return TRUE;
}


/**
 * ExitInstance
 *
 * Cleanup any song data and call base class for exit code
 * 
 * Return values:
 *  Exit code
 * Exceptions:
 *  None
 */
int
CSSDSongSigThread::ExitInstance()
{
    m_SongData.Albums.RemoveAll();
    m_SongData.Artists.RemoveAll();
    m_SongData.Genres.RemoveAll();

	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(CSSDSongSigThread, CWinThread)
	//{{AFX_MSG_MAP(CSSDSongSigThread)
	ON_THREAD_MESSAGE(WM_SONGSIGTHREAD_NEWSONGS, OnThreadNewSongCount)
	ON_THREAD_MESSAGE(WM_THREAD_DIE, OnThreadDie)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDSongSigThread message handlers

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
CSSDSongSigThread::OnThreadDie()
{
	::PostQuitMessage(0);
}


/**
 * OnThreadNewSongCount
 *
 * @wParam: Windows Message WPARAM variable, not used
 * @lParam: Pointer to the document that has new songs
 *
 * Called whenever songs have been added to loop through all songs in the 
 * document and attempt to indentify them by calling IndentifySong
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongSigThread::OnThreadNewSongCount(WPARAM wParam, LPARAM lParam)
{
    // Leave if terminated

    if (m_bTerminateThread) return;

    // Get document pointer

    m_pDoc=(CSSDDocument*)lParam;

    // Init count vars
	
    int nSongIndex=0;
    int nTotalSongs;

    // Song data pointers

    SongDataStruct* pSongData =NULL;
    SongDataStruct* pSongDataVerify =NULL;

    // Create mutex lock

    CSingleLock SongMutexLock(&m_pDoc->m_SongListMutex);

    // Get the current song count

    SongMutexLock.Lock();
    nTotalSongs=m_pDoc->m_SongList.GetSize();
    SongMutexLock.Unlock();
    
    // Increment through songs in document

    while(nSongIndex < nTotalSongs)
    {
        // Clear Song Data Arrays/Strings

        m_SongData.Title="";
        m_SongData.URL="";
        m_SongData.Albums.RemoveAll();
        m_SongData.Artists.RemoveAll();
        m_SongData.Genres.RemoveAll();
        
        // Copy song data to work variable. This is necessary because we do not want to
        // hold mutex during the possibly time consuming signature process and there is
        // a risk the song could be deleted or have it's index change during this time

        SongMutexLock.Lock();
        if (nSongIndex<m_pDoc->m_SongList.GetSize())
        {
            pSongData=(SongDataStruct*)m_pDoc->m_SongList.GetAt(nSongIndex);
            m_SongData.Path=pSongData->Path;
            m_SongData.Status=pSongData->Status;
        }
        SongMutexLock.Unlock();

        // If not IDed, then ID it

        if (m_SongData.Status==SONG_NEW)
        {

            IndentifySong();

            if (m_bTerminateThread) break;

            // Verify song data is still valid and has not been moved or 
            // deleted. If ok then udpate the data.

            SongMutexLock.Lock();

            if ((!m_bTerminateThread) && (nSongIndex<m_pDoc->m_SongList.GetSize()))
            {
                pSongDataVerify=(SongDataStruct*)m_pDoc->m_SongList.GetAt(nSongIndex);
                if (pSongDataVerify==pSongData)
                {
                    pSongData->Status=m_SongData.Status;
                    pSongData->URL=m_SongData.URL;
                    pSongData->Title=m_SongData.Title;
                    pSongData->Albums.Append(m_SongData.Albums);
                    pSongData->Artists.Append(m_SongData.Artists);
                    pSongData->Genres.Append(m_SongData.Genres);
                }
            }
            SongMutexLock.Unlock();

            if (m_bTerminateThread) break;
 
            // Update song list view 

            ((CSSDMainFrame*)AfxGetApp()->m_pMainWnd)->SendMessage(WM_UPDATESONGLIST, 0, nSongIndex);

        }

        // Check Count, if changed the restart

        SongMutexLock.Lock();
        if (nTotalSongs!=m_pDoc->m_SongList.GetSize())
        {
            nTotalSongs=m_pDoc->m_SongList.GetSize();
            nSongIndex=0;
        }
        else nSongIndex++;
        SongMutexLock.Unlock();

        // Leave if terminated

        if (m_bTerminateThread) break;
        
    }

}

/////////////////////////////////////////////////////////////////////////////
// CSSDSongSigThread routines

/**
 * IdentifySong
 *
 * Indentify the song whose data resides in m_SongData
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDSongSigThread::IndentifySong()
{
    // Codec vars

    codec_type_t FileType;
    sp_id_t *SongInfo;
    codec_state_t *CodecState;
    codec_vtable_t CodecFunctions;
    sp_audio_info_t PCMFormat;
    sp_signature_t Signature; 
    sp_error_t SPError;
    sp_id_recv_t **SongDataRecv = NULL; 

    int nMatches = 0;
    int nResult;
    int nCount;
    
    // Get codec type to use

    FileType = SelectCodecFunctions(&m_SongData.Path, &CodecFunctions);

    if (FileType == CODEC_UNKNOWN)
    {
        m_SongData.Status=SONG_UNKNOWN;
        return;
    }


    // If known codec then ID

    if (FileType!=CODEC_UNKNOWN)
    {

        // Create a new SongPrint ID
        
        SongInfo = sp_id_new();


        // Open file, get the pcm data format, and read tage data

        CodecState = CodecFunctions.open(m_SongData.Path);
        CodecFunctions.get_format(CodecState, &PCMFormat);
        CodecFunctions.get_song_info(CodecState, SongInfo);

        // Leave if terminated

        if (m_bTerminateThread) return;

        // Generate signature
               
        nResult = sp_generate_signature(&PCMFormat,
                                        &Signature,
                                        CodecFunctions.get_pcm_data, 
                                        CodecState);

        // Leave if terminated
    
        if (m_bTerminateThread) return;

		// Error generating signature

		if (nResult==SP_FAILURE)
			m_SongData.Status=SONG_SIG_ERROR;
		else
		{
			// ID signature

			nResult = sp_id_signature(&PCMFormat,
				                      &Signature,
					                  (sp_id_send_t *) SongInfo, 
						              &SongDataRecv, 
							          &nMatches, 
								      &SPError); 

			// Leave if terminated

			if (m_bTerminateThread)
			{
	            nResult = sp_delete_id_recv(SongDataRecv, nMatches);
				return;
			}

			if (nResult==SP_FAILURE) 
				m_SongData.Status=SONG_SERVER_ERROR;
			else if (nResult==SP_FAILURE_EXTENDED)
			{
				m_SongData.Status=SONG_SERVER_EXTENDED_ERROR;
				m_SongData.URL.Format("Server returned: %s [%d]\n",SPError.error, SPError.error_number);
			}
			else if (nMatches==0)
			{
				// Load song data structure with returned Tag

				m_SongData.Title=SongInfo->title;

				nCount=0;
				while (SongInfo->albums[nCount]!=NULL)
				{
					m_SongData.Albums.Add(SongInfo->albums[nCount]);
					nCount++;
				}
				nCount=0;
				while (SongInfo->artists[nCount]!=NULL)
				{
					m_SongData.Artists.Add(SongInfo->artists[nCount]);
					nCount++;
				}
				nCount=0;
				while (SongInfo->genres[nCount]!=NULL)
				{
					m_SongData.Genres.Add(SongInfo->genres[nCount]);
					nCount++;
				}

				// Generate URL for submitting song information

			    m_SongData.URL = CreateSubmitURL(SongInfo, &Signature);
				m_SongData.Status=SONG_UNKNOWN;
			}
			else
			{
				// Load song data structure with returned SongPrint data

				m_SongData.Title=(**SongDataRecv).title;
				nCount=0;
				while ((**SongDataRecv).albums[nCount]!=NULL)
				{
					m_SongData.Albums.Add((**SongDataRecv).albums[nCount]);
					nCount++;
				}
				nCount=0;
				while ((**SongDataRecv).artists[nCount]!=NULL)
				{
		            m_SongData.Artists.Add((**SongDataRecv).artists[nCount]);
					nCount++;
				}
				nCount=0;
				while ((**SongDataRecv).genres[nCount]!=NULL)
				{
		            m_SongData.Genres.Add((**SongDataRecv).genres[nCount]);
					nCount++;
				}

                m_SongData.URL=(**SongDataRecv).moreinfo;
			    m_SongData.Status=SONG_IDED;
			}
		}


        // Close the file and delete the SongPrint ID

        CodecFunctions.close(CodecState);
        nResult = sp_delete_id_recv(SongDataRecv, nMatches);
    }

}


/**
 * SelectCodecFunctions
 *
 * @pstrFilename: Pointer to CString containg the filename
 * @functions: Pointer to codec_vtable_t structure of codec functions
 *
 * Called to load the codec functions based on the extention of the filename.
 * 
 * Return values:
 *  Returns the codec type if known, otherwise returns CODEC_UKNOWN
 * Exceptions:
 *  None
 */
codec_type_t
CSSDSongSigThread::SelectCodecFunctions(CString* pstrFilename, codec_vtable_t *Functions)
{

    // Default to unknown

    codec_type_t CodecReturnType = CODEC_UNKNOWN;

    // Get file extension

    CString strExt=pstrFilename->Right(4);
    strExt.MakeUpper();

    // Check extensions against known types

    if (strExt==".OGG")
    {
#ifdef HAVE_OGGVORBIS
        _CODECS_ADD_FUNCTIONS(Functions, vorbis);
        CodecReturnType = CODEC_OGGVORBIS;
#endif 
    } 
    else if (strExt==".MP3")
    { 
#ifdef HAVE_MPG123
    _CODECS_ADD_FUNCTIONS(Functions, mpg123);
    CodecReturnType = CODEC_MP3_MPG123;
#endif
    } 
    else if (strExt==".WAV")
    {
#ifdef HAVE_WAV
    _CODECS_ADD_FUNCTIONS(Functions, wav);
    CodecReturnType = CODEC_WAV;
#endif
    }

    return CodecReturnType ;
}


/**
 * CreateSubmitURL
 *
 * @pSongInfo: Pointer to song information retrieve from the song file
 * @pSignature: Pointer to song signature data
 *
 * Creates a URL string for submitting unknown song data based on the
 * information in pSongInfo and pSignature.
 *
 * Return values:
 *  Returns URL in a CString
 * Exceptions:
 *  None
 */
CString
CSSDSongSigThread::CreateSubmitURL(sp_id_t * pSongInfo, sp_signature_t * pSignature)
{
	int nCount;
	CString Temp;

	// Assemble Signature String

	CString Signature;
	for (nCount=0; nCount<32; nCount++)
	{
		Temp.Format("%.4x", pSignature->data[nCount]);
		Signature+=Temp;
		if (nCount!=31) Signature+="+";
	}

	// Encode Title

	CString Title=URLEncode(m_SongData.Title);
	CString Albums=StringArrayURLEncode("&album=", &m_SongData.Albums);
	CString Artists=StringArrayURLEncode("&artist=", &m_SongData.Artists);
	CString Genres=StringArrayURLEncode("&genre=", &m_SongData.Genres);

	CString URL;
	URL.Format("http://sigadd.etantrum.com/index.php?sig=%s&title=%s%s%s%s", Signature,
		Title, Artists, Albums, Genres);
		
	return URL;
}


/**
 * URLEncode
 *
 * @strOldString: Original string
 *
 * Convert a string to a URL safe string. Fixes unprintable characters and spaces.
 *
 * Return values:
 *  Returns URL safe string in a CString
 * Exceptions:
 *  None
 */
CString 
CSSDSongSigThread::URLEncode(CString strOldString)
{
	int nFind;
	int nCount;
	CString strNewString;
	CString strTemp;

	for (nCount=0; nCount<strOldString.GetLength(); nCount++)
	{
		if (strOldString[nCount]==' ')
			strNewString+="+";
		else
		{
			strTemp=strOldString[nCount];
			nFind=strTemp.FindOneOf(" ;/?:@&=+$,<>%#\t\r\n\v");
			if ((nFind!=-1) || (isprint(strOldString[nCount])==0))
			{
				strTemp.Format("%%%.2X", strOldString[nCount]);
				strNewString+=strTemp;
			}
			else strNewString+=strOldString[nCount];
		}
	}
	
	return strNewString;
}



/**
 * StringArrayURLEncode
 *
 * @strPrefix: Prefix string for each entry in the array
 * @pStringArray: Pointer to a CStringArray
 *
 * Takes strings in pStringArray and does a URLEncode on each and assembles
 * them into one string, each preceed with strPrefix.
 *
 * Return values:
 *  Returns URL safe string with all pStringArray members seperated with 
 *  strPrefix
 * Exceptions:
 *  None
 */
CString
CSSDSongSigThread::StringArrayURLEncode(CString strPrefix, CStringArray* pStringArray)
{
	CString strItem;
	for (int nCount=0; nCount<pStringArray->GetSize(); nCount++)
		strItem+=strPrefix+URLEncode(pStringArray->GetAt(nCount));
	return strItem;
}

