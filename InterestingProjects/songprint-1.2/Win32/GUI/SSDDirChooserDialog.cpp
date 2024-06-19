// SSDDirChooserDialog.cpp : implementation of the CSSDDirChooserDialog 
// class
//
// Implementation of the CSSDDirChooserDialog class, a dialog box to let the
// user select a directory from all available drives using a CDirectoryTree
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

#include "stdafx.h"
#include "SongSigDemo.h"
#include "SSDDirChooserDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSSDDirChooserDialog dialog


CSSDDirChooserDialog::CSSDDirChooserDialog(CWnd* pParent /*=NULL*/)
	: CDialog(CSSDDirChooserDialog::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSSDDirChooserDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CSSDDirChooserDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSSDDirChooserDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSSDDirChooserDialog, CDialog)
	//{{AFX_MSG_MAP(CSSDDirChooserDialog)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSSDDirChooserDialog message handlers


/**
 * OnInitDialog
 *
 * Subclass the tree control to a CDirectoryTree class and set the
 * recurse sub directories option to checked.
 * 
 * Return values:
 *  Always returns TRUE
 * Exceptions:
 *  Failure to subclass tree control to CDirectoryTree
 */
BOOL
CSSDDirChooserDialog::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// Subclass Control

	VERIFY(m_DirectoryTreeCtrl.SubclassDlgItem(IDC_DIRECTORYTREE, this));

    // Recurse subdirectories default to checked

	CheckDlgButton(IDC_RECURSESUBDIRS, TRUE);

    // Init the CDirectoryTree class

    m_DirectoryTreeCtrl.Init();

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}


/**
 * OnOK
 *
 * If the user click OK, then save the directory choosen and the recurse
 * subdirectories option in the dialog class for retrieval.
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CSSDDirChooserDialog::OnOK() 
{
    if (IsDlgButtonChecked(IDC_RECURSESUBDIRS)==0)
        m_bRecurseDirs=FALSE;
    else m_bRecurseDirs=TRUE;

	m_strDirectoryChoosen=m_DirectoryTreeCtrl.m_strSelectedPath;
	
	CDialog::OnOK();
}
