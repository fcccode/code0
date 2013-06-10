#ifndef QS_INSTRUCTION_SET_H
#define QS_INSTRUCTION_SET_H

/*
�������� ���������� ������, ��������� ����� ���������� ��� �������.
*/

#include "object.h"
#include "disassembly.h"

//������������ ���������� ����� ����������
#define MAX_INSTRUCTIONS_COUNT												0x1000
//������������ ���������� ������ ����������
#define MAX_INSTRUCTION_BYTES_COUNT										0x10000

class CInstructionSet : public CObject
{
public:
	// ����������

	//����� ����������
	INSTRUCTION_INFORMATION* mInstructions;
	//����� ������ ����������
	INSTRUCTION_BYTE* mInstructionBytes;
	//���������� ����������
	DWORD mInstructionsCount;
	//���������� ���������� ������ ����������
	DWORD mInstructionBytesCount;

public:
	CInstructionSet();
	virtual ~CInstructionSet();

protected:
	// ��������������� �������

	//��������������� ����������
	VOID CorrectInstruction(INSTRUCTION_INFORMATION& II);

public:
	// ������ ��� ���������� ������� ����������

	//� ���������� �� ���������� ����������� �� ��������� ��������� ����; ��� ����� ������������� ���������������
	//��������� ���������� � ����� (������������ ������������� ����������)
	DWORD LoadInstruction_OneByte(INSTRUCTION_INFORMATION II,BYTE Code);
	DWORD LoadInstruction_TwoBytes(INSTRUCTION_INFORMATION II,BYTE Code1,BYTE Code2,BYTE Mask);
	DWORD LoadInstruction_ThreeBytes(INSTRUCTION_INFORMATION II,BYTE Code1,BYTE Code2,BYTE Code3,BYTE Mask1,BYTE Mask2);
	//��������� ����������-�������
	VOID LoadPrefix(BYTE Code,DWORD Flag);
	//������� ��������� ����������� ���������� ����(�)
	VOID SetInstructionFlag(DWORD Flag);

	// ������� ������� ������� � HTML
	VOID PrintHTML(LPCTSTR szFile,DWORD GroupByte=0);
};

#endif
