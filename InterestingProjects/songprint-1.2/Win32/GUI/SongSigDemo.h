// SongSigDemo.h
//
// Main header file for the SONGSIGDEMO application. Creates the application
// and manages the about box.

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

#if !defined(AFX_SONGSIGDEMO_H__53643E5B_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
#define AFX_SONGSIGDEMO_H__53643E5B_6D6B_11D4_A0FA_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"       // main symbols

// Windows Messages for Project

#define WM_ADDSONGSTHREAD_ADD           (WM_USER+100)
#define WM_ADDSONGSTHREAD_REACTIVATE    (WM_USER+101)
#define WM_THREAD_DIE			        (WM_USER+102)
#define WM_UPDATELISTCOUNTS             (WM_USER+103)
#define WM_UPDATESONGDATA               (WM_USER+104)
#define WM_UPDATESTATUSBAR		        (WM_USER+105)
#define WM_UPDATESONGLIST		        (WM_USER+106)
#define WM_SONGSIGTHREAD_NEWSONGS       (WM_USER+107)

/////////////////////////////////////////////////////////////////////////////
// CSSDApp:
// See SongSigDemo.cpp for the implementation of this class
//

class CSSDApp : public CWinApp
{
public:
	CSSDApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation
	//{{AFX_MSG(CSSDApp)
	afx_msg void OnAppAbout();
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SONGSIGDEMO_H__53643E5B_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
