// SSDDirChooserDialog.h
//
// Interface of the CSSDDirChooserDialog class, a dialog box to let the user
// select a directory from all available drives using a CDirectoryTree
// control

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

#if !defined(AFX_SSDDIRCHOOSERDIALOG_H__95ACE292_6EEA_11D4_A0FE_000102242A3D__INCLUDED_)
#define AFX_SSDDIRCHOOSERDIALOG_H__95ACE292_6EEA_11D4_A0FE_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DirectoryTree.h"

/////////////////////////////////////////////////////////////////////////////
// CSSDDirChooserDialog dialog

class CSSDDirChooserDialog : public CDialog
{
// Construction
public:
	CString GetSelectedDir();
	CSSDDirChooserDialog(CWnd* pParent = NULL);   // standard constructor

    // Dialog results saved for retrieval

	CString			m_strDirectoryChoosen;
	bool			m_bRecurseDirs;

// Dialog Data
	//{{AFX_DATA(CSSDDirChooserDialog)
	enum { IDD = IDD_DIRCHOOSER };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CSSDDirChooserDialog)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

    CDirectoryTree  m_DirectoryTreeCtrl;

	// Generated message map functions
	//{{AFX_MSG(CSSDDirChooserDialog)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SSDDIRCHOOSERDIALOG_H__95ACE292_6EEA_11D4_A0FE_000102242A3D__INCLUDED_)
