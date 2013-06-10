#ifndef QS_CYBER_MEMORY_H
#define QS_CYBER_MEMORY_H

// ���� �������� ���������� ������ CCyberMemory, ������������ �������� ������������������.

#include "object.h"

// �������

//������ ����������� �������� �����������
#define MEMORY_PAGE_SIZE														0x1000

//���������� ������� �����������
#define MEMORY_PAGES_COUNT													0x100000

//������ ��� ��������� ������ �������� �� ������
#define MEMORY_ADDRESS_GET_PAGE(Address)						( (Address) / MEMORY_PAGE_SIZE )
//������ ��� ��������� �������� ������ ��������
#define MEMORY_ADDRESS_GET_OFFSET(Address)					( (Address) % MEMORY_PAGE_SIZE )

//������� ����������
#define ALIGN_UP(Address,Align) (((Address)+(Align)-1)/(Align)*(Align))
#define ALIGN_DOWN(Address,Align) ((Address)/(Align)*(Align))

// ����

typedef DWORD CYBER_ADDRESS;

// ������

class CCyberMemoryPageFaultException
{
public:
	CYBER_ADDRESS mAddress;

	CCyberMemoryPageFaultException(CYBER_ADDRESS Address);
};

class CCyberMemory : public CObject
{
protected:
	//��������� �� ������� �������
	PVOID* mpCatalogue;

public:
	CCyberMemory();
	virtual ~CCyberMemory();

	//�������� �������� ����� �� �����������
	PVOID Translate(CYBER_ADDRESS Address);

	//�������� ����
	BYTE Byte(CYBER_ADDRESS Address);
	//�������� �����
	WORD Word(CYBER_ADDRESS Address);
	//�������� ������� �����
	DWORD Dword(CYBER_ADDRESS Address);
	//������� ������������ ���������� ������
	VOID Data(PVOID pBuffer,CYBER_ADDRESS Address,DWORD Size);
	//������� ASCIIZ-������ (������ ������ ���� ����������� ���������� �������� � ������� delete [])
	LPSTR ReadASCIIZ(CYBER_ADDRESS Address);

	//������������� �������� ������ �� �����������
	BOOL Map(CYBER_ADDRESS Address,PVOID pBuffer,DWORD Size);
	//��������� ������������� ������
	BOOL Unmap(CYBER_ADDRESS Address,DWORD Size);
	//�������� �����������, ������� ��� ��������
	VOID Clear();
};

class CCyberMemoryAccessor : public CObject
{
protected:
	//��������� �� �����������������
	CCyberMemory* mpMemory;
	//������� �����
	CYBER_ADDRESS mAddress;

public:
	CCyberMemoryAccessor(CCyberMemory* pMemory);
	virtual ~CCyberMemoryAccessor();

	//���������� ���������� �������� �����
	VOID SetPointer(CYBER_ADDRESS Address);
	CYBER_ADDRESS GetPointer();

	//��������� ������� ��������� �� �����������������
	CCyberMemory* Memory();

	//�������� ������� ����
	BYTE CurrentByte();
	//�������� ������� �����
	WORD CurrentWord();
	//�������� ������� ������� �����
	DWORD CurrentDword();
	//�������� ������� ������
	DWORD CurrentData(BYTE Size);
	//�������� ������� ������ � ������ �����
	DWORD CurrentSignedData(BYTE Size);
};

#endif
