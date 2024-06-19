// SSDDocument.h
//
// Interface of the CSSDDocument class , an MFC CDocument derived class that
// stores all song data

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

#if !defined(AFX_SSDDOCUMENT_H__53643E61_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
#define AFX_SSDDOCUMENT_H__53643E61_6D6B_11D4_A0FA_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// Song indentification status

typedef enum {
    SONG_NEW,
    SONG_IDED,
    SONG_UNKNOWN,
    SONG_SIG_ERROR,
	SONG_SERVER_ERROR,
	SONG_SERVER_EXTENDED_ERROR,
} SongStatusType;

// Song data structure

struct SongDataStruct
{
    CString Path;
    SongStatusType Status;
    CString Title;
    CStringArray Artists;
    CStringArray Albums;
    CStringArray Genres;
    CString URL;
};
    
/////////////////////////////////////////////////////////////////////////////
// CSSDDocument class

class CSSDDocument : public CDocument
{
protected: // create from serialization only
	CSSDDocument();
	DECLARE_DYNCREATE(CSSDDocument)

// Attributes
public:

    // The song list and it's mutex. All others accessing song data must
    // use this mutex variable

    CPtrArray   m_SongList;
    CMutex      m_SongListMutex;

    // Shortcut pointer to the CSSDSongListView list view control, provides
    // direct access to the control from other views and threads

    CListCtrl*  m_pSongListCtrl;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDDocument)
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);
	//}}AFX_VIRTUAL

// Implementation
public:
	void AddSongs(CStringList* pFiles, bool bAddDirectories=FALSE, bool bRecurse=FALSE);
	virtual ~CSSDDocument();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CSSDDocument)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDDOCUMENT_H__53643E61_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
