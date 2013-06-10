#include "pe_parser.h"
#include "dialog_peparsersettings.h"
#include "application.h"
#include "image_extend_object.h"
#include "image_list_imports.h"
#include "image_list_exports.h"
#include <bitset>
#include <winnt.h>

// class CPEParser

BOOL CPEParser::IsAskSettingsAvaliable()
{
	return TRUE;
}

BOOL CPEParser::AskSettings(HWND hWndParent)
{
	//��������� ������
	CDialogPEParserSettings Dialog;
	return Dialog.Show(hWndParent,&mSettings);
}

BOOL CPEParser::PrepareAnalize()
{
	//�������� ��������� �����
	if(!RetrieveHeaders()) return FALSE;

	//���� ���������� �������� �������, ����� �������,
	//��� ���� ������������� ������� PE.

	//������� ��������� ������ ������ �����
	mFileBufferSize=mpImageOptionalHeader->SizeOfImage;

	//������� ��������� ����������� �� ���������
	//������� ����� ��������
	mSettings.PreferredBaseAddress=mpImageOptionalHeader->ImageBase;
	mSettings.BaseAddress=mSettings.PreferredBaseAddress;
	//�����: ������������ ���������������� ����� ��������, ����������������� �������
	mSettings.Flags=PEPSF_USE_PREFERRED_BASE_ADDRESS | PEPSF_ANALIZE_EXPORTS_CODE;

	return TRUE;
}

DWORD CPEParser::Analize()
{
	// ��������� ������ ������ ���� ��� �������� ������� PrepareAnalize,
	// � ��������� ����� ��������

	//��������������� ���������
	// ���� ������������ ���������������� ������� ����� ��������, �� ��������� ���
	if(mSettings.Flags & PEPSF_USE_PREFERRED_BASE_ADDRESS) mSettings.BaseAddress=mSettings.PreferredBaseAddress;

	//������������� ���� � �����������
	if(!MapToCyberMemory()) return FILE_PARSER_ERROR_LOADING;
	//���������������� ������� �����
	AnalizeGeneral();
	AnalizeImport();
	AnalizeExport();
	//����������������� ���
	if(!ParseCode()) return FILE_PARSER_ERROR_ANALIZING;

	return FILE_PARSER_ERROR_SUCCESS;
}

BOOL CPEParser::RetrieveHeaders()
{
	//��������� ������������� ������� ��� DOS-���������
	if(mFileSize < sizeof(IMAGE_DOS_HEADER)) return FALSE;
	//�������� DOS-���������
	mpImageDosHeader=(PIMAGE_DOS_HEADER)mpFileMapping;
	//��������� ��������� MZ
	if(mpImageDosHeader->e_magic!=IMAGE_DOS_SIGNATURE) return FALSE;

	//��������� ������������ �������� �� PE-���������
	if(mFileSize < mpImageDosHeader->e_lfanew+4+sizeof(IMAGE_FILE_HEADER)) return FALSE;
	//��������� ��������� PE
	if(*(DWORD*)((PBYTE)mpFileMapping+mpImageDosHeader->e_lfanew)!=IMAGE_NT_SIGNATURE) return FALSE;
	//�������� PE-���������
	mpImageFileHeader=(PIMAGE_FILE_HEADER)((PBYTE)mpFileMapping+mpImageDosHeader->e_lfanew+4);

	//��������� ������������ ���������� ������� ������������� ���������
	if(mpImageFileHeader->SizeOfOptionalHeader < sizeof(IMAGE_OPTIONAL_HEADER)) return FALSE;
	//��������� ������������� ������� ��� ������������� ���������
	if(mFileSize < ( (PBYTE)mpImageFileHeader-(PBYTE)mpFileMapping+sizeof(IMAGE_FILE_HEADER)+mpImageFileHeader->SizeOfOptionalHeader )) return FALSE;
	//�������� ������������ ���������
	mpImageOptionalHeader=(PIMAGE_OPTIONAL_HEADER)(mpImageFileHeader+1);

	//��������� ������������� ������� ��� ���� ���������� � ������� ������
	if(mFileSize < (
		(PBYTE)mpImageOptionalHeader-(PBYTE)mpFileMapping
		+ mpImageFileHeader->SizeOfOptionalHeader
		+ mpImageFileHeader->NumberOfSections*sizeof(IMAGE_SECTION_HEADER)
		) ) return FALSE;

	//��������� ������������ ���������� ������� ����������
	if(mpImageOptionalHeader->SizeOfHeaders < ( (PBYTE)mpImageOptionalHeader-(PBYTE)mpFileMapping+mpImageFileHeader->SizeOfOptionalHeader+mpImageFileHeader->NumberOfSections*sizeof(IMAGE_SECTION_HEADER) ) ) return FALSE;

	//�������� ��������� �� ������� ������
	mpImageSections=(PIMAGE_SECTION_HEADER)((PBYTE)mpImageOptionalHeader+mpImageFileHeader->SizeOfOptionalHeader);

	//�������� ��������� �� ��������� ������
//	PIMAGE_SECTION_HEADER pLastHeader=mpImageSections+mpImageFileHeader->NumberOfSections-1;
	//��������� ������������ ���������� ������� ������
//	if(mpImageOptionalHeader->SizeOfImage > ALIGN_UP(pLastHeader->VirtualAddress+pLastHeader->Misc.VirtualSize,0x1000)) return FALSE;

	//����� ��.
	return TRUE;
}

BOOL CPEParser::MapToCyberMemory()
{
	//������������� ����� � �����������
	if(!mpMemory->Map(mSettings.BaseAddress,mpFileBuffer,ALIGN_UP(mpImageOptionalHeader->SizeOfImage,MEMORY_PAGE_SIZE))) return FALSE;

	//����������� ���������
	CopyMemory(mpFileBuffer,mpFileMapping,mpImageOptionalHeader->SizeOfHeaders);

	//�������� ���������� ������
	DWORD SectionsCount=mpImageFileHeader->NumberOfSections;
	//�������� �������� ������������
//	DWORD FileAlignment=mpImageOptionalHeader->FileAlignment;
	DWORD SectionAlignment=mpImageOptionalHeader->SectionAlignment;
	//���� �� ������� �����
	for(DWORD i=0;i<SectionsCount;++i)
	{
		//�������� ��������� ������
		PIMAGE_SECTION_HEADER pSection=mpImageSections+i;

		//�������� ���������� � ����������� ������� ������
		DWORD PhysicalSize=pSection->SizeOfRawData;
		DWORD VirtualSize=pSection->Misc.VirtualSize;

		//���� ����������� ������ ����� 0
		if(!VirtualSize)
			//������������ � ��� �������� ����������
			VirtualSize=ALIGN_UP(PhysicalSize,SectionAlignment);
		//����� ���� ����������� ������ ������ �����������
		else if(VirtualSize<PhysicalSize && !VirtualSize)
			//� �������� ����������� ������� ������������ �����������
			PhysicalSize=VirtualSize;

		//��������� ������������ ����������� �������
		if(mFileSize < pSection->PointerToRawData+PhysicalSize) return FALSE;

		//����������� ������������������ ������ ������
		CopyMemory((PBYTE)mpFileBuffer+pSection->VirtualAddress,(PBYTE)mpFileMapping+pSection->PointerToRawData,PhysicalSize);
	}

	//��!
	return TRUE;
}

BOOL CPEParser::AnalizeGeneral()
{
	//�������� ����� �� ����� �����
	mpAddressMap->SetLabel(mSettings.BaseAddress+mpImageOptionalHeader->AddressOfEntryPoint,TEXT("EntryPoint"));
	//������� � ����� ����� �������
	{
		CAddressData* pAddressData=mpAddressMap->OpenAddress(mSettings.BaseAddress+mpImageOptionalHeader->AddressOfEntryPoint);
		pAddressData->CreateSubroutine();
		pAddressData->Release();
	}
	//�������� ����� �� DOS-��������
	mpAddressMap->SetLabel(mSettings.BaseAddress+sizeof(IMAGE_DOS_HEADER),TEXT("DosStub"));
	//������� � DOS-�������� �������
	{
		CAddressData* pAddressData=mpAddressMap->OpenAddress(mSettings.BaseAddress+sizeof(IMAGE_DOS_HEADER));
		pAddressData->CreateSubroutine();
		pAddressData->Release();
	}
	return TRUE;
}

BOOL CPEParser::AnalizeImport()
{
	//�������� ���������� �������
	PIMAGE_DATA_DIRECTORY pDataDirectory=mpImageOptionalHeader->DataDirectory+IMAGE_DIRECTORY_ENTRY_IMPORT;
	//���� ������� ���, ���������
	if(!pDataDirectory->VirtualAddress || !pDataDirectory->Size) return TRUE;
	//������� ���������� �������
	CYBER_ADDRESS DescriptorAddress=mSettings.BaseAddress+pDataDirectory->VirtualAddress;
	IMAGE_IMPORT_DESCRIPTOR Descriptor;
	try
	{
		mpMemory->Data(&Descriptor,DescriptorAddress,sizeof(IMAGE_IMPORT_DESCRIPTOR));
	}
	catch(CCyberMemoryPageFaultException)
	{
		goto error;
	}

	//������� ������ �������
	CImageListImports* pImportsList=new CImageListImports;
	//�������� ��� � ������ �������������� ��������
	mpExtendMap->insert(std::make_pair(IMAGE_EXTEND_LIST_IMPORTS,pImportsList));
	//�������� ������ ���������
	std::list<CImageListImports::CImportLibrary*>* pLibrariesList=pImportsList->GetList();

	//���� �� ������������� �����������
	while(Descriptor.Name)
	{
		//�������� ������� ������� (IAT)
		CYBER_ADDRESS IATAddress=mSettings.BaseAddress+Descriptor.FirstThunk;

		//������������� ����������
		CImageListImports::CImportLibrary* pImportLibrary;
		{
			//������� ��� (� ASCII)
			LPSTR szName;
			try
			{
				szName=mpMemory->ReadASCIIZ(mSettings.BaseAddress+Descriptor.Name);
			}
			catch(CCyberMemoryPageFaultException)
			{
				goto error;
			}
#ifdef UNICODE
			//������������� ������ � Unicode
			UINT Length=strlen(szName)+1;
			LPTSTR szUnicodeName=new TCHAR[Length];
			MultiByteToWideChar(CP_ACP,0,szName,-1,szUnicodeName,Length);
			//������� ������������� ����������
			pImportLibrary=new CImageListImports::CImportLibrary(szUnicodeName,IATAddress);
			SafeDeleteMassive(szUnicodeName);
#else
			//������� ������������� ����������
			pImportLibrary=new CImageListImports::CImportLibrary(szName,IATAddress);
#endif
			SafeDeleteMassive(szName);
		}
		//�������� ���������� � ������ ���������
		pLibrariesList->push_back(pImportLibrary);
		//�������� ������ �������
		std::list<CImageListImports::CImportFunction*>* pFunctionsList=pImportLibrary->GetList();

		//�������� ������ ������������� �������
		CYBER_ADDRESS ThunkAddress=mSettings.BaseAddress+Descriptor.OriginalFirstThunk;
		DWORD Thunk;
		try
		{
			Thunk=mpMemory->Dword(ThunkAddress);
		}
		catch(CCyberMemoryPageFaultException)
		{
			goto error;
		}
		//������� �������
		UINT i=0;
		//���� �� ��������
		while(Thunk)
		{
			//���� ������� ������������� �� ������
			if(Thunk & 0x80000000)
			{
			}
			//����� ������� ������������� �� �����
			else
			{
				//������� ��� ������������� ������� (� ������ ������� ������������ ASCII)
				LPSTR szName;
				try
				{
					szName=mpMemory->ReadASCIIZ(mSettings.BaseAddress+Thunk+2);
				}
				catch(CCyberMemoryPageFaultException)
				{
					goto error;
				}
#ifdef UNICODE
				//������������� ������ � Unicode
				UINT Length=strlen(szName)+1;
				LPTSTR szUnicodeName=new TCHAR[Length];
				MultiByteToWideChar(CP_ACP,0,szName,-1,szUnicodeName,Length);
				//��������� ��� ��� �����
				LPTSTR szLabel=new TCHAR[Length+6];
				_stprintf(szLabel,TEXT("__imp_%s"),szUnicodeName);
				SafeDeleteMassive(szName);
#else
				//��������� ��� ��� �����
				LPTSTR szLabel=new TCHAR[Length+6];
				_stprintf(szLabel,TEXT("__imp_%s"),szName);
#endif
				//���������� ��� ������������� ��� ������
				CAddressData* pAddressData=mpAddressMap->OpenAddress(IATAddress+i*sizeof(DWORD));
				CInterpretation* pInterpretation=new CInterpretationData(mpMemory,IATAddress+i*sizeof(DWORD),4);
				pAddressData->SetInterpretation(pInterpretation);
				mpAddressMap->SetAddressSize(IATAddress+i*sizeof(DWORD),4);
				pInterpretation->Release();
				//������� �����
				CLabel* pLabel=new CLabel(szLabel);
				SafeDeleteMassive(szLabel);
				pAddressData->SetLabel(pLabel);
				pLabel->Release();
				pAddressData->Release();

				//�������� ������������� ������� � ������ �������
#ifdef UNICODE
				pFunctionsList->push_back(new CImageListImports::CImportFunction(szUnicodeName,IATAddress+i*sizeof(DWORD)));
				SafeDeleteMassive(szUnicodeName);
#else
				pFunctionsList->push_back(new CImageListImports::CImportFunction(szName,IATAddress+i*sizeof(DWORD)));
				SafeDeleteMassive(szName);
#endif
			}

			//��������� ������� �������
			i++;
			//������� � ��������� �������
			ThunkAddress+=sizeof(Thunk);
			try
			{
				Thunk=mpMemory->Dword(ThunkAddress);
			}
			catch(CCyberMemoryPageFaultException)
			{
				goto error;
			}
		}

		//������� � ��������� ����������
		DescriptorAddress+=sizeof(IMAGE_IMPORT_DESCRIPTOR);
		try
		{
			mpMemory->Data(&Descriptor,DescriptorAddress,sizeof(IMAGE_IMPORT_DESCRIPTOR));
		}
		catch(CCyberMemoryPageFaultException)
		{
			goto error;
		}
	}

	//������ �������� �������
	return TRUE;

	//������
error:
	//������ ������� �������
	MessageBox(NULL,TEXT("The import section in this file is non-readable.\nPE Parser coudn't parse import."),TEXT("Import parsing"),MB_ICONSTOP);

	return FALSE;
}

BOOL CPEParser::AnalizeExport()
{
	//����� ������� ����� � ������� (������������ ���������������, ����� �� ���� warning'� ��-�� goto).
	static std::bitset<0x10000> NamesFlags;
	NamesFlags.reset();

	//�������� ���������� ��������
	PIMAGE_DATA_DIRECTORY pDataDirectory=mpImageOptionalHeader->DataDirectory+IMAGE_DIRECTORY_ENTRY_EXPORT;
	//���� �������� ���, ���������
	if(!pDataDirectory->VirtualAddress || pDataDirectory->Size<sizeof(IMAGE_EXPORT_DIRECTORY)) return TRUE;
	//������� ���������� ��������
	CYBER_ADDRESS DescriptorAddress=mSettings.BaseAddress+pDataDirectory->VirtualAddress;
	IMAGE_EXPORT_DIRECTORY Descriptor;
	try
	{
		mpMemory->Data(&Descriptor,DescriptorAddress,sizeof(IMAGE_EXPORT_DIRECTORY));
	}
	catch(CCyberMemoryPageFaultException)
	{
		goto error;
	}

	//������� ������ ��������
	CImageListExports* pExportsList=new CImageListExports;
	//�������� ��� � ������ �������������� ��������
	mpExtendMap->insert(std::make_pair(IMAGE_EXTEND_LIST_EXPORTS,pExportsList));
	//�������� ������ �������
	std::list<CImageListExports::CExportFunction*>* pFunctionsList=pExportsList->GetList();

	//�������� ���������� ������� � ����
	UINT FunctionsCount=Descriptor.NumberOfFunctions;
	if(FunctionsCount>0x10000) goto error;
	UINT NamesCount=Descriptor.NumberOfNames;
	if(NamesCount>0x10000) goto error;
	//�������� ������� ����, ������� � ���������
	CYBER_ADDRESS AddressTable=mSettings.BaseAddress+Descriptor.AddressOfFunctions;
	CYBER_ADDRESS NamesTable=mSettings.BaseAddress+Descriptor.AddressOfNames;
	CYBER_ADDRESS OrdinalsTable=mSettings.BaseAddress+Descriptor.AddressOfNameOrdinals;

	//���� �� ������
	for(UINT i=0;i<NamesCount;++i)
	{
		//������� ���, �������, ����� � ������� �������
		LPSTR szName;
		LPSTR szForward;
		WORD Ordinal;
		CYBER_ADDRESS Address;
		//���� ��������
		BOOL IsForward=FALSE;
		try
		{
			//������� ���
			szName=mpMemory->ReadASCIIZ(mSettings.BaseAddress+mpMemory->Dword(NamesTable+i*sizeof(DWORD)));
			//������� �������
			Ordinal=mpMemory->Word(OrdinalsTable+i*sizeof(WORD));
			//������� �����
			Address=mpMemory->Dword(AddressTable+Ordinal*sizeof(DWORD));

			//���� ����� ��������� � �������� ������� ��������, ������, ��� �������
			if(Address>=pDataDirectory->VirtualAddress && Address<pDataDirectory->VirtualAddress+pDataDirectory->Size)
				IsForward=TRUE;
			Address+=mSettings.BaseAddress;

			if(IsForward)
				//������� ��� �������-�������
				szForward=mpMemory->ReadASCIIZ(Address);
		}
		catch(CCyberMemoryPageFaultException)
		{
			goto error;
		}

		//������� ���� ������� �����
		NamesFlags[Ordinal]=true;

#ifdef UNICODE
		//������������� ������ � Unicode
		UINT NameLength=strlen(szName)+1;
		LPTSTR szUnicodeName=new TCHAR[NameLength];
		MultiByteToWideChar(CP_ACP,0,szName,-1,szUnicodeName,NameLength);
		SafeDeleteMassive(szName);

		UINT ForwardLength;
		LPTSTR szUnicodeForward;
		if(IsForward)
		{
			ForwardLength=strlen(szForward)+1;
			szUnicodeForward=new TCHAR[ForwardLength];
			MultiByteToWideChar(CP_ACP,0,szForward,-1,szUnicodeForward,ForwardLength);
			SafeDeleteMassive(szForward);
		}
#define szName szUnicodeName
#define szForward szUnicodeForward
#endif

		//�������� ������� � ������
		if(IsForward)
			pFunctionsList->push_back(new CImageListExports::CExportFunction(szName,Ordinal,szForward));
		else
			pFunctionsList->push_back(new CImageListExports::CExportFunction(szName,Ordinal,Address));

		//���� ������� ���������
		if(!IsForward)
		{
			//������������ ��� � ���������� �����
			LPTSTR szLabelName=new TCHAR[NameLength+6];
			_stprintf(szLabelName,TEXT("__exp_%s"),szName);
			CAddressData* pAddressData=mpAddressMap->OpenAddress(Address);
			CLabel* pLabel=new CLabel(szLabelName);
			SafeDeleteMassive(szLabelName);
			pAddressData->SetLabel(pLabel);
			pLabel->Release();
			//������� �������
			pAddressData->CreateSubroutine();
			pAddressData->Release();
		}

		//���������� ������
		SafeDeleteMassive(szName);
		if(IsForward)
			SafeDeleteMassive(szForward);

#ifdef UNICODE
#undef szName
#undef szForward
#endif
	}

	//���� �� ��������
	for(UINT i=0;i<FunctionsCount;++i)
	{
		//���� ������� ����� ���, ������, ��� ��� ����������
		if(NamesFlags[i]) continue;

		//������� �����
		CYBER_ADDRESS Address;
		try
		{
			Address=mSettings.BaseAddress+mpMemory->Dword(AddressTable+i*sizeof(DWORD));
		}
		catch(CCyberMemoryPageFaultException)
		{
			goto error;
		}

		//�������� ������� � ������
		pFunctionsList->push_back(new CImageListExports::CExportFunction(NULL,i,Address));
	}

	return TRUE;

	//������
error:
	//������ �������� �������
	MessageBox(NULL,TEXT("The export section in this file is non-readable.\nPE Parser coudn't parse export."),TEXT("Export parsing"),MB_ICONSTOP);

	return FALSE;
}

BOOL CPEParser::ParseCode()
{
	//������� ���������� ����
	mpCodeParser=new CCodeParser(mpMemory,mpAddressMap,&gApplication.mInstructionSet16,&gApplication.mInstructionSet32);

	//���� ����������, ����������������� DOS-��������
	if(mSettings.Flags & PEPSF_ANALIZE_DOS_STUB)
	{
		//���������� 16-������ �����
		mpCodeParser->SetMode(MODE_16BIT);
		//��������� ������ ���� DOS-��������
		mpCodeParser->ParseEntryPoint(mSettings.BaseAddress+sizeof(IMAGE_DOS_HEADER));
	}
	//���������� 32-������ �����
	mpCodeParser->SetMode(MODE_32BIT);
	//��������� ������ ����, ������� � ����� �����
	mpCodeParser->ParseEntryPoint(mSettings.BaseAddress+mpImageOptionalHeader->AddressOfEntryPoint);

	//���� ����������, ����������������� �������
	if(mSettings.Flags & PEPSF_ANALIZE_EXPORTS_CODE)
	{
		//�������� ������ ��������, �������������� �� ������ AnalizeImport
		std::map<UINT,CObject*>::iterator ListIterator=mpExtendMap->find(IMAGE_EXTEND_LIST_EXPORTS);
		if(ListIterator!=mpExtendMap->end())
		{
			std::list<CImageListExports::CExportFunction*>* pExportsList=((CImageListExports*)(*ListIterator).second)->GetList();
			//���������� 32-������ �����
			mpCodeParser->SetMode(MODE_32BIT);
			//���� �� ��������
			for(std::list<CImageListExports::CExportFunction*>::iterator i=pExportsList->begin();i!=pExportsList->end();i++)
			{
				//�������� �������
				CImageListExports::CExportFunction* pFunction=*i;
				//���� ������� ���������
				if(pFunction->IsLocal())
				{
					//����������������� �������
					mpCodeParser->ParseEntryPoint(pFunction->GetAddress());
				}
			}
		}
	}

	//��!
	return TRUE;
}

CYBER_ADDRESS CPEParser::GetBeginAddress()
{
	//������� ����� �����
	return mSettings.BaseAddress+mpImageOptionalHeader->AddressOfEntryPoint;
}

VOID CPEParser::About(FILE_PARSER_ABOUT* pAbout)
{
	//������� ��������
	_tcscpy(pAbout->szName,TEXT("PE Win32 Files Parser"));
	_tcscpy(pAbout->szDescription,TEXT("This parser perform initial analize for standard Microsoft 32-bit PE files (usually \"*.exe\" or \"*.dll\" name extension)."));
	pAbout->Version=MAKELONG(0,1);
}
