#ifndef QS_DIALOG_PEPARSERSETTINGS_H
#define QS_DIALOG_PEPARSERSETTINGS_H

/*
���� �������� ����������� ������-������� ��������
��������� ����������� CPEParser.
*/

#include "dialog.h"
#include "pe_parser.h"

class CDialogPEParserSettings : public CDialog
{
protected:
	//��������� �� ��������� � �����������
	CPEParser::SETTINGS* mpSettings;

protected:
	//���������� ���������
	virtual BOOL DlgProc(UINT Msg,WPARAM wParam,LPARAM lParam);

public:
	//�������� ������
	BOOL Show(HWND hWndParent,CPEParser::SETTINGS* pSettings);
};

#endif
