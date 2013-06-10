#include "dialog.h"
#include "application.h"

// class CDialog

BOOL CALLBACK CDialog::StaticDlgProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//��������� �� ������-������
	CDialog* pDialog;
	//���� ��� ���������-�������������
	if(Msg==WM_INITDIALOG)
	{
		//��������� ������
		pDialog=(CDialog*)lParam;
		SetWindowLong(hWnd,GWL_USERDATA,(LONG)lParam);
		//��������� ���� �������
		pDialog->mhWindow=hWnd;
	}
	else
    //�������� ������
		pDialog=(CDialog*)GetWindowLong(hWnd,GWL_USERDATA);

	//��������� ��������� ���������
	switch(Msg)
	{
	case WM_INITDIALOG:
		//������� ������ ��������������
		SetWindowLong(hWnd,GWL_EXSTYLE,GetWindowLong(hWnd,GWL_EXSTYLE) /*| WS_EX_TOOLWINDOW */| WS_EX_LAYERED);
		SetLayeredWindowAttributes(hWnd,0,255*90/100,LWA_ALPHA);
		//������� ������� �������� ����
//		RemoveMenu(GetSystemMenu(hWnd,FALSE),SC_CLOSE,MF_BYCOMMAND);
		break;
	case WM_CTLCOLORDLG:
	case WM_CTLCOLORSTATIC:
	case WM_CTLCOLORBTN:
//	case WM_CTLCOLOREDIT:
//	case WM_CTLCOLORLISTBOX:
	case WM_CTLCOLORSCROLLBAR:
		SetBkColor((HDC)wParam,gApplication.mBrushSelectColor);
		return (BOOL)gApplication.mhBrushSelect;
	}

	//�������� ������� ���������
	return pDialog ? pDialog->DlgProc(Msg,wParam,lParam) : FALSE;
}

INT_PTR CDialog::Ask(LPCTSTR szTemplate,HWND hWndParent)
{
	//������� ������, ������� ��� � �������� ��������� ��� ������-������
	return DialogBoxParam(GetModuleHandle(NULL),szTemplate,hWndParent,StaticDlgProc,(LPARAM)this);
}
