#ifndef QS_DIALOG_EDITLABEL_H
#define QS_DIALOG_EDITLABEL_H

/*
���� �������� ����������� ������ ������� �������������� �����.
*/

#include "dialog.h"
#include "label.h"

class CDialogEditLabel : public CDialog
{
protected:
	//��������� �� �����
	LPTSTR mszLabel;

protected:
	//������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	//����� �������
	//������������ ������ (� *pszResultLabel) - ��� �����, ������ ������ ������������� � ���������� ������� � ������� delete []
	//���� ������������ NULL, �� ����� ����� �������
	//���� ��������� ������� - FALSE, �� ������������ ����� "������"
	BOOL Show(LPCTSTR szExistLabel,LPTSTR* pszResultLabel,HWND hWndParent);
};

#endif
