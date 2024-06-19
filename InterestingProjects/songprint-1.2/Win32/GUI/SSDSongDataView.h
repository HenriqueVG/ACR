// SSDSongDataView.h
//
// Interface of the CSSDSongDataView class, a CScrollView derived class that
// handles displaying song information
 
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

#if !defined(AFX_SSDSONGDATAVIEW_H__53643E6C_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
#define AFX_SSDSONGDATAVIEW_H__53643E6C_6D6B_11D4_A0FA_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "SSDDocument.h"

// Scroll view margin setting

const MARGIN=1;


/////////////////////////////////////////////////////////////////////////////
// CSSDSongDataView view class

class CSSDSongDataView : public CScrollView
{
protected:
	CSSDSongDataView();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CSSDSongDataView)

// Attributes
public:
	CSSDDocument* GetDocument();

// Operations
public:
	void PrintSongData();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDSongDataView)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnInitialUpdate();     // first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CSSDSongDataView();

	// Formatted Song Data

    CStringArray        m_SongDisplayData;
    CStringArray        m_SongPrintData;
    CArray<int, int>    m_PaginationData;
	CString				m_strURL;

	// Line height and print wait dialog used for print and print preview

    int				m_nLineHeight;
    CDialog         m_PrintWaitDialog;

    void FormatForPrint(CDC* pDC, CPrintInfo* pInfo);
	void FormatSongData(CStringArray* pDestArray, SongDataStruct* pSongData, bool bPrinting);
    void Paginate(CDC* pDC, CPrintInfo* pInfo);

#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CSSDSongDataView)
	afx_msg void OnUpdateSongData(WPARAM wParam, LPARAM lParam);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDSONGDATAVIEW_H__53643E6C_6D6B_11D4_A0FA_000102242A3D__INCLUDED_)
