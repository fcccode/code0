#ifndef QS_DIALOG_SPLASH_H
#define QS_DIALOG_SPLASH_H

/*
���� �������� �������� ������ ��� ������������� ���� �����-������.
*/

#include "dialog.h"

class CSplashWindow : CDialog
{
protected:
	//����� � ���������
	LPCTSTR mszText;
	LPCTSTR mszCaption;

	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	//����������� � ����������
	CSplashWindow();
	~CSplashWindow();

	//������� ����
	VOID Show(HWND hWndParent,LPCTSTR szText,LPCTSTR szCaption);
	//������� ����
	VOID Hide();
};

#endif
