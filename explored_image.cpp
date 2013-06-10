#include "explored_image.h"
#include "danglewindow_danglegroup.h"
#include "danglewindow_code.h"
#include "dialog_selectfileanalizer.h"

// class CExploredImage

CExploredImage::CExploredImage(LPCTSTR szFileName)
{
	//���������������� ����������
	mpFileBuffer=NULL;
	mpFileMapping=NULL;
	mpMemory=NULL;
	mpAddressMap=NULL;
	mpCodeParser=NULL;
	//����������� ��� �����
	_tcscpy(mszFileName,szFileName);

	//����������� ��������� ����
	if(LoadFile(mszFileName))
	{
		//������� �������� ����������������� � ����� ������� �����
		mpMemory=new CCyberMemory;
		mpAddressMap=new CAddressMap;
	}
}

CExploredImage::~CExploredImage()
{
	//���������� ������ �����
	SafeVirtualFree(mpFileBuffer);
	//��������� ������������� �����
	SafeUnmapViewOfFile(mpFileMapping);
	//������� �������� �����������������, ����� ������� � ���������� ����
	SafeRelease(mpMemory);
	SafeRelease(mpAddressMap);
	SafeRelease(mpCodeParser);

	//�������� ����� �������������� ��������
	for(std::map<UINT,CObject*>::iterator i=mExtendMap.begin();i!=mExtendMap.end();i++)
		(*i).second->Release();
	mExtendMap.clear();
}

BOOL CExploredImage::IsFileOpened()
{
	//�������, ������ �� ����
	return mpFileMapping ? TRUE : FALSE;
}

LPCTSTR CExploredImage::GetFileName()
{
	return mszFileName;
}

BOOL CExploredImage::LoadFile(LPCTSTR szFileName)
{
	//������� ����
	HANDLE hFile=CreateFile(szFileName,GENERIC_READ,FILE_SHARE_READ,NULL,OPEN_EXISTING,0,NULL);
	if(hFile==INVALID_HANDLE_VALUE) return FALSE;
	mFileMappingSize=GetFileSize(hFile,NULL);
	HANDLE hMapping=CreateFileMapping(hFile,NULL,PAGE_READONLY,0,0,NULL);
	CloseHandle(hFile);
	if(!hMapping) return FALSE;
	mpFileMapping=MapViewOfFile(hMapping,FILE_MAP_READ,0,0,0);
	CloseHandle(hMapping);
	if(!mpFileMapping) return FALSE;

	return TRUE;
}

DWORD CExploredImage::AnalizeAndPresent(CFileParser* pFileParser,CDangleGroup* pDangleGroup)
{
	//�������� ����������� ����� ������
	DWORD BufferSize=pFileParser->GetNeededMemorySize();

	//�������� ������
	mpFileBuffer=VirtualAlloc(NULL,BufferSize,MEM_RESERVE | MEM_COMMIT,PAGE_READWRITE);
	//������� �����
	pFileParser->SetFileBuffer(mpFileBuffer);
	//������� ��������� �������
	pFileParser->SetEnvironment(mpMemory,mpAddressMap,&mExtendMap);

	//��������� ������
	DWORD Result=pFileParser->Analize();
	//���� ������ �� �����������
	switch(Result)
	{
	case FILE_PARSER_ERROR_LOADING:
		//����������� ������
		break;
	case FILE_PARSER_ERROR_SUCCESS:
	case FILE_PARSER_ERROR_ANALIZING:
		{
			//��������� ��������� �� ���������� ����
			mpCodeParser=pFileParser->GetCodeParser();

			//����������� ���� �� ����� - ������� ���� ��������� ���� �� ����� �����
			CDangleWindow* pDangleWindow=new CDangleWindowCode(this,pFileParser->GetBeginAddress(),mszFileName);
			pDangleGroup->AddWindow(pDangleWindow);
			pDangleWindow->Release();
		}
		break;
	default:
		_ASSERTE(("Unknown file parser error code.",TRUE));
		break;
	}

	//������� ���������
	return Result;
}

CAddressMap* CExploredImage::GetAddressMap()
{
	mpAddressMap->AddRef();
	return mpAddressMap;
}

CAddressData* CExploredImage::GetAddressData(CYBER_ADDRESS Address)
{
	return mpAddressMap->GetAddress(Address);
}

CCyberMemory* CExploredImage::GetMemory()
{
	mpMemory->AddRef();
	return mpMemory;
}

CCodeParser* CExploredImage::GetCodeParser()
{
	mpCodeParser->AddRef();
	return mpCodeParser;
}

std::map<UINT,CObject*>* CExploredImage::GetExtendMap()
{
	return &mExtendMap;
}

CFileParser* CExploredImage::AskFileParser(HWND hWndParent)
{
	//������� ���������� ��� �����
	CDialogSelectFileAnalizer Dialog;
	return Dialog.Show(hWndParent,mszFileName,mpFileMapping,mFileMappingSize);
}

VOID CExploredImage::SetAddressInterpretationData(CYBER_ADDRESS Address,DWORD Size)
{
	//������� �������������
	CInterpretation* pInterpretation=new CInterpretationData(mpMemory,Address,Size);
	//������� ������ ������
	CAddressData* pAddressData=mpAddressMap->OpenAddress(Address);
	//������� �������������
	pAddressData->SetInterpretation(pInterpretation);
	//������� ������ ������
	mpAddressMap->SetAddressSize(Address,Size);
	//���������� ������������� � ������ ������
	pInterpretation->Release();
	pAddressData->Release();
}

VOID CExploredImage::SetAddressInterpretationCode(CYBER_ADDRESS Address)
{
	//����������������� ���
	mpCodeParser->ParseEntryPoint(Address);
}

VOID CExploredImage::SetAddressInterpretationStringUnicode(CYBER_ADDRESS Address)
{
	//������� �������������
	CInterpretationString* pInterpretation=new CInterpretationStringUnicode(mpMemory,Address);
	//������� ������ ������
	CAddressData* pAddressData=mpAddressMap->OpenAddress(Address);
	//������� �������������
	pAddressData->SetInterpretation(pInterpretation);
	//������� ������ ������
	mpAddressMap->SetAddressSize(Address,pInterpretation->GetStringSize());
	//���������� ������������� � ������ ������
	pInterpretation->Release();
	pAddressData->Release();
}

VOID CExploredImage::SetAddressInterpretationStringASCII(CYBER_ADDRESS Address)
{
	//������� �������������
	CInterpretationString* pInterpretation=new CInterpretationStringASCII(mpMemory,Address);
	//������� ������ ������
	CAddressData* pAddressData=mpAddressMap->OpenAddress(Address);
	//������� �������������
	pAddressData->SetInterpretation(pInterpretation);
	//������� ������ ������
	mpAddressMap->SetAddressSize(Address,pInterpretation->GetStringSize());
	//���������� ������������� � ������ ������
	pInterpretation->Release();
	pAddressData->Release();
}

VOID CExploredImage::DeleteAddressIntepretation(CYBER_ADDRESS Address)
{
	//������� �������������
	CAddressData* pAddressData=mpAddressMap->GetAddress(Address);
	if(pAddressData)
	{
		pAddressData->SetInterpretation(NULL);
		CAddressData* pPreviousAddressData=mpAddressMap->GetAddress(Address+pAddressData->GetSize());
		if(pPreviousAddressData)
		{
			pPreviousAddressData->SetPreviousSize(1);
			pPreviousAddressData->Release();
		}
		pAddressData->SetSize(1);
		pAddressData->Release();
	}
}
