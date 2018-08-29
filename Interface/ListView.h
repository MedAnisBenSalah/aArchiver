/* =========================================================
			MRAT - http://www.hackforums.com

-- File: ListView.h
-- Project: Client
-- Author(s): m0niSx
=============================================================*/

#ifndef LISTVIEW_H
#define LISTVIEW_H

class CListView : public CWindow
{
	public:
		CListView(DWORD dwStyle, int iX, int iY, int iW, int iH, CWindow *pParent);
		CListView(char *szResource, CWindow *pParent);
		CListView(HWND hWnd);
		~CListView();

		virtual int  AddColumn(int iColumnId, char *szText, DWORD dwWidth, int iSubItem);
		virtual int  AddItem(char *szText, int iImageId);
		virtual int  AddSubItem(int iItem, int iSubItem, char *szName);
		virtual void DeleteItem(int iItemId);
		virtual void DeleteAllItems();

		virtual int  GetSelectedItem() { return ListView_GetSelectionMark(GetHWND()); };
		virtual void SetSelectedItem(int iItem);
		virtual int  GetItemsCount() { return ListView_GetItemCount(GetHWND()); };
		virtual void GetItemText(int iItem, char *szText);


	private:


};

#endif