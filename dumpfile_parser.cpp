#include "dumpfile_parser.h"
#include "application.h"

// class CDumpFileParser

BOOL CDumpFileParser::PrepareAnalize()
{
	//�������� ��������� �����
	if(mFileSize<sizeof(FILE_DUMP_HEADER)) return FALSE;
	mpHeader=(FILE_DUMP_HEADER*)mpFileMapping;
	//��������� ��������� �����
	if(mpHeader->Magics[0]!=FILE_DUMP_MAGIC0 || mpHeader->Magics[1]!=FILE_DUMP_MAGIC1) return FALSE;
	//��������� ������������ ������� �����
	if(mpHeader->FileSize!=mFileSize) return FALSE;

	//�������� ������� ��������
	mRegionsCount=mpHeader->RegionsCount;
	mThreadsCount=mpHeader->ThreadsCount;
	mModulesCount=mpHeader->ModulesCount;

	//��������� ������������� ������� ����� ��� �������� ���������� ��������, ������� � �������
	DWORD NeededFileSize=sizeof(FILE_DUMP_HEADER)+mpHeader->RegionsCount*sizeof(FILE_DUMP_REGION)+mpHeader->ThreadsCount*sizeof(FILE_DUMP_THREAD)+mpHeader->ModulesCount*sizeof(FILE_DUMP_MODULE);
	if(mFileSize<NeededFileSize) return FALSE;

	//�������� ��������� �� ������� ����������
	mpRegions=(FILE_DUMP_REGION*)(mpHeader+1);
	mpThreads=(FILE_DUMP_THREAD*)(mpRegions+mRegionsCount);
	mpModules=(FILE_DUMP_MODULE*)(mpThreads+mThreadsCount);
	//��������� ����������� ������ ������ ��� ����� �������� ��������
	mFileBufferSize=0;
	for(UINT i=0;i<mRegionsCount;++i)
		if(mpRegions[i].State==MEM_COMMIT)
			mFileBufferSize+=mpRegions[i].Size;
	//��������� ������������� ������� �����
	NeededFileSize+=mFileBufferSize;
	if(mFileSize<NeededFileSize) return FALSE;

	//��, ���������� ����� � �������
	return TRUE;
}

DWORD CDumpFileParser::Analize()
{
	//������� ��������� ������
	PBYTE pMemory=(PBYTE)mpFileBuffer;
	//���� �� ��������
	UINT i;
	for(i=0;i<mRegionsCount;++i)
	{
		//��������� �� ������ � ���������� �������
		LPCTSTR szState;
		//����� �� ��������� �������
		switch(mpRegions[i].State)
		{
		case MEM_COMMIT:										//������ ��������������, � ������ ��������
			//����������� ������ �������
			CopyMemory(pMemory,(PBYTE)mpFileMapping+mpRegions[i].DataOffset,mpRegions[i].Size);
			//������������� ������ � �����������
			mpMemory->Map(mpRegions[i].BaseAddress,pMemory,mpRegions[i].Size);
			//�������� ��������� �� ������ � ������ �����
			pMemory+=mpRegions[i].Size;
			//������� ������ ���������
			szState=TEXT("commit");
			break;
		case MEM_RESERVE:										//������ ��������������
			//������� ������ ���������
			szState=TEXT("reserve");
			break;
		case MEM_FREE:											//������ ��������
			//������� ������ ���������
			szState=TEXT("free");
			break;
		default:
			szState=TEXT("(unknown state)");
			break;
		}
/*				//������� �������������-������
				CAddressData* pAddressData=mpAddressMap->OpenAddress(mpRegions[i].BaseAddress);
				CInterpretation* pInterpretation=new CInterpretationData(mpMemory,mpRegions[i].BaseAddress,mpRegions[i].Size);
				pAddressData->SetInterpretation(pInterpretation);
				pInterpretation->Release();
				pAddressData->Release();
				mpAddressMap->SetAddressSize(mpRegions[i].BaseAddress,mpRegions[i].Size);*/
		//������������ ����� ��� �����
		TCHAR szLabel[0x40];
		_stprintf(szLabel,TEXT("Region state = \'%s\', size = %Xh"),szState,mpRegions[i].Size);
		//������� �����
		mpAddressMap->SetLabel(mpRegions[i].BaseAddress,szLabel);
	}

	//������� ���������� ����
	mpCodeParser=new CCodeParser(mpMemory,mpAddressMap,&gApplication.mInstructionSet16,&gApplication.mInstructionSet32);
	//���������� 32-������ �����
	mpCodeParser->SetMode(MODE_32BIT);

	//���������� ����� ����� ����� ������� � ����������������� ��
	//���� �� �������
	for(i=0;i<mModulesCount;++i)
	{
		//�������� ������� ����� ������
		CYBER_ADDRESS BaseAddress=mpModules[i].BaseAddress;
		try
		{
			//������� DOS-��������� ������
			IMAGE_DOS_HEADER DosHeader;
			mpMemory->Data(&DosHeader,BaseAddress,sizeof(DosHeader));
			//��������� ��������� MZ
			if(DosHeader.e_magic!=IMAGE_DOS_SIGNATURE) continue;
			//�������� �������� �� new-exe ���������
			DWORD FileHeaderOffset=BaseAddress+DosHeader.e_lfanew;
			//��������� ��������� PE
			if(mpMemory->Dword(FileHeaderOffset)!=IMAGE_NT_SIGNATURE) continue;
			//������� new-exe ���������
			IMAGE_FILE_HEADER FileHeader;
			FileHeaderOffset+=4;
			mpMemory->Data(&FileHeader,FileHeaderOffset,sizeof(FileHeader));
			//������� ������������ ���������
			IMAGE_OPTIONAL_HEADER OptionalHeader;
			mpMemory->Data(&OptionalHeader,FileHeaderOffset+sizeof(FileHeader),sizeof(OptionalHeader));
			//����������������� ����� �����
			mpCodeParser->ParseEntryPoint(BaseAddress+OptionalHeader.AddressOfEntryPoint);
		}
		catch(CCyberMemoryPageFaultException)
		{
			//�������� ���������� ��� ������� � ������; ������, ���-�� �����������
			//� ����� ������ ���� ���������� �� ������� ������
		}
	}

	//��!
	return FILE_PARSER_ERROR_SUCCESS;
}

CYBER_ADDRESS CDumpFileParser::GetBeginAddress()
{
	//���� ���������� 0
	return 0;
}

VOID CDumpFileParser::About(FILE_PARSER_ABOUT* pAbout)
{
	//������� ��������
	_tcscpy(pAbout->szName,TEXT("Code0 Dump Files Parser"));
	_tcscpy(pAbout->szDescription,TEXT("This analizer can be used to load and analize dump files created with Code0 Process Dump tool."));
	pAbout->Version=MAKELONG(0,1);
}
