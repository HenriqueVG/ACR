// DirectoryTree.h 
//
// Interface of the CDirectoryTree class, a Tree control that lists all
// drives and directories and allows the user to choose one.

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

#if !defined(AFX_DIRECTORYTREE_H__95ACE291_6EEA_11D4_A0FE_000102242A3D__INCLUDED_)
#define AFX_DIRECTORYTREE_H__95ACE291_6EEA_11D4_A0FE_000102242A3D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


// Drive image list indexes

typedef enum { 
    ILI_HARD_DISK, 
    ILI_FLOPPY, 
    ILI_CD_ROM, 
    ILI_NET_DRIVE, 
    ILI_REMOVABLE, 
    ILI_OPEN_FOLDER, 
    ILI_CLOSED_FOLDER
} DriveImageType;


/////////////////////////////////////////////////////////////////////////////
// CDirectoryTree control

class CDirectoryTree : public CTreeCtrl
{
// Construction
public:
	CDirectoryTree();

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDirectoryTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	void Init();
	virtual ~CDirectoryTree();
	
    // Stores the user selected path on exit

    CString		m_strSelectedPath;


protected:
	// Protected routines
    
    BOOL AddDriveNode (CString& strDrive);
    void AddDummyNode (HTREEITEM hItem);
    DWORD GetSerialNumber (CString& strDrive);
    CString GetPathFromItem (HTREEITEM hItem);
    BOOL IsMediaValid (CString& strPathName);
    HTREEITEM GetDriveNode (HTREEITEM hItem);
    void DeleteChildren (HTREEITEM hItem);
    BOOL IsPathValid (CString& strPathName);
    UINT AddDirectoryNodes (HTREEITEM hItem, CString& strPathName);
    BOOL IsDriveNode (HTREEITEM hItem);

    // Drive serial number array and icon images list

    DWORD       m_dwMediaID[26];
    CImageList  m_imglDrives;

	//{{AFX_MSG(CDirectoryTree)
	afx_msg void OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIRECTORYTREE_H__95ACE291_6EEA_11D4_A0FE_000102242A3D__INCLUDED_)
