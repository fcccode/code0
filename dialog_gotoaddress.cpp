#include "dialog_gotoaddress.h"

// class CDialogGoToAddress

BOOL CDialogGoToAddress::Show(HWND hWndParent,CYBER_ADDRESS* pAddress)
{
	//������������ ������� �����
	TCHAR s[0x10];
	_stprintf(s,TEXT("%08X"),*pAddress);
	//������� ������
	LPCTSTR szResult=CDialogEnterString::Show(TEXT("Enter target address:"),TEXT("Go to address"),s,hWndParent);
	if(szResult)
	{
		//������������� ������ � �����
		_stscanf(szResult,TEXT("%X"),pAddress);
		return TRUE;
	}
	return FALSE;
}
