// SSD Song Sig Thread.h : interface of the CSSDSongSigThread class
//
// Interface of the CSSDSongSigThread class, a CWinThread derived class that
// handles the creating of signatures for songs and obtaining song
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

#if !defined(AFX_SSDSONGSIGTHREAD_H__13FBF7E1_6FB9_11D4_969D_00E01890773C__INCLUDED_)
#define AFX_SSDSONGSIGTHREAD_H__13FBF7E1_6FB9_11D4_969D_00E01890773C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSDDocument.h"

// Include SongPrint library and required 3rd party libraries

#include "codecs.h"
#include "sp_types.h"
#include "songprint.h"
#include "rfftw.h"
#include "..\config.h"

// Include codecs

#ifdef HAVE_MPG123
#include "mpg123_iface.h"
#endif

#ifdef HAVE_WAV
#include "wav_iface.h"
#endif

#ifdef HAVE_OGGVORBIS
#include "vorbis_iface.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CSSDSongSigThread thread

class CSSDSongSigThread : public CWinThread
{
	DECLARE_DYNCREATE(CSSDSongSigThread)
protected:
	CSSDSongSigThread();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	virtual ~CSSDSongSigThread();

    // Termination flag

    bool	m_bTerminateThread;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDSongSigThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

    // Current song data and document pointer

    SongDataStruct  m_SongData;
    CSSDDocument*   m_pDoc;

    codec_type_t SelectCodecFunctions(CString* pstrFilename, codec_vtable_t *functions);  
    void IndentifySong();

	CString CreateSubmitURL(sp_id_t * pSongInfo, sp_signature_t * pSignature);

	CString URLEncode(CString strOldString);
	CString StringArrayURLEncode(CString Prefix, CStringArray* pStringArray);

	// Generated message map functions
	//{{AFX_MSG(CSSDSongSigThread)
	afx_msg void OnThreadDie();
	afx_msg void OnThreadNewSongCount(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDSONGSIGTHREAD_H__13FBF7E1_6FB9_11D4_969D_00E01890773C__INCLUDED_)
