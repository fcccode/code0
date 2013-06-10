#include "dialog_comparsersettings.h"
#include "resource.h"

// class CDialogCOMParserSettings

BOOL CDialogCOMParserSettings::DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam)
{
	//����� �� ���������
	switch(Msg)
	{
	case WM_INITDIALOG:
		{
			//������� ����� �������� � ����� ����� �����
			TCHAR s[0x10];
			_stprintf(s,TEXT("%08X"),mpSettings->BaseAddress);
			SetDlgItemText(mhWindow,IDC_EDIT_BASEADDRESS,s);
			_stprintf(s,TEXT("%08X"),mpSettings->EntryPointAddress);
			SetDlgItemText(mhWindow,IDC_EDIT_ENTRYPOINTADDRESS,s);
			//���������� ������������� 16/32 ����
			CheckDlgButton(mhWindow,(mpSettings->Flags & COMPSF_32BIT) ? IDC_RADIO_32BIT : IDC_RADIO_16BIT,BST_CHECKED);
			//���������� ������ "Interrupt Table"
			CheckDlgButton(mhWindow,IDC_CHECK_INTERRUPTTABLE,(mpSettings->Flags & COMPSF_INTERRUPTTABLE) ? BST_CHECKED : BST_UNCHECKED);
		}
		return TRUE;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			{
				//�������� ����� �������� � ����� ����� �����
				{
					TCHAR s[0x10];
					CYBER_ADDRESS Address;
					GetDlgItemText(mhWindow,IDC_EDIT_BASEADDRESS,s,0x10);
					_stscanf(s,TEXT("%x"),&Address);
					mpSettings->BaseAddress=Address;
					GetDlgItemText(mhWindow,IDC_EDIT_ENTRYPOINTADDRESS,s,0x10);
					_stscanf(s,TEXT("%x"),&mpSettings->EntryPointAddress);
				}
				//���������� �����
				mpSettings->Flags=
					(IsDlgButtonChecked(mhWindow,IDC_RADIO_32BIT)==BST_CHECKED ? COMPSF_32BIT : 0) |
					(IsDlgButtonChecked(mhWindow,IDC_CHECK_INTERRUPTTABLE)==BST_CHECKED ? COMPSF_INTERRUPTTABLE : 0)
					;

				EndDialog(mhWindow,TRUE);
			}
			return TRUE;
		case IDCANCEL:
			EndDialog(mhWindow,FALSE);
			return TRUE;
		}
		return FALSE;
	case WM_CLOSE:
		EndDialog(mhWindow,FALSE);
		return TRUE;
	}
	return FALSE;
}

BOOL CDialogCOMParserSettings::Show(HWND hWndParent,CCOMParser::SETTINGS* pSettings)
{
	//��������� ��������� �� ���������
	mpSettings=pSettings;
	//������� ������
	return Ask(MAKEINTRESOURCE(IDD_DIALOG_COMPARSERSETTINGS),hWndParent);
}
