#include "code_assembler.h"
#include "strings.h"

// class CCodeAssembler

CCodeAssembler::CCodeAssembler(CInstructionSet* pInstructionSet)
{
	//��������� ����� ����������
	mpInstructionSet=pInstructionSet;
	mpInstructionSet->AddRef();
}

CCodeAssembler::~CCodeAssembler()
{
	SafeRelease(mpInstructionSet);
}

BOOL CCodeAssembler::Assemble(LPCTSTR szCode,std::list<CCodeAssembler::CVariant*>* pList)
{
	//��������� ������
	mpList=pList;

	//����������� ������ ����
	UINT Length=_tcslen(szCode);
	LPTSTR szLine=new TCHAR[Length];
	_tcscpy(szLine,szCode);
	//�������� ��� ���������� (������, ������ �����)
	UINT i;
	for(i=0;i<Length;++i)
		if(!_istspace(szLine[i]))
			break;
	LPCTSTR szFilter=szLine+i;
	for(i++;i<Length;++i)
		if(_istspace(szLine[i]))
			break;
	szLine[i]=0;

	//����� ���������� � ���������� ������
	UINT InstructionsCount=mpInstructionSet->mInstructionsCount;
	for(i=0;i<InstructionsCount;++i)
	{
		//�������� ����������
		INSTRUCTION_INFORMATION II=mpInstructionSet->mInstructions[i];
		//���� ��� ��������
		if(IsAgreeFilterI(II.szName,szFilter))
		{
		}
	}

	delete [] szLine;

	return TRUE;
}
