#include "dangle_window.h"
#include "application.h"

// class CDangleWindow

CDangleWindow::CDangleWindow()
{
	//�������� ����������
	mhWindow=NULL;
	mszName=NULL;
	ZeroMemory(&mUI,sizeof(mUI));
}

CDangleWindow::~CDangleWindow()
{
}

LRESULT CDangleWindow::PaintedWindowProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_WINDOWPOSCHANGED:
	case WM_ACTIVATE:
		//���������� ����� �� ����
		SetFocus(mhWindow);
		break;
	}
	//���� ������ �������� ��������� ������������� �������
	return DangleProc(Msg,wParam,lParam);
}

LPCTSTR CDangleWindow::GetName()
{
	//������� ���
	return mszName;
}

VOID CDangleWindow::GetUI(DANGLE_WINDOW_UI* pUI)
{
	//������� ���������� � ���������������� ����������
	*pUI=mUI;
}

VOID CDangleWindow::Create(HWND hWndParent,RECT Rect)
{
	//������� ����
	mhWindow=CreateWindow(gApplication.mszDangleWindowClass,mszName,WS_CHILD | WS_CLIPSIBLINGS | WS_CLIPCHILDREN | WS_VISIBLE,Rect.left,Rect.top,Rect.right-Rect.left,Rect.bottom-Rect.top,hWndParent,NULL,GetModuleHandle(NULL),this);
}

VOID CDangleWindow::Delete()
{
	//������� ����
	DestroyWindow(mhWindow);
}
