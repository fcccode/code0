#include "dialog_editcomment.h"
#include "application.h"
#include "resource.h"

// class CDialogEditComment

BOOL CDialogEditComment::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		//������� ����� ��� ���������� ����
		FORWARD_WM_SETFONT(GetDlgItem(mhWindow,IDC_EDIT_COMMENT),gApplication.mhFontCode,FALSE,SendMessage);
		//���� ��������� ����� �����
		if(mszComment)
		{
			//������� ��� � ��������� ����
			SetDlgItemText(mhWindow,IDC_EDIT_COMMENT,mszComment);
			//�������� ������ Delete
			EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_DELETE_COMMENT),TRUE);
		}
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:										//������ "OK"
			{
				//�������� ����� ���������� ������
				HWND hEdit=GetDlgItem(mhWindow,IDC_EDIT_COMMENT);
				UINT Length=GetWindowTextLength(hEdit)+1;
				//���� ����� ����
				if(Length>1)
				{
					//�������� ������ � �������� �����
					mszComment=new TCHAR[Length];
					GetWindowText(hEdit,mszComment,Length);
					//��
					EndDialog(mhWindow,TRUE);
				}
				else
				{
					//���� ������� �����������
					mszComment=NULL;
					EndDialog(mhWindow,TRUE);
				}
			}
			return TRUE;
		case IDCANCEL:								//������ "Cancel"
			//������ �� ������
			EndDialog(mhWindow,FALSE);
			return TRUE;
		case IDC_BUTTON_DELETE_COMMENT:	//������ "Delete"
			//���� ������� �����������
			mszComment=NULL;
			EndDialog(mhWindow,TRUE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CDialogEditComment::Show(LPCTSTR szExistComment,LPTSTR* pszResultComment,HWND hWndParent)
{
	//��������� ��������� �� ������
	mszComment=(LPTSTR)szExistComment;
	//������� ������
	BOOL Result=Ask(MAKEINTRESOURCE(IDD_DIALOG_EDITCOMMENT),hWndParent);
	//������� �������� ��������� ������
	*pszResultComment=mszComment;
	//������� �����
	return Result;
}
