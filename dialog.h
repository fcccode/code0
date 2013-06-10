#ifndef QS_DIALOG_H
#define QS_DIALOG_H

/*
���� �������� ����������� ������������ ������ �������,
������������� ��� ��������� ����������� ��������.
*/

#include "windows.h"

class CDialog
{
protected:
	//����
	HWND mhWindow;

protected:
	//����������� ������� ���������
	static BOOL CALLBACK StaticDlgProc(HWND hWnd,UINT Msg,WPARAM wParam,LPARAM lParam);
	//������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam) PURE;

	//������� ������
	INT_PTR Ask(LPCTSTR szTemplate,HWND hWndParent);
};

#endif
