#include "com_parser.h"
#include "dialog_comparsersettings.h"
#include "application.h"

// class CCOMParser

BOOL CCOMParser::IsAskSettingsAvaliable()
{
	return TRUE;
}

BOOL CCOMParser::AskSettings(HWND hWndParent)
{
	//��������� ������
	CDialogCOMParserSettings Dialog;
	return Dialog.Show(hWndParent,&mSettings);
}

BOOL CCOMParser::PrepareAnalize()
{
	//������� ������� ������� ����� ��������� �� �����, ��� ��� COM-���� ������ ����������
	//��������� ������, ��� ������ ��� ����� ����� - �� MZ
	//(����� �������� ���������� MS-DOS)

	if(mFileSize>=2)
		if(*(WORD*)mpFileMapping==IMAGE_DOS_SIGNATURE)
			return FALSE;

	//��������� ����������� ������ ������ � ������ ������������ (�� ���� �������� ������)
	mFileBufferSize=mFileSize+MEMORY_PAGE_SIZE;

	//������� ��������� ����������� �� ���������
	mSettings.BaseAddress=0x100;
	mSettings.EntryPointAddress=0x100;
	mSettings.Flags=0;

	//��, �������� ���������
	return TRUE;
}

DWORD CCOMParser::Analize()
{
	//�������� ����� ������ ������������� � �����������
	CYBER_ADDRESS MappingBegin=ALIGN_DOWN(mSettings.BaseAddress,MEMORY_PAGE_SIZE);
	//����������� ���� � ����� � ������ ������
	CopyMemory((PBYTE)mpFileBuffer+mSettings.BaseAddress-MappingBegin,mpFileMapping,mFileSize);
	//������������� ����� � �����������, ������� � ��������� ������
	if(!mpMemory->Map(MappingBegin,mpFileBuffer,ALIGN_UP(mFileSize,MEMORY_PAGE_SIZE))) return FILE_PARSER_ERROR_LOADING;

	//������� ���������� ����
	mpCodeParser=new CCodeParser(mpMemory,mpAddressMap,&gApplication.mInstructionSet16,&gApplication.mInstructionSet32);
	//���������� ������ �����
	mpCodeParser->SetMode((mSettings.Flags & COMPSF_32BIT) ? MODE_32BIT : MODE_16BIT);
	//������� ����� � ����� �����
	mpAddressMap->SetLabel(mSettings.EntryPointAddress,TEXT("EntryPoint"));
	CAddressData* pAddress=mpAddressMap->OpenAddress(mSettings.EntryPointAddress);
	pAddress->CreateSubroutine();
	pAddress->Release();
	//��������� ������ ����, ������� � ����� �����
	mpCodeParser->ParseEntryPoint(mSettings.EntryPointAddress);

	//���� ������� ��������� ������� ����������
	if(mSettings.Flags & COMPSF_INTERRUPTTABLE)
		for(unsigned i=0;i<256;++i)
			try
			{
				DWORD address=mpMemory->Dword(i*4);
				address=(address & 0xFFFF) + (address >> 12);
				if(address)
				{
					//������� ����� ��� �������� �������
					WCHAR buffer[0x30];
					_stprintf(buffer,TEXT("int_%02xh_entry"),i+1);
					mpAddressMap->SetLabel(i*4,buffer);
					//�������� ��� ������
					CAddressData* pAddressData=mpAddressMap->OpenAddress(i*4);
					CInterpretation* pInterpretation=new CInterpretationData(mpMemory,i*4,4);
					pAddressData->SetInterpretation(pInterpretation);
					mpAddressMap->SetAddressSize(i*4,4);
					pInterpretation->Release();
					pAddressData->Release();

					//������� ����� ��� ����
					_stprintf(buffer,TEXT("int_%02xh_handler"),i+1);
					mpAddressMap->SetLabel(address,buffer);
					//������� ������������
					pAddressData=mpAddressMap->OpenAddress(address);
					pAddressData->CreateSubroutine();
					pAddressData->Release();
					//��������� ������ ����
					mpCodeParser->ParseEntryPoint(address);
				}
			}
			catch(CCyberMemoryPageFaultException)
			{
			}

	//��!
	return FILE_PARSER_ERROR_SUCCESS;
}

CYBER_ADDRESS CCOMParser::GetBeginAddress()
{
	//������� ��������� �����
	return mSettings.EntryPointAddress;
}

VOID CCOMParser::About(FILE_PARSER_ABOUT* pAbout)
{
	//������� ��������
	_tcscpy(pAbout->szName,TEXT("MS-DOS .COM Files Parser"));
	_tcscpy(pAbout->szDescription,TEXT("This parser is used to disassembly standard COM files (usually \"*.com\" file mask), such consist of only 16-bit code. Be sure that your file is COM file because no checks are performed."));
	pAbout->Version=MAKELONG(0,1);
}
