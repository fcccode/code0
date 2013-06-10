#ifndef QS_DIALOG_EDITCOMMENT_H
#define QS_DIALOG_EDITCOMMENT_H

/*
���� �������� ����������� ������ ������� �������������� �����������.
*/

#include "dialog.h"
#include "comment.h"

class CDialogEditComment : public CDialog
{
protected:
	//��������� �� ����� �����������
	LPTSTR mszComment;

protected:
	//������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	//����� �������
	//������������ ������ (� *pszResultComment) - ����� �����������, ������ ������ ������������� � ���������� ������� � ������� delete []
	//���� ������������ NULL, �� ����� ����� �������
	//���� ��������� ������� - FALSE, �� ������������ ����� "������"
	BOOL Show(LPCTSTR szExistComment,LPTSTR* pszResultComment,HWND hWndParent);
};

#endif
