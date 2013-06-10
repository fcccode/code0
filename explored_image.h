#ifndef QS_EXPLORED_IMAGE_H
#define QS_EXPLORED_IMAGE_H

#include "object.h"
#include "cyber_memory.h"
#include "address_map.h"
#include "code_parser.h"
#include "file_parser.h"
#include "dangle_group.h"

/*
���� �������� ���������� ������ CExploredImage, ������� ������������ ���� ����������� ��� ������� ����.
���� ����� �������� ��������, � ���������� ��� �������, ������������ ��� �������.
��� �������, ������� ������������ �����������. �� CExploredImage �������� ������
"����������", "���������" ������� �������������, � �� �������� ���������� � ����������������
���������� ��� ������������.

LoadFile ��������� ������ ���������� �������� � ������������� �����, ������� �� �����������.
*/

class CExploredImage : public CObject
{
protected:
	//��� ������������ �����
	TCHAR mszFileName[MAX_PATH];
	//��������� �� ������ ������������ ������
	PVOID mpFileBuffer;
	//��������� �� �������� �����
	PVOID mpFileMapping;
	//������ ��������
	DWORD mFileMappingSize;

	//�������� ����������������� ������������ �����
	CCyberMemory* mpMemory;
	//����� ������� �����
	CAddressMap* mpAddressMap;
	//���������� ����
	CCodeParser* mpCodeParser;

	//����� �������������� �������������� ��������
	std::map<UINT,CObject*> mExtendMap;

protected:
	//��������� ����
	BOOL LoadFile(LPCTSTR szFileName);

public:
	CExploredImage(LPCTSTR szFileName);
	virtual ~CExploredImage();

	//��������, ������ ���� ��� ���
	BOOL IsFileOpened();
	//�������� ��� ����� (��� ����)
	LPCTSTR GetFileName();

	//��������� ������ ����� � ����������� ��� �� ����� (������������ ��� ���������� ��������� �����������)
	DWORD AnalizeAndPresent(CFileParser* pFileParser,CDangleGroup* pDangleGroup);

	//�������� ����� �������
	CAddressMap* GetAddressMap();
	//�������� ���������� �� ������
	CAddressData* GetAddressData(CYBER_ADDRESS Address);
	//�������� ��������� �� �����������
	CCyberMemory* GetMemory();
	//�������� ��������� �� ���������� ����
	CCodeParser* GetCodeParser();
	//�������� ��������� �� ����� �������������� �������������� ��������
	std::map<UINT,CObject*>* GetExtendMap();

	//��������� ������ � ������������ � �������� ���������� �����
	CFileParser* AskFileParser(HWND hWndParent);

	//������ � ���������������
	VOID SetAddressInterpretationData(CYBER_ADDRESS Address,DWORD Size);
	VOID SetAddressInterpretationCode(CYBER_ADDRESS Address);
	VOID SetAddressInterpretationStringUnicode(CYBER_ADDRESS Address);
	VOID SetAddressInterpretationStringASCII(CYBER_ADDRESS Address);
	VOID DeleteAddressIntepretation(CYBER_ADDRESS Address);
};

#endif
