#ifndef QS_INSTRUCTION_H
#define QS_INSTRUCTION_H

/*
���� �������� ���������� ������ ������� ����������, ��������� ���������� �� ����� ������������������� ����������.
*/

#include "object.h"
#include "operands.h"
#include "instruction_set.h"
#include "listing.h"
#include <list>

class CInstruction : public CObject
{
protected:
	typedef std::list<COperand*> IOL;							//��� ������ ���������
	typedef IOL::iterator IOLI;										//�������� ������ ���������

	CInstructionSet* mpInstructionSet;						//����� ����������
	IOL mOperandsList;														//������ ���������
	DWORD mPrefixFlags;														//����� ���������
public:
	DWORD mInstructionID;													//������������� ����������

public:
	CInstruction(CInstructionSet* pInstructionSet,DWORD InstructionID,DWORD PrefixFlags);
	virtual ~CInstruction();

	VOID AddOperand(COperand* pOperand);					//�������� ������� � ������
	VOID Print(CListing* pListing);								//���������� ����������
	BOOL GetReference(DWORD* pReference);					//�������� ������ ������
};

#endif
