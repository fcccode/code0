#include "instruction.h"
#include "consts.h"

// class CInstruction

CInstruction::CInstruction(CInstructionSet* pInstructionSet,DWORD InstructionID,DWORD PrefixFlags)
{
	//��������� ����� ����������, ������������� ���������� � ����� ���������
	mpInstructionSet=pInstructionSet;
	mpInstructionSet->AddRef();
	mInstructionID=InstructionID;
	mPrefixFlags=PrefixFlags;
}

CInstruction::~CInstruction()
{
	//�������� ������ ���������
	for(IOLI i=mOperandsList.begin();i!=mOperandsList.end();i++)
		(*i)->Release();
	mOperandsList.clear();

	//���������� ����� ����������
	mpInstructionSet->Release();
}

VOID CInstruction::AddOperand(COperand* pOperand)
{
	//�������� ������� � ������ ���������
	pOperand->AddRef();
	mOperandsList.push_back(pOperand);
}

VOID CInstruction::Print(CListing* pListing)
{
	//��������� ������
	UINT Position=pListing->GetColumnMargin(LISTING_COLUMN_NAME);

	//������ ������
	pListing->BeginLine();

	//���������� ��������
	for(DWORD i=0;i<PREFIXES_COUNT;++i)
		//���� ������� ����, � �� ���������
		if(mPrefixFlags & (1<<i) && PREFIXES_VISIBLE & (1<<i))
		{
			//���������� ��� �������� � ���������
			pListing->Print(Position,gcszPrefixNames[i]);
			Position=pListing->Print(TEXT(":"))+1;
		}

	//���������� ��� �������
	LPCTSTR szName=mpInstructionSet->mInstructions[mInstructionID].szName;
	Position=pListing->Print(Position,szName)+1;

	//�������� ��������� �� ������� ���������
	pListing->SetPosition(pListing->GetColumnMargin(LISTING_COLUMN_OPERANDS));

	//���������� ��������
	for(IOLI i=mOperandsList.begin();i!=mOperandsList.end();i++)
	{
		//���� ���� ������� - �� ���������
		if(i!=mOperandsList.begin())
			//���������� �������
			pListing->Print(TEXT(", "));

		//���������� �������
		(*i)->Print(pListing);
	}

#ifdef NIKITIN
	{
		//���� �����, ����������
		if(mpInstructionSet->mInstructions[mInstructionID].bFlags & IIF_ENDPROC)
			pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_LABEL), TEXT("S"));
		//���� ���� �������, ����������
		DWORD reference;
		if(GetReference(&reference))
		{
			TCHAR s[100];
			_stprintf(s, TEXT("J%08X"), reference);
			pListing->Print(pListing->GetColumnMargin(LISTING_COLUMN_LABEL) + 1, s);
		}
	}
#endif

	//��������� ������
	pListing->EndLine();
}

BOOL CInstruction::GetReference(DWORD* pReference)
{
	//���� �� ���������
	for(IOLI i=mOperandsList.begin();i!=mOperandsList.end();i++)
		//���� ������� ���������� ������
		if((*i)->GetReference(pReference))
			//������� �
			return TRUE;
	//����� ������ ���
	return FALSE;
}
