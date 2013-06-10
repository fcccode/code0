#include "window.h"

// class CWindow

CWindow::~CWindow()
{
}

LRESULT CALLBACK CWindow::StaticWndProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//������
	CWindow* pWindow;
	//���� ��������� - � ��������
	if(Msg==WM_CREATE)
	{
		//��������� ������
		pWindow=(CWindow*)((CREATESTRUCT*)lParam)->lpCreateParams;
		SetWindowLong(hWnd,GWL_USERDATA,(LONG)pWindow);
		//��������� ����
		pWindow->mhWindow=hWnd;
	}
	else
		//�������� ������
		pWindow=(CWindow*)GetWindowLong(hWnd,GWL_USERDATA);
	//���� ������ ��������
	if(pWindow)
		//�������� ��� ���������
		return pWindow->WindowProc(Msg,wParam,lParam);
	//����� ������ ����������; ��������� ��������� �� ���������
	return DefWindowProc(hWnd,Msg,wParam,lParam);
}

HWND CWindow::GetWindowHandle()
{
	//������� ���������� ����
	return mhWindow;
}

VOID CWindow::PostMessage(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//��������� ���� ���������
	::PostMessage(mhWindow,Msg,wParam,lParam);
}
