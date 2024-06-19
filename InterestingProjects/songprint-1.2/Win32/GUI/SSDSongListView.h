// SSD Song List View.h : interface of the CSSDSongListView class
//
// Interface of the CSSDSongListView class, a CListView derived class that
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

#if !defined(AFX_SSDSONGLISTVIEW_H__53643E63_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
#define AFX_SSDSONGLISTVIEW_H__53643E63_6D6B_11D4_A0FA_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CSSDSongListView view class

class CSSDSongListView : public CListView
{
protected: // create from serialization only
	CSSDSongListView();
	DECLARE_DYNCREATE(CSSDSongListView)

// Attributes
public:
	CSSDDocument* GetDocument();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDSongListView)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	protected:
	virtual void OnInitialUpdate(); // called first time after construct
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CSSDSongListView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

    // Initial help message and flag for showing it

    CStringArray    m_InitialMessage;
    bool            m_bDisplayInitMessage;

// Generated message map functions
protected:
	//{{AFX_MSG(CSSDSongListView)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnUpdateListCount(WPARAM wParam, LPARAM lParam);
	afx_msg void OnItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKeydown(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in SSD Song List View.cpp
inline CSSDDocument* CSSDSongListView::GetDocument()
   { return (CSSDDocument*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDSONGLISTVIEW_H__53643E63_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
