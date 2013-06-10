#include "dialog_editlabel.h"
#include "application.h"
#include "resource.h"

// class CDialogEditLabel

BOOL CDialogEditLabel::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	switch(Msg)
	{
	case WM_INITDIALOG:
		//������� ����� ��� ���������� ����
		FORWARD_WM_SETFONT(GetDlgItem(mhWindow,IDC_EDIT_LABEL),gApplication.mhFontCode,FALSE,SendMessage);
		//���� ��������� ����� ������
		if(mszLabel)
		{
			//������� � � ��������� ����
			SetDlgItemText(mhWindow,IDC_EDIT_LABEL,mszLabel);
			//�������� ������ Delete
			EnableWindow(GetDlgItem(mhWindow,IDC_BUTTON_DELETE_LABEL),TRUE);
		}
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:										//������ "OK"
			{
				//�������� ����� ��������� �����
				HWND hEdit=GetDlgItem(mhWindow,IDC_EDIT_LABEL);
				UINT Length=GetWindowTextLength(hEdit)+1;
				//���� ����� ����
				if(Length>1)
				{
					//�������� ������ � �������� �����
					mszLabel=new TCHAR[Length];
					GetWindowText(hEdit,mszLabel,Length);
					//��
					EndDialog(mhWindow,TRUE);
				}
				else
				{
					//���� ������� �����
					mszLabel=NULL;
					EndDialog(mhWindow,TRUE);
				}
			}
			return TRUE;
		case IDCANCEL:								//������ "Cancel"
			//������ �� ������
			EndDialog(mhWindow,FALSE);
			return TRUE;
		case IDC_BUTTON_DELETE_LABEL:	//������ "Delete"
			//���� ������� �����
			mszLabel=NULL;
			EndDialog(mhWindow,TRUE);
			return TRUE;
		}
		return FALSE;
	}
	return FALSE;
}

BOOL CDialogEditLabel::Show(LPCTSTR szExistLabel,LPTSTR* pszResultLabel,HWND hWndParent)
{
	//��������� ��������� �� ������
	mszLabel=(LPTSTR)szExistLabel;
	//������� ������
	BOOL Result=Ask(MAKEINTRESOURCE(IDD_DIALOG_EDITLABEL),hWndParent);
	//������� �������� ��������� ������
	*pszResultLabel=mszLabel;
	//������� �����
	return Result;
}
