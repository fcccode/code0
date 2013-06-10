#ifndef QS_CODE_PARSER_H
#define QS_CODE_PARSER_H

/*
���� �������� ����������� ������ CCodeParser, ������������� ������������������� ����.
*/

#include "object.h"
#include "cyber_memory.h"
#include "address_map.h"
#include "instruction_set.h"
#include "instruction.h"
#include <set>

#define MODE_16BIT					0							//16-������ �����
#define MODE_32BIT					1							//32-������ �����

//���������, ������������ ��� �������� ���������� �������
struct MODE_STRUCT
{
	CInstructionSet* pInstructionSet;				//����� ����������
	BYTE NormalAddressOperandSize;					//������ "��������" �������� ��� ���������� ��������
	BYTE RedefinedAddressOperandSize;				//������ "��������" �������� ��� ������� �������� ��������������� ������� ������
};

class CCodeParser : public CObject										//�����, ������������ ������������������ ����
{
protected:
	CCyberMemoryAccessor* mpMemory;											//�������� ����������������� ������
	CAddressMap* mpAddressMap;													//����� ���������� �������
	MODE_STRUCT mModes[2];															//��������� �������

	DWORD mMode;																				//������� ����� (�.�. ����� ����������, �� ��������� �� ��������� ��������������� �����������)
	BYTE mAddressOperandSize;														//������ "��������" �������� � ������ ������ (2 ��� 4 �����)
	MODE_STRUCT* mpCurrentMode;													//��������� �������� ������ (� ��� ����� ������� ����������)
	DWORD mPrefixFlags;																	//����� ��������� ������� ������������ ����������
	CYBER_ADDRESS mInstructionAddress;									//����� ������ ������� ����������
	CYBER_ADDRESS mSubroutineAddress;										//����� ������ �������
	std::set<std::pair<CYBER_ADDRESS,CYBER_ADDRESS> > mParsingQueue;	//������� ������������������ (������ ����� - �����, ������ ����� - ������ �������)

public:
	CCodeParser(CCyberMemory* pMemory,CAddressMap* pAddressMap,CInstructionSet* pSet16,CInstructionSet* pSet32);	//�����������
	virtual ~CCodeParser();

	VOID SetMode(DWORD Mode);														//���������� ������� �����
	VOID ParseEntryPoint(CYBER_ADDRESS EntryPoint);			//������ ������������������ � ��������� ����� �����

protected:
	//�������� ������� �����
	VOID UpdateMode();
	//������������ ���� ModRM
	VOID ParseModRM(MODRM* pModRM);
	//���������� ������������ ��������� ���� ����������
	DWORD ParseInstructionByte(DWORD Group,BYTE Mask);
	//������������ ����������
	CInstruction* ParseInstruction(CYBER_ADDRESS Address);
	//�������� ������������ ������
	VOID AddXReference(CYBER_ADDRESS AddressFrom,CYBER_ADDRESS AddressTo,BYTE ReferenceFlags,BYTE Size=0);
};

#endif
