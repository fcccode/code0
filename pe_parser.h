#ifndef QS_PE_PARSER_H
#define QS_PE_PARSER_H

/*
���� �������� ����������� ������ CPEParser, ������������ ������
��������� ������������ ����� ������� PE Executable (EXE).
*/

#include "file_parser.h"

//����� �������� �����������
#define PEPSF_USE_PREFERRED_BASE_ADDRESS						1						//������������ ���������������� ������� �����
#define PEPSF_ANALIZE_DOS_STUB											2						//����������������� DOS-��������
#define PEPSF_ANALIZE_EXPORTS_CODE									4						//����������������� �������������� �������

class CPEParser : public CFileParser
{
public:
	struct SETTINGS
	{
		//���������������� ����� ��������
		CYBER_ADDRESS PreferredBaseAddress;
		//��������� ������� ����� ��������
		CYBER_ADDRESS BaseAddress;

		//�����
		DWORD Flags;
	};

protected:

	//��������� ����� (�������� ������)
	PIMAGE_DOS_HEADER mpImageDosHeader;
	PIMAGE_FILE_HEADER mpImageFileHeader;
	PIMAGE_OPTIONAL_HEADER mpImageOptionalHeader;
	PIMAGE_SECTION_HEADER mpImageSections;
	//�������������� ���������
	SETTINGS mSettings;

protected:

	//������, �� ������� ������ ������� ������� �����

	//�������� ��������� ����� � ��������� ��������� ��������
	BOOL RetrieveHeaders();
	//������������� ���� � �����������
	BOOL MapToCyberMemory();
	//���������������� ����� ��������� �����
	BOOL AnalizeGeneral();
	//���������������� ������ �������
	BOOL AnalizeImport();
	//���������������� ������ ��������
	BOOL AnalizeExport();
	//����������������� ���
	BOOL ParseCode();

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
