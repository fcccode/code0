#include "dialog_enterstring.h"
#include "application.h"
#include "resource.h"

// class CDialogEnterString

BOOL CDialogEnterString::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		//������� ����� ��� ���������� ����
		FORWARD_WM_SETFONT(GetDlgItem(mhWindow,IDC_EDIT_STRING),gApplication.mhFontCode,FALSE,SendMessage);
		//���������������� ������ � �������
		SetWindowText(mhWindow,mszDialogCaption);
		SetDlgItemText(mhWindow,IDC_STATIC_TITLE,mszDialogTitle);
		if(mszDialogInitText) SetDlgItemText(mhWindow,IDC_EDIT_STRING,mszDialogInitText);
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				//�������� ����
				HWND hEdit=GetDlgItem(mhWindow,IDC_EDIT_STRING);
				//�������� ����� ������ � ����
				INT Length=GetWindowTextLength(hEdit);
				//�������� ������ � ����������� �����
				mszTextResult=new TCHAR[Length+1];
				GetWindowText(hEdit,mszTextResult,Length+1);
				//��������� ������
				EndDialog(mhWindow,TRUE);
			}
			return TRUE;
		case IDCANCEL:
			//��������� ������
			EndDialog(mhWindow,FALSE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

CDialogEnterString::CDialogEnterString()
{
	//���������������� ���������
	mszTextResult=NULL;
}

CDialogEnterString::~CDialogEnterString()
{
	//���������� ������ ������ �������������� ������
	SafeDeleteMassive(mszTextResult);
}

LPTSTR CDialogEnterString::Show(LPCTSTR szDialogTitle,LPCTSTR szDialogCaption,LPCTSTR szDialogInitText,HWND hWndParent)
{
	//��������� ��������� �� ������
	mszDialogTitle=szDialogTitle;
	mszDialogCaption=szDialogCaption;
	mszDialogInitText=szDialogInitText;
	//������� ������; ���� �������
	if(Ask(MAKEINTRESOURCE(IDD_DIALOG_ENTERSTRING),hWndParent))
		//������� ��������� �� ������
		return mszTextResult;
	//����� ������ �� ����������
	return NULL;
}
