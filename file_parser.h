#ifndef QS_FILE_PARSER_H
#define QS_FILE_PARSER_H

/*
���� �������� ����������� ������������ ������ ����������� ��������� �����.

������������������ �������� ��� �������:
1. ������� SetFile ����������� ���� ��� ���������. ��� ���� ����� CFileParser ������ ��� �� ���������.
			���������� ��������� ���������� mpFileMapping � mFileSize.
2. ������� PrepareAnalize �����������, ����� �� ���������� ���������� ���� ����.
			���������� �������� ��������� ������ ������� �����, ��������� ���������� mFileBufferSize
			�������� ����������� ������.
3. � ������� AskSettings ���������� ����� �������� �������������� ��������� �������.
			���������� ����� �������������� ���������� mFileBufferSize.
			����� ���� ���������, ��� AskSettings - �������������� �����, � ������ ���� �������������
			��������� �� ���������.
4. ������� GetNeededMemorySize ���������� ������ ������, ����������� ����������� ��� ������� �����.
5. �������� ������ ��� �����������.
6. ������� SetMemoryBuffer ������� ���������� ������ �����������.
7. ������� SetEnvironment ���������� ������� ����������� ��� ������� �������.
			���������� ��������� ���������� mpMemory, mpAddressMap � mpExtendMap.
8. ������� Analize - ���������������� ����.
			���������� ������ ���������� ���� � ���������� CCodeParser.
9. ������� Present - ������� ��������� ���� ��������� ��� ������������ ������������������� �����.
10. ������ ����� ������� ������ �����������.
*/

#include "object.h"
#include "cyber_memory.h"
#include "address_map.h"
#include "code_parser.h"

//** ���� ���������� �������
#define FILE_PARSER_ERROR_SUCCESS					0					//�������������� ������ �������� �������
#define FILE_PARSER_ERROR_LOADING					1					//������, ���� ���� �� ��������
#define FILE_PARSER_ERROR_ANALIZING				2					//������ � �������, �� ���� �������� � ����� ��������������� �������

//��������� ��������������� ���������� �� �����������
struct FILE_PARSER_ABOUT
{
	TCHAR szName[0x40];												//��� �����������
	TCHAR szDescription[0x100];								//�������� �����������
	DWORD Version;														//������ �����������
};

class CFileParser : public CObject
{
protected:
	//��������� �����������
	PVOID mpFileMapping;														//�������� ����� �� �������� ������������
	DWORD mFileSize;																//������ �����
	CCyberMemory* mpMemory;													//�����������
	CAddressMap* mpAddressMap;											//����� �������
	CCodeParser* mpCodeParser;											//���������� ����
	std::map<UINT,CObject*>* mpExtendMap;						//����� �������������� �������������� ��������
	PVOID mpFileBuffer;															//����� �����
	DWORD mFileBufferSize;													//������ ������ �����

public:
	//����������� � ����������
	CFileParser();
	virtual ~CFileParser();

	//������� ���� ��� ���������
	VOID SetFile(PVOID pFileMapping,DWORD Size);

	//���������, ����� �� ������ ���������� ���������� ������ ����
	//�������� �� ����������� ��������� ������; ������ ���� ������ �������� ������������ �����,
	//����� ������������ ���� ����� ��������
	virtual BOOL PrepareAnalize() PURE;

	//�������� ���������� ����������� ������
	DWORD GetNeededMemorySize();

	//������� ������, ���������� ��� �������
	VOID SetFileBuffer(PVOID pBuffer);

	//��������, ����� �� �������� �������������� ��������� (���������� ��������, ������������ FALSE)
	virtual BOOL IsAskSettingsAvaliable();
	//�������� �������������� ��������� � ������������ (���������� ��������)
	virtual BOOL AskSettings(HWND hWndParent);

	//������� ��������� �����������
	VOID SetEnvironment(CCyberMemory* pMemory,CAddressMap* pAddressMap,std::map<UINT,CObject*>* pExtendMap);
	//��������� ������ (������������ ��� ������ FILE_PARSER_ERROR_)
	virtual DWORD Analize() PURE;
	//�������� ���������� ����
	CCodeParser* GetCodeParser();

	//�������� ��������� ����� ��� ���������
	virtual CYBER_ADDRESS GetBeginAddress() PURE;

	//�������� ���������� �� ����������� (������ ��������������� �������� ��� ������������)
	virtual VOID About(FILE_PARSER_ABOUT* pAbout) PURE;
};

#endif
