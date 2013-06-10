#include "operands.h"
#include "consts.h"

// class COperand

BOOL COperand::GetReference(DWORD* pReference)
{
	//������ ���, ��� ��������
	return FALSE;
}

// class COperandData

COperandData::COperandData(DWORD Data)
{
	//��������� �������� ���������
	mData=Data;
	//�������� ����� ������
	mReferenceFlags=0;
}

DWORD COperandData::GetType()
{
	return OT_DATA;
}

VOID COperandData::Print(CListing* pListing)
{
	//���� ��������� - ������
	if(mReferenceFlags & (OIR_ABSOLUTE_POINTER | OIR_RELATIVE_POINTER))
		//���������� �������� ������
		pListing->PrintOffset(mData);
	else
	{
		//���������� �������� ���������
		TCHAR s[0x10];
		_stprintf(s,(int)mData>=0 ? TEXT("0%+Xh") : TEXT("0%-Xh"),(int)mData>0 ? mData : -(int)mData);
		pListing->Print(s);
	}
}

BOOL COperandData::GetReference(DWORD* pReference)
{
	//���� ��������� - ������
#ifdef NIKITIN
	if((mReferenceFlags & (OIR_ABSOLUTE_POINTER | OIR_RELATIVE_POINTER)) && (mReferenceFlags & OIR_REFERENCE_EXECUTE) && !(mReferenceFlags & OIR_REFERENCE_EXECUTE_CALL))
#else
	if(mReferenceFlags & (OIR_ABSOLUTE_POINTER | OIR_RELATIVE_POINTER))
#endif
	{
		//������� ������
		*pReference=mData;
		return TRUE;
	}
	//����� ������ ���
	return FALSE;
}

// class COperandRegister

COperandRegister::COperandRegister(BYTE Register)
{
	//��������� ����� �������� � ��������� (��������� ���������� �������������)
	mRegister=Register;
}

DWORD COperandRegister::GetType()
{
	return OT_REGISTER;
}

VOID COperandRegister::Print(CListing* pListing)
{
	//���������� ��� ��������
	pListing->Print(gcszRegisterNames[mRegister]);
}

// class COperandMemory

COperandMemory::COperandMemory(DWORD Address)
{
	//���������������� �������-����������
	mFlags=OMF_OFFSET;
	mBaseRegister=0;
	mIndexRegister=0;
	mMultiplier=0;
	mOffset=Address;
	mSegment=0;
}

COperandMemory::COperandMemory(MODRM ModRM)
{
	//���������������� �������-����������
	mFlags=ModRM.RM.bFlags & (OMF_BASE_REGISTER | OMF_INDEX_REGISTER | OMF_MULTIPLIER | OMF_OFFSET);
	mBaseRegister=(mFlags & OMF_BASE_REGISTER) ? ModRM.RM.bBaseRegister : 0;
	mIndexRegister=(mFlags & OMF_INDEX_REGISTER) ? ModRM.RM.bIndexRegister : 0;
	mMultiplier=(mFlags & OMF_MULTIPLIER) ? ModRM.RM.bMultiplier : 0;
	mOffset=(mFlags & OMF_OFFSET) ? ModRM.RM.dwOffset : 0;
	mSegment=0;
}

VOID COperandMemory::SetSegment(BYTE Segment)
{
	//�������� ���� ������� ����������� ��������
	mFlags|=OMF_SEGMENT;
	//��������� ���������� �������
	mSegment=Segment;
}

DWORD COperandMemory::GetType()
{
	return OT_MEMORY;
}

VOID COperandMemory::Print(CListing* pListing)
{
	//���������� ������� �������
	switch(mSize)
	{
	case 1:
		pListing->Print(TEXT("byte ptr "));
		break;
	case 2:
		pListing->Print(TEXT("word ptr "));
		break;
	case 4:
		pListing->Print(TEXT("dword ptr "));
		break;
	case 6:
		pListing->Print(TEXT("tword ptr "));
		break;
	case 8:
		pListing->Print(TEXT("qword ptr "));
		break;
	case 10:
		pListing->Print(TEXT("fword ptr "));
		break;
	default:
		pListing->Print(TEXT("invalid ptr "));
		break;
	}

	//���������� ���������� �������, ���� ����
	if(mFlags & OMF_SEGMENT)
	{
		pListing->Print(gcszRegisterNames[REGISTER_ID(RC_SEGMENT,mSegment)]);
		pListing->Print(TEXT(":"));
	}
	//���������� ������ [
	pListing->Print(TEXT("["));

	//���� ������������� ������ ����� +
	BOOL bPlus=FALSE;
	//���������� ������� �������, ���� �� ����
	if(mFlags & OMF_BASE_REGISTER)
	{
		pListing->Print(gcszRegisterNames[mBaseRegister]);
		bPlus=TRUE;
	}
	//���������� ��������� �������, ���� �� ����
	if(mFlags & OMF_INDEX_REGISTER)
	{
		if(bPlus)
			pListing->Print(TEXT("+"));
		else
			bPlus=TRUE;
		pListing->Print(gcszRegisterNames[mIndexRegister]);
	}
	//���������� ���������, ���� �� ����
	if(mFlags & OMF_MULTIPLIER)
	{
		TCHAR s[3];
		s[0]=TEXT('*');
		s[1]=mMultiplier+TEXT('0');
		s[2]=0;
		pListing->Print(s);
		bPlus=TRUE;
	}
	//���������� ��������, ���� ��� ����
	if(mFlags & OMF_OFFSET)
	{
		//�������� �������� � ���� ������, ������ ���� ������ ������ ���
		if(mFlags==OMF_OFFSET)
		{
			if(bPlus)
				pListing->Print(TEXT("+"));
			else
				bPlus=TRUE;
			pListing->PrintOffset(mOffset);
		}
		else
		{
			TCHAR s[0x10];
			//���� �������� �������������
			if(mOffset & 0x80000000)
			{
				//���������� � �������
				_stprintf(s,TEXT("-0%Xh"),-(signed)mOffset);
				bPlus=TRUE;
			}
			else
			{
				if(bPlus)
					pListing->Print(TEXT("+"));
				else
					bPlus=TRUE;
				_stprintf(s,TEXT("0%Xh"),mOffset);
			}
			pListing->Print(s);
		}
	}

	//���������� ����������� ������
	pListing->Print(TEXT("]"));
}

BOOL COperandMemory::GetReference(DWORD* pReference)
{
#ifdef NIKITIN
	return FALSE;
#endif
	//���� ����� ������������ ������ ���������
	if(mFlags==OMF_OFFSET)
	{
		//������� ������
		*pReference=mOffset;
		return TRUE;
	}
	//����� ������ ���
	return FALSE;
}
