/* =========================================================
			MRAT - http://www.hackforums.com

-- File: ListView.cpp
-- Project: Client
-- Author(s): m0niSx
=============================================================*/

#include "Main.h"

CListView::CListView(DWORD dwStyle, int iX, int iY, int iW, int iH, CWindow *pParent) : CWindow(WC_LISTVIEW, "", dwStyle, iX, iY, iW, iH, pParent)
{

}

CListView::CListView(char *szResource, CWindow *pParent) : CWindow(szResource, pParent)
{

}

CListView::CListView(HWND hWnd) : CWindow(hWnd)
{

}

CListView::~CListView()
{

}

int CListView::AddColumn(int iColumnId, char *szText, DWORD dwWidth, int iSubItem)
{
	// Create LVCOLUMN structure
	LVCOLUMN lvColumn;
	// Element types to add
	lvColumn.mask = LVCF_TEXT | LVCF_WIDTH;
	// Set the sub item id
	lvColumn.iSubItem = iSubItem;
	// Set the column width
	lvColumn.cx = dwWidth;
	// Set column text
	lvColumn.pszText = szText;
	// Send the windiw message
	return SendWindowMessage<int>(LVM_INSERTCOLUMN, iColumnId, (LPARAM)&lvColumn);
}

int CListView::AddItem(char *szText, int iImageId)
{
	// Create LVITEM structure
	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(LVITEM));
	// Set the item mask
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT;
	// Set item image id
	lvItem.iImage = iImageId;
	// Set column text
	lvItem.pszText = szText;
	lvItem.cchTextMax = strlen(szText) + 1;
	lvItem.iItem = ListView_GetItemCount(GetHWND()); // Item id
	lvItem.iSubItem = 0; // No sub item
	// Add the item
	return ListView_InsertItem(GetHWND(), &lvItem);
}

int CListView::AddSubItem(int iItem, int iSubItem, char *szName)
{
	LVITEM lvItem;
	memset(&lvItem, 0, sizeof(LVITEM));
	// Set the structure members
	lvItem.mask = LVIF_IMAGE | LVIF_TEXT; // Flags
	lvItem.iImage = -1;	// No image specified
	lvItem.pszText = szName; // Set text
	lvItem.cchTextMax = strlen(szName) + 1; // Max text length
	lvItem.iItem = iItem; // Item id
	lvItem.iSubItem = iSubItem; // Sub item id
	// Insert the item
	return SendWindowMessage<int>(LVM_SETITEM, 0, (LPARAM)&lvItem);
}

void CListView::DeleteItem(int iItemId)
{
	SendWindowMessage<void>(LVM_DELETEITEM, (WPARAM)iItemId, (LPARAM)0);
}

void CListView::DeleteAllItems()
{
	SendWindowMessage<void>(LVM_DELETEALLITEMS, (WPARAM)0, (LPARAM)0);
}

void CListView::GetItemText(int iItem, char *szText)
{
	ListView_GetItemText(GetHWND(), iItem, -1, szText, MAX_PATH);
}

void CListView::SetSelectedItem(int iItem)
{
	// Deselect all items
	ListView_SetItemState(GetHWND(), -1, 0, LVIS_SELECTED);
	// Scroll to the item if its not currently shown
	SendWindowMessage<void>(LVM_ENSUREVISIBLE, (WPARAM)iItem, FALSE);
	// Select the item
	ListView_SetItemState(GetHWND(), iItem, LVIS_SELECTED, LVIS_SELECTED);
	// Set the item focus
	ListView_SetItemState(GetHWND(), iItem, LVIS_FOCUSED, LVIS_FOCUSED);
	// Set the focus window to us
	SetAsFocusWindow();
	// Update the window
	Update();
}