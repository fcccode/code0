#ifndef QS_COM_PARSER_H
#define QS_COM_PARSER_H

/*
���� �������� ����������� ������ ������� ������������ ����� COM.
*/

#include "file_parser.h"

//����� �������� �����������
#define COMPSF_32BIT														1					//���� �������� 32-������
#define COMPSF_INTERRUPTTABLE										2					//� ������ ����� ����������� ������� ����������

class CCOMParser : public CFileParser
{
public:
	//��������� �������� COM-�����������
	struct SETTINGS
	{
		//����� ��������
		CYBER_ADDRESS BaseAddress;
		//����� ����� �����
		CYBER_ADDRESS EntryPointAddress;
		//�����
		DWORD Flags;
	};

protected:
	//��������� �����������
	SETTINGS mSettings;

public:
	//��������, ����� �� �������� �������������� ���������
	virtual BOOL IsAskSettingsAvaliable();
	//�������� �������������� ��������� � ������������
	virtual BOOL AskSettings(HWND hWndParent);

	//���������, ����� �� ������ ���������� ���������� ������ ����
	virtual BOOL PrepareAnalize();
	//��������� ������
	virtual DWORD Analize();

	//�������� ��������� ����� ��� ���������
	virtual CYBER_ADDRESS GetBeginAddress();

	//�������� ���������� �� �����������
	virtual VOID About(FILE_PARSER_ABOUT* pAbout);
};

#endif
