#ifndef QS_DIALOG_GOTOADDRESS_H
#define QS_DIALOG_GOTOADDRESS_H

/*
���� �������� ����������� ������ CDialogGoToAddress,
������������ ������ � ������������ �������� ������.
*/

#include "dialog_enterstring.h"
#include "cyber_memory.h"

class CDialogGoToAddress : public CDialogEnterString
{
public:
	BOOL Show(HWND hWndParent,CYBER_ADDRESS* pAddress);
};

#endif
