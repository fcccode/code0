#ifndef QS_DIALOG_ABOUT_H
#define QS_DIALOG_ABOUT_H

/*
���� �������� ����������� ������ ������� "About Code0".
*/

#include "dialog.h"

class CDialogAbout : public CDialog
{
protected:
	//�������� � �������� �����������
	HDC mhdcImage;
	HBITMAP mbmpImage;

protected:
	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);
public:
	//�������� ������
	VOID Show(HWND hWndParent);
};

#endif
