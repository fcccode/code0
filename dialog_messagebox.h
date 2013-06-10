#ifndef QS_DIALOG_MESSAGEBOX_H
#define QS_DIALOG_MESSAGEBOX_H

/*
���� �������� ����������� ������ ������� - ���� ���������.
*/

#include "dialog.h"

class CDialogMessageBox : public CDialog
{
protected:
	//��������� �������
	HWND mhWndParent;
	LPCTSTR mszText;
	LPCTSTR mszCaption;
	UINT mButtonsCount;
	LPCTSTR* mszButtonNames;
	HANDLE mhIcon;
	UINT mDefaultButton;

protected:
	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	//�������� ������
	UINT Show(HWND hWndParent,LPCTSTR szText,LPCTSTR szCaption,UINT ButtonsCount,LPCTSTR* szButtonNames,HANDLE hIcon=NULL,UINT DefaultButton=0);
};

#endif
