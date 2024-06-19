// SSDMainFrame.h
//
// Interface of the CSSDMainFrame class, a CFrameWnd derived class that
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

#if !defined(AFX_SSDMAINFRAME_H__53643E5F_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
#define AFX_SSDMAINFRAME_H__53643E5F_6D6B_11D4_A0FA_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSDAddSongsThread.h"
#include "SSDSongSigThread.h"
#include "SSDSongListView.h"

/////////////////////////////////////////////////////////////////////////////
// CSSDMainFrame class

class CSSDMainFrame : public CFrameWnd
{
	
protected: // create from serialization only
	CSSDMainFrame();
	DECLARE_DYNCREATE(CSSDMainFrame)

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDMainFrame)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual BOOL OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSSDMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

    // Threads

	CSSDSongSigThread*	m_pSongSigThread;
    CSSDAddSongsThread* m_pAddSongsThread;
	
protected:

    // Main frame controls

	CStatusBar          m_wndStatusBar;
	CToolBar            m_wndToolBar;
    CSplitterWnd        m_wndSplitter;

// Generated message map functions
protected:
	//{{AFX_MSG(CSSDMainFrame)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnUpdateAddsongs(CCmdUI* pCmdUI);
	afx_msg void OnAddsongs();
	afx_msg void OnUpdateStatusBar(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateSongList(WPARAM wParam, LPARAM lParam);
	afx_msg void OnUpdateAddDirectory(CCmdUI* pCmdUI);
	afx_msg void OnAddDirectory();
	afx_msg void OnClose();
	afx_msg void OnFilePrintPreview();
	afx_msg void OnFilePrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDMAINFRAME_H__53643E5F_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
