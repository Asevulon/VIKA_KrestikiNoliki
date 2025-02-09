#pragma once
#include <afxwin.h>
#include<gdiplus.h>
#include<vector>

#define TURN_DONE WM_USER + 1

// в дате крестик - это 1, а -1 - это нолик
class Drawer :
	public CStatic
{
private:
	ULONG_PTR token;
	int width = 0;
	int height = 0;
	std::vector<int> m_data;
public:
	Drawer();
	~Drawer();
	virtual void DrawItem(LPDRAWITEMSTRUCT /*lpDrawItemStruct*/);
	void SetData(std::vector<int>& val);
	HWND parent;
	bool stop = false;
	bool init = false;
	DECLARE_MESSAGE_MAP()
		afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};




