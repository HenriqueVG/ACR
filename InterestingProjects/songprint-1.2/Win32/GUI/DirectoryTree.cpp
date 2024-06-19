// DirectoryTree.cpp : implementation of the CDirectoryTree class
//
// Implementation of the CDirectoryTree class, a Tree control that lists all 
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
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 */

#include "stdafx.h"
#include "SongSigDemo.h"
#include "DirectoryTree.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDirectoryTree

CDirectoryTree::CDirectoryTree()
{
}

CDirectoryTree::~CDirectoryTree()
{
}


BEGIN_MESSAGE_MAP(CDirectoryTree, CTreeCtrl)
	//{{AFX_MSG_MAP(CDirectoryTree)
	ON_NOTIFY_REFLECT(TVN_ITEMEXPANDING, OnItemexpanding)
	ON_NOTIFY_REFLECT(TVN_SELCHANGED, OnSelchanged)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDirectoryTree message handlers

/**
 * OnItemexpanding
 * @pNMHDR: WIN32 Tree control header
 * @pResult: WIN32 event result. Set to FALSE to abort event action
 *
 * Called when a user clicks to expand a tree item. The drive is first
 * checked to see if the media has changed or the drive is empty, if so then 
 * all items for the drive are deleted. Then the path is checked for 
 * validity, if it is no longer valid then the drive is removed. if all 
 * checked are satified then the subitems are removed on node collapse or the
 * subdirectories added for node expanding.
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CDirectoryTree::OnItemexpanding(NMHDR* pNMHDR, LRESULT* pResult) 
{

    // Get item selected

    NM_TREEVIEW* pnmtv = (NM_TREEVIEW*) pNMHDR;
    HTREEITEM hItem = pnmtv->itemNew.hItem;
    
    // Do not do any action until verified

    *pResult = FALSE;

    // Get the path for the item selected

    CString strPathName = GetPathFromItem (hItem);

    // Reset the drive node if the drive is empty or the media changed.

    if (!IsMediaValid (strPathName)) {
        HTREEITEM hRoot = GetDriveNode (hItem);
        Expand (hRoot, TVE_COLLAPSE);
        DeleteChildren (hRoot);
        AddDummyNode (hRoot);
        *pResult = TRUE;
        return;
    }

    // Delete the item if strPathName no longer specifies a valid path.

    if (!IsPathValid (strPathName)) {
        DeleteItem (hItem);
        *pResult = TRUE;
        return;
    }

    // If the item is expanding, delete the dummy item attached to it
    // and add folder items. If the item is collapsing, delete
    // its folder items and add a dummy item

    if (pnmtv->action == TVE_EXPAND) {
        DeleteChildren (hItem);
        if (!AddDirectoryNodes (hItem, strPathName))
            *pResult = TRUE;
    }
    else {
        DeleteChildren (hItem);
        AddDummyNode (hItem);
    }
}


/**
 * OnSelchanged
 * @pNMHDR: WIN32 Tree control header
 * @pResult: WIN32 event result. Set to FALSE to abort event action
 *
 * Called when a user clicks to expand a tree item. The drive is first
 * checked to see if the media has changed or the drive is empty, if so then 
 * all items for the drive are deleted. Then the path is checked for 
 * validity, if it is no longer valid then the drive is removed. if all 
 * checked are satified then the item is marked for redraw so that it's
 * icon can be updated.
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CDirectoryTree::OnSelchanged(NMHDR* pNMHDR, LRESULT* pResult) 
{
    HTREEITEM hItem = ((NM_TREEVIEW*) pNMHDR)->itemNew.hItem;
    m_strSelectedPath = GetPathFromItem (hItem);

    // Reset the drive node if the drive is empty or the media changed.

    if (!IsMediaValid (m_strSelectedPath)) {
        HTREEITEM hRoot = GetDriveNode (hItem);
        Expand (hRoot, TVE_COLLAPSE);
        DeleteChildren (hRoot);
        AddDummyNode (hRoot);
        return;
    }

    // Delete the item if m_strSelectedPath no longer specifies a valid path.

    if (!IsPathValid (m_strSelectedPath)) {
        DeleteItem (hItem);
        return;
    }

    // Invalidate the item so it is updated

    Invalidate(TRUE);
}


/////////////////////////////////////////////////////////////////////////////
// CDirectoryTree routines

/**
 * Init
 *
 * Called for first time setup of the Tree control, to initialize the image
 * lists, and add all drive letters to the root of the tree.
 * 
 * Return values:
 *  None
 * Exceptions:
 *  Failure to create image list
 */
void
CDirectoryTree::Init()
{

    // Setup images

    m_imglDrives.Create (IDB_DRIVEIMAGES, 16, 1, RGB (255, 0, 255));
    SetImageList (&m_imglDrives, TVSIL_NORMAL);

    // Init for drive list creation

    int nPos = 0;
    UINT nCount = 0;
    CString strDrive = "?:\\";

    // Get list of drives and add them

    DWORD dwDriveList = ::GetLogicalDrives ();

    // Check all drive letters and add drives that exist

    while (dwDriveList) {
        if (dwDriveList & 1) {
            strDrive.SetAt (0, 0x41 + nPos);
            if (AddDriveNode (strDrive))
                nCount++;
        }
        dwDriveList >>= 1;
        nPos++;
    }
}


/**
 * AddDriveNode
 * @pstrDrive: Pointer to CString containing a drive letter in the form
 *             "X:\\" where X is the drive letter
 *
 * Called to add drive letters to the root of the tree and set the 
 * appropriate icon. Drives for removable devices such as CD-ROM's, floppies,
 * etc. will have the serial numbers of any media in the drive marked to 
 * detect media changes.
 * 
 * Return values:
 *  TRUE/FALSE based on if the drive was added
 * Exceptions:
 *  None
 */
BOOL
CDirectoryTree::AddDriveNode (CString& pstrDrive)
{
    HTREEITEM hItem;

    // Get the drive type and 0 offset value

    UINT nType = ::GetDriveType ((LPCTSTR) pstrDrive);
    UINT nDrive = (UINT) pstrDrive[0] - 0x41;

    // Add known drive types, set icon, and get serial number of any
    // inserted media if necessary

    switch (nType) {

    case DRIVE_REMOVABLE:
        hItem = InsertItem (pstrDrive, ILI_REMOVABLE, ILI_REMOVABLE);
        AddDummyNode (hItem);
        m_dwMediaID[nDrive] = GetSerialNumber (pstrDrive);
        break;

    case DRIVE_FIXED:
        hItem = InsertItem (pstrDrive, ILI_HARD_DISK, ILI_HARD_DISK);
        AddDummyNode (hItem);
        break;

    case DRIVE_REMOTE:
        hItem = InsertItem (pstrDrive, ILI_NET_DRIVE,
            ILI_NET_DRIVE);
        AddDummyNode (hItem);
        break;

    case DRIVE_CDROM:
        hItem = InsertItem (pstrDrive, ILI_CD_ROM, ILI_CD_ROM);
        AddDummyNode (hItem);
        m_dwMediaID[nDrive] = GetSerialNumber (pstrDrive);
        break;

    case DRIVE_RAMDISK:
        hItem = InsertItem (pstrDrive, ILI_HARD_DISK, ILI_HARD_DISK);
        AddDummyNode (hItem);
        break;

    default:
        return FALSE;
    }

    return TRUE;
}


/**
 * AddDriveNode
 * @hItem: Handle to a tree control item
 *
 * Inserts a empty dummy node into the tree item sent in hItem
 * 
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CDirectoryTree::AddDummyNode (HTREEITEM hItem)
{
    InsertItem ("", 0, 0, hItem);
}


/**
 * GetSerialNumber
 * @pstrDrive: Pointer to CString containing a drive letter in the form
 *             "X:\\" where X is the drive letter.
 *
 * Returns the serial number of the drive media in pstrDrive
 *
 * Return values:
 *  Serial number of media in drive or 0xFFFFFFFF for none
 * Exceptions:
 *  None
 */
DWORD
CDirectoryTree::GetSerialNumber (CString& strDrive)
{
    DWORD dwSerialNumber;
    if (!::GetVolumeInformation ((LPCTSTR) strDrive, NULL, 0,
        &dwSerialNumber, NULL, NULL, NULL, 0))
        dwSerialNumber = 0xFFFFFFFF;
    return dwSerialNumber;
}


/**
 * GetPathFromItem
 * @hItem: Handle to a tree control item
 *
 * Returns the full path of the node in hItem
 *
 * Return values:
 *  CString containing the full path generated from hItem
 * Exceptions:
 *  None
 */
CString
CDirectoryTree::GetPathFromItem (HTREEITEM hItem)
{

    // Build path from node to root

    CString strPathName;
    while (hItem != NULL) {
        CString string = GetItemText (hItem);
        if ((string.Right (1) != "\\") && !strPathName.IsEmpty ())
            string += "\\";
        strPathName = string + strPathName;
        hItem = GetParentItem (hItem);
    }
    return strPathName;
}


/**
 * IsMediaValid
 * @pstrPathName: Pointer to a CString contraining a path with drive letter
 *
 * This function is to check removable media to see if the media has changed
 * or has been removed. Returns False if the media is no longer valid. This
 * function always returns True for fixed media
 *
 * Return values:
 *  TRUE/FALSE based on if the media is valid
 * Exceptions:
 *  None
 */
BOOL
CDirectoryTree::IsMediaValid (CString& pstrPathName)
{
    // Return TRUE if the drive doesn't support removable media.

    UINT nDriveType = GetDriveType ((LPCTSTR) pstrPathName);
    if ((nDriveType != DRIVE_REMOVABLE) && (nDriveType != DRIVE_CDROM))
        return TRUE;

    // Return FALSE if the drive is empty (::GetVolumeInformation fails).

    DWORD dwSerialNumber;
    CString strDrive = pstrPathName.Left(3);
    UINT nDrive = (UINT) strDrive[0] - 0x41;

    if (!::GetVolumeInformation ((LPCTSTR) strDrive, NULL, 0,
        &dwSerialNumber, NULL, NULL, NULL, 0)) {
        m_dwMediaID[nDrive] = 0xFFFFFFFF;
        return FALSE;
    }

    // Also return FALSE if the disk's serial number has changed.

    if ((m_dwMediaID[nDrive] != dwSerialNumber) &&
        (m_dwMediaID[nDrive] != 0xFFFFFFFF)) {
        m_dwMediaID[nDrive] = dwSerialNumber;
        return FALSE;
    }

    // Update our record of the serial number and return TRUE.

    m_dwMediaID[nDrive] = dwSerialNumber;
    return TRUE;
}


/**
 * GetDriveNode
 * @hItem: Handle to a tree control item
 *
 * Returns the handle to the drive letter node at the top of the tree for the
 * node in which hItem resides.
 *
 * Return values:
 *  Tree control handle of the drive letter node
 * Exceptions:
 *  None
 */
HTREEITEM
CDirectoryTree::GetDriveNode (HTREEITEM hItem)
{
    HTREEITEM hParent;

    do {
        hParent = GetParentItem (hItem);
        if (hParent != NULL)
            hItem = hParent;
    } while (hParent != NULL);
    return hItem;
}


/**
 * DeleteChildren
 * @hItem: Handle to a tree control item
 *
 * Deletes all tree control nodes contained within hItem
 *
 * Return values:
 *  None
 * Exceptions:
 *  None
 */
void
CDirectoryTree::DeleteChildren (HTREEITEM hItem)
{
    HTREEITEM hChild = GetChildItem (hItem);

    while (hChild != NULL) {
        HTREEITEM hNextItem = GetNextSiblingItem (hChild);
        DeleteItem (hChild);
        hChild = hNextItem;
    }
}


/**
 * IsPathValid
 * @pstrPathName: Pointer to a CString contraining a path with drive letter
 *
 * Checks the path in pstrPathName for validity. Assumes that the drive
 * letter is valid.
 *
 * Return values:
 *  TRUE/FALSE based on if the path is valid
 * Exceptions:
 *  None
 */
BOOL
CDirectoryTree::IsPathValid (CString& strPathName)
{

    // Only drive letter? If so then path is good

    if (strPathName.GetLength () == 3)
        return TRUE;

    // Use FindFirstFile to see if the path is valid and a directory

    HANDLE hFind;
    WIN32_FIND_DATA fd;
    BOOL bResult = FALSE;

    if ((hFind = ::FindFirstFile ((LPCTSTR) strPathName, &fd)) !=
        INVALID_HANDLE_VALUE) {
            if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
                bResult = TRUE;
        ::FindClose (hFind);
    }
    return bResult;
}


/**
 * AddDirectoryNodes
 * @hItem: Handle to a tree control item
 * @pstrPathName: Pointer to a CString contraining a path with drive letter
 *
 * Adds all subdirectories in the directory path pointed to by pstrPathName
 * as nodes to the tree control handle hItem.
 *
 * Return values:
 *  Number of directories added
 * Exceptions:
 *  None
 */
UINT
CDirectoryTree::AddDirectoryNodes (HTREEITEM hItem, CString& pstrPathName)
{
    HANDLE hFind;
    WIN32_FIND_DATA fd;

    UINT nCount = 0;

    // Setup file spec

    CString strFileSpec = pstrPathName;
    if (strFileSpec.Right (1) != "\\")
        strFileSpec += "\\";
    strFileSpec += "*.*";

    // Start search 

    if ((hFind = ::FindFirstFile ((LPCTSTR) strFileSpec, &fd)) ==
        INVALID_HANDLE_VALUE) {
        if (IsDriveNode (hItem))
            AddDummyNode (hItem);
        return 0;
    }

    // Search/add all directories

    do {
        if (fd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            CString strFileName = (LPCTSTR) &fd.cFileName;

            // Do not add . and ..

            if ((strFileName != ".") && (strFileName != "..")) {
                HTREEITEM hChild =
                    InsertItem ((LPCTSTR) &fd.cFileName,
                    ILI_CLOSED_FOLDER, ILI_OPEN_FOLDER, hItem, TVI_SORT);

                CString strNewPathName = pstrPathName;
                if (strNewPathName.Right (1) != "\\")
                    strNewPathName += "\\";

                strNewPathName += (LPCTSTR) &fd.cFileName;
                AddDummyNode (hChild);
                nCount++;
            }
        }
    } while (::FindNextFile (hFind, &fd));

    ::FindClose (hFind);

    return nCount;
}


/**
 * IsDriveNode
 * @hItem: Handle to a tree control item
 *
 * Checks of the node in hItem is a drive letter node. Return true/false 
 * result.
 *
 * Return values:
 *  TRUE/FALSE based on if hItem is a drive letter node
 * Exceptions:
 *  None
 */
BOOL
CDirectoryTree::IsDriveNode (HTREEITEM hItem)
{
    return (GetParentItem (hItem) == NULL) ? TRUE : FALSE;
}

