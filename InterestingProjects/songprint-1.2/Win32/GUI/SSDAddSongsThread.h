// SSDAddSongsThread.h
//
// Interface of the CSSDAddSongsThread class, a CWinThread derived class that
// handles the adding of songs in the background

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

#if !defined(AFX_SSDADDSONGSTHREAD_H__B1FC9614_6E09_11D4_A0FC_000102242A3D__INCLUDED_)
#define AFX_SSDADDSONGSTHREAD_H__B1FC9614_6E09_11D4_A0FC_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSDDocument.h"

/* Structure passed for adding songs:

  pFiles: Pointer to a list of file paths or a directory path in a 
      CStringList

  pDoc: Pointer to the document

  bAddingDirectories: TRUE if pFiles contains a directory path, FALSE if
      it contains file paths

  bRecurseDirs: If bAddingDirectories is TRUE, this determines if songs
      should be added recursively from the directory, or just that
      directory only
*/
struct AddSongsQueryStruct
{
    CStringList*    pFiles;
    CSSDDocument*   pDoc;
    bool            bAddingDirectories;
    bool            bRecurseDirs;
};

/////////////////////////////////////////////////////////////////////////////
// CSSDAddSongsThread thread

class CSSDAddSongsThread : public CWinThread
{
	DECLARE_DYNCREATE(CSSDAddSongsThread)
protected:
	CSSDAddSongsThread();           // protected constructor used by dynamic creation

// Operations
public:

    bool	m_bTerminateThread;     // Set to TRUE when thread should stop
    bool    m_bStopAndClear;        // Set to TRUE for thread to dump messages

	virtual ~CSSDAddSongsThread();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDAddSongsThread)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:

	int RecurseSubDirs(CString CurrentDir);
    int AddSongsInDir(CString* CurrentDir, CString SearchSpec);
	void AddSong(CString* pstrPath);

    AddSongsQueryStruct*    m_pAddQuery;

	// Generated message map functions
	//{{AFX_MSG(CSSDAddSongsThread)
	afx_msg void OnThreadAddSongs(WPARAM wParam, LPARAM lParam);
	afx_msg void OnThreadReactivate();
	afx_msg void OnThreadDie();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDADDSONGSTHREAD_H__B1FC9614_6E09_11D4_A0FC_000102242A3D__INCLUDED_)
