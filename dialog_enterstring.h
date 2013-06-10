#ifndef QS_DIALOG_ENTERSTRING_H
#define QS_DIALOG_ENTERSTRING_H

/*
���� �������� ����������� ������ �������, �������
���������� � ������������ ��������� ������.
*/

#include "dialog.h"

class CDialogEnterString : public CDialog
{
protected:
	//��������� �� ����� ��� ������
	LPTSTR mszTextResult;
	//������ � ������� � �������
	LPCTSTR mszDialogTitle;
	LPCTSTR mszDialogCaption;
	LPCTSTR mszDialogInitText;

protected:
	//������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	CDialogEnterString();
	~CDialogEnterString();

	//�������� ��������� ������; ���� ������������ �������� ������,
	//�� ������������ NULL. ����� ������������ ��������� �� ����� �� �������,
	//������� ��������, ���� ���������� ��������� ������.
	LPTSTR Show(LPCTSTR szDialogTitle,LPCTSTR szDialogCaption,LPCTSTR szDialogInitText,HWND hWndParent);
};

#endif
