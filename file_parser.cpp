#include "file_parser.h"

// class CFileParser

CFileParser::CFileParser()
{
	//�������� ���������
	mpFileMapping=NULL;
	mpMemory=NULL;
	mpAddressMap=NULL;
	mpCodeParser=NULL;
	mpExtendMap=NULL;
	mpFileBuffer=NULL;
}

CFileParser::~CFileParser()
{
	//������� �������
	SafeRelease(mpMemory);
	SafeRelease(mpAddressMap);
	SafeRelease(mpCodeParser);
}

VOID CFileParser::SetFile(PVOID pFileMapping,DWORD Size)
{
	//��������� ���������
	mpFileMapping=pFileMapping;
	mFileSize=Size;
}

DWORD CFileParser::GetNeededMemorySize()
{
	//������� �������� ����������
	return mFileBufferSize;
}

VOID CFileParser::SetFileBuffer(PVOID pBuffer)
{
	//��������� ���������
	mpFileBuffer=pBuffer;
}

BOOL CFileParser::IsAskSettingsAvaliable()
{
	//��� ��������, �������������� ���������� �� �����������
	return FALSE;
}

BOOL CFileParser::AskSettings(HWND hWndParent)
{
	//������ �� ����������, ��� ��������
	return TRUE;
}

VOID CFileParser::SetEnvironment(CCyberMemory* pMemory,CAddressMap* pAddressMap,std::map<UINT,CObject*>* pExtendMap)
{
	//��������� ���������
	mpMemory=pMemory;
	mpMemory->AddRef();
	mpAddressMap=pAddressMap;
	mpAddressMap->AddRef();
	mpExtendMap=pExtendMap;
}

CCodeParser* CFileParser::GetCodeParser()
{
	//������� ���������� ����
	mpCodeParser->AddRef();
	return mpCodeParser;
}
