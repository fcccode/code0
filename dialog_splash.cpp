#include "dialog_splash.h"
#include "resource.h"

// class CSplashWindow

CSplashWindow::CSplashWindow()
{
	//�������������
	mhWindow=NULL;
}

CSplashWindow::~CSplashWindow()
{
	Hide();
}

VOID CSplashWindow::Show(HWND hWndParent,LPCTSTR szText,LPCTSTR szCaption)
{
	//������ ������� ����
	Hide();
	//��������� ���������
	mszText=szText;
	mszCaption=szCaption;
	//������� ����� ����
	mhWindow=CreateDialogParam(GetModuleHandle(NULL),MAKEINTRESOURCE(IDD_DIALOG_SPLASH),hWndParent,StaticDlgProc,(LPARAM)this);
	ShowWindow(mhWindow,SW_NORMAL);
	UpdateWindow(mhWindow);
}

VOID CSplashWindow::Hide()
{
	//���� ���� ����, ���������� ���
	if(mhWindow)
	{
		DestroyWindow(mhWindow);
		mhWindow=NULL;
	}
}

BOOL CSplashWindow::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		//������� ����� ��������� � ���������
		SetDlgItemText(mhWindow,IDC_STATIC_MESSAGE,mszText);
		SetWindowText(mhWindow,mszCaption);
		return TRUE;
	}
	return FALSE;
}
