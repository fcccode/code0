#ifndef QS_DIALOG_COMPARSERSETTINGS_H
#define QS_DIALOG_COMPARSERSETTINGS_H

/*
���� �������� ����������� ������-������� ��������
��������� ����������� CCOMParser.
*/

#include "dialog.h"
#include "com_parser.h"

class CDialogCOMParserSettings : public CDialog
{
protected:
	//��������� �� ��������� � �����������
	CCOMParser::SETTINGS* mpSettings;

protected:
	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	//�������� ������
	BOOL Show(HWND hWndParent,CCOMParser::SETTINGS* pSettings);
};

#endif
