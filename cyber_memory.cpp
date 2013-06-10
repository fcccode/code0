#include "cyber_memory.h"

// class CCyberMemoryPageFaultException

CCyberMemoryPageFaultException::CCyberMemoryPageFaultException(CYBER_ADDRESS Address)
{
	mAddress=Address;
}

// class CCyberMemory realization

CCyberMemory::CCyberMemory()
{
	//�������� ������ ��� �������� �������
	mpCatalogue=(PVOID*)VirtualAlloc(NULL,MEMORY_PAGES_COUNT*sizeof(PVOID),MEM_RESERVE | MEM_COMMIT,PAGE_READWRITE);
}

CCyberMemory::~CCyberMemory()
{
	//���������� ������ �������� �������
	VirtualFree(mpCatalogue,0,MEM_RELEASE);
}

PVOID CCyberMemory::Translate(CYBER_ADDRESS Address)
{
	//�������� �������� ����� ��������
	PVOID PageRealAddress=mpCatalogue[MEMORY_ADDRESS_GET_PAGE(Address)];
	//���� �������� ����������
	if(PageRealAddress)
		//������� �������� �����
		return ((PBYTE)PageRealAddress+MEMORY_ADDRESS_GET_OFFSET(Address));
	//����� �������� �� ��������, ��������� ����������
	throw CCyberMemoryPageFaultException(Address);
}

//�������� ���� �� �����������
BYTE CCyberMemory::Byte(CYBER_ADDRESS Address)
{
	//�������� ����
	return *(PBYTE)Translate(Address);
}

//�������� ����� �� �����������
WORD CCyberMemory::Word(CYBER_ADDRESS Address)
{
	//�������� ����� ��� 2 �����
	return (*(PBYTE)Translate(Address)) | ((*(PBYTE)Translate(Address+1))<<8);
}

//�������� ������� ����� �� �����������
DWORD CCyberMemory::Dword(CYBER_ADDRESS Address)
{
	//�������� ������� ����� ��� 4 �����
	return (*(PBYTE)Translate(Address)) | ((*(PBYTE)Translate(Address+1))<<8) | ((*(PBYTE)Translate(Address+2))<<16) | ((*(PBYTE)Translate(Address+3))<<24);
}

//������� ������������ ���������� ������
VOID CCyberMemory::Data(PVOID pBuffer,CYBER_ADDRESS Address,DWORD Size)
{
	//���� �� ���������� ������
	for(DWORD i=0;i<Size;++i)
		//������� ���� ������
		((PBYTE)pBuffer)[i]=Byte(Address+i);
}

//������� ASCIIZ-������
LPSTR CCyberMemory::ReadASCIIZ(CYBER_ADDRESS Address)
{
	//��������� ������ ������
	CYBER_ADDRESS BeginAddress=Address;
	//��������� ����� ������
	for(;*(PCHAR)Translate(Address);Address++);
	//�������� ������
	UINT Length=Address-BeginAddress+1;
	LPSTR szString=new CHAR[Length];
	//������� ������
	Data(szString,BeginAddress,Length);
	//������� ������
	return szString;
}

// class CCyberMemoryAccessor

CCyberMemoryAccessor::CCyberMemoryAccessor(CCyberMemory* pMemory)
{
	//��������� ��������� �� �����������������
	mpMemory=pMemory;
	mpMemory->AddRef();
	//���������������� ������� �����
	mAddress=0;
}

CCyberMemoryAccessor::~CCyberMemoryAccessor()
{
	//���������� �����������
	mpMemory->Release();
}

VOID CCyberMemoryAccessor::SetPointer(CYBER_ADDRESS Address)
{
	//���������� ������� �����
	mAddress=Address;
}

CYBER_ADDRESS CCyberMemoryAccessor::GetPointer()
{
	//������� ������� �����
	return mAddress;
}

CCyberMemory* CCyberMemoryAccessor::Memory()
{
	mpMemory->AddRef();
	return mpMemory;
}

//�������� ������� ���� �� �����������
BYTE CCyberMemoryAccessor::CurrentByte()
{
	//������� ������� ����
	BYTE Data=mpMemory->Byte(mAddress);
	//��������� ������� �������� ������
	mAddress++;
	//������� ����
	return Data;
}

//������� ������� ����� �� �����������
WORD CCyberMemoryAccessor::CurrentWord()
{
	//������� ������� �����
	WORD Data=mpMemory->Word(mAddress);
	//��������� ������� �������� ������
	mAddress+=2;
	//������� �����
	return Data;
}

//������� ������� ������� ����� �� �����������
DWORD CCyberMemoryAccessor::CurrentDword()
{
	//������� ������� ������� �����
	DWORD Data=mpMemory->Dword(mAddress);
	//��������� ������� �������� ������
	mAddress+=4;
	//������� ������� �����
	return Data;
}

//������� ������� ������ ��������� �������
DWORD CCyberMemoryAccessor::CurrentData(BYTE Size)
{
	//����� �� ������� ������
	switch(Size)
	{
	case 1:
		return CCyberMemoryAccessor::CurrentByte();
	case 2:
		return CCyberMemoryAccessor::CurrentWord();
	case 4:
		return CCyberMemoryAccessor::CurrentDword();
	default:
		return 0;
	}
}

//������� ������� ������ ��������� ������� � ������ �����
DWORD CCyberMemoryAccessor::CurrentSignedData(BYTE Size)
{
	//����� �� ������� ������
	switch(Size)
	{
	case 1:
		{
			BYTE Data=CurrentByte();
			//���� ������ ������������
			if(Data & 0x80)
				//��������� ������������� � Dword
				return 0-(DWORD)(0x100-Data);
			//����� �������������� �� ���������
			return (DWORD)Data;
		}
	case 2:
		{
			WORD Data=CurrentWord();
			//���� ������ ������������
			if(Data & 0x8000)
				//��������� ������������� � Dword
				return 0-(DWORD)(0x10000-Data);
			//����� �������������� �� ���������
			return (DWORD)Data;
		}
	case 4:
		//������ ������� Dword
		return CurrentDword();
	default:
		return 0;
	}
}

//������������� �������� ������ �� �����������
BOOL CCyberMemory::Map(CYBER_ADDRESS Address,PVOID pBuffer,DWORD Size)
{
	//���� ����� ��� ������ �� ������ ������� ��������, ���������
	if((Address%MEMORY_PAGE_SIZE) || (Size%MEMORY_PAGE_SIZE)) return FALSE;

	//�������� ����� � ���������
	Address/=MEMORY_PAGE_SIZE;
	//�������� ������ � ���������
	Size/=MEMORY_PAGE_SIZE;

	//���������, �� ������� �� ���������� ���� �� ������� �����������������
	if(Address+Size>MEMORY_PAGES_COUNT) return FALSE;

	//��������� �������� ������� ����� ������ �������
	Size+=Address;

	//���������, �������� �� ��� ������
	DWORD i;
	for(i=Address;i<Size;++i)
		//���� ���� ���� �������� ��������, ���������
		if(mpCatalogue[i]) return FALSE;

	//������������� ������
	for(i=Address;i<Size;++i)
		//������������� ��������
		mpCatalogue[i]=(PVOID)((PBYTE)pBuffer+(i-Address)*MEMORY_PAGE_SIZE);

	//��!
	return TRUE;
}

//��������� �������������
BOOL CCyberMemory::Unmap(CYBER_ADDRESS Address,DWORD Size)
{
	//���� ����� ��� ������ �� ������ ������� ��������, ���������
	if((Address%MEMORY_PAGE_SIZE) || (Size%MEMORY_PAGE_SIZE)) return FALSE;

	//�������� ����� � ���������
	Address/=MEMORY_PAGE_SIZE;
	//�������� ������ � ���������
	Size/=MEMORY_PAGE_SIZE;

	//��������� �������� ������� ����� ������ �������
	Size+=Address;

	//��������� ������������� ������
	for(DWORD i=Address;i<Size;++i)
		//��������� ������������� ��������
		mpCatalogue[i]=NULL;

	//��!
	return TRUE;
}

//�������� ����������� ���������
VOID CCyberMemory::Clear()
{
	//��������� ������������� ���� �������
	for(DWORD i=0;i<MEMORY_PAGES_COUNT;++i)
		mpCatalogue[i]=NULL;
}
