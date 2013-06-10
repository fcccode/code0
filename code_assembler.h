#ifndef QS_CODE_ASSEMBLER_H
#define QS_CODE_ASSEMBLER_H

/*
� ����� ���������� ����������� ������ ���������� ����.
*/

#include "object.h"
#include "instruction_set.h"
#include "instruction.h"
#include <list>

class CCodeAssembler : public CObject
{
public:
	//����� �������� ���������������� ����������
	class CVariant
	{
	public:
		//��� � ��� �����
		PBYTE mpCode;
		UINT mCodeSize;
		//����������
		CInstruction* mpInstruction;

	public:
		//����������� � ����������
		CVariant();
		~CVariant();
	};

protected:
	//����� ����������
	CInstructionSet* mpInstructionSet;
	//������ ���������, ������������ ��� ���������������
	std::list<CVariant*>* mpList;

public:
	//����������� � ����������
	CCodeAssembler(CInstructionSet* pInstructionSet);
	~CCodeAssembler();

	//�������������� ���������� � ������ �������� ����
	//������������ �������� ���������� ������������ ������
	BOOL Assemble(LPCTSTR szCode,std::list<CVariant*>* pList);
};

#endif
